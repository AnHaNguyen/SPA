#include <string>
#include <algorithm>
#include <iostream>
#include "QueryTree.h"
#include "QueryPreprocessor.h"
#include "..\QueryProcessor\QueryHandler.h"

using namespace std;

string designEntity[] = {"assign","stmt","while","variable","constant","prog_line","if","procedure","call","program","stmtLst","plus","minus","times"};
string keywords[] = { "such that", "pattern", "with" };

QueryPreprocessor::QueryPreprocessor(){
}

QueryPreprocessor::~QueryPreprocessor(){
}

vector<string> QueryPreprocessor::start(string line) {

	string declare = "", input = "";
	if (line.find("Select") != string::npos) {
		vector<string> seperate = stringToVector(line, "Select");
		declare = trim(seperate[0]);
		input = trim("Select" + seperate[1]);
	}
	QueryTree* tree = startProcess(declare, input);
	if (tree->getValidity() == false) {
		//cout << "================== Invalid QueryTree ===================" << endl;
	}
	//vector<string> result;
	//return result;
	QueryHandler handler;
	return handler.queryRec(tree);
}

QueryTree* QueryPreprocessor::startProcess(string declare, string input) {
	declare = trim(declare);
	declare = removeMultipleSpace(declare);
	input = trim(input);
	input = removeMultipleSpace(input);


	QueryTree* tree = new QueryTree();

	if (isValidDeclaration(declare)) {
		setDeclarationTable(declare);
		//  cout << "-----declaration table----" << endl;
		//  printTable(declarations);
	}
	else {
		//cout << "wrong declare" << endl;
		tree->setValidity(false);
		return tree;
	}

	if (isValidSelection(input)) {
		setResultTable(input);
		//	cout << "-----selection table----" << endl;
		//	printTable(selections);
	}
	else {
		//cout << "wrong select" << endl;
		tree->setValidity(false);
		return tree;
	}

	setSuchThatTable(input);
	//	cout << "-----suchThat table----" << endl;
	//	printTable(relations);

	setPatternTable(input);
	//	cout << "-----pattern table----" << endl;
	//	printTable(patterns);

	setWithTable(input);

	if (isValidSuchThat() && isValidPattern() && isValidWith()) {
		tree->setSymbolTable(declarations);
		tree->setPattern(patterns);
		tree->setResult(selections);
		tree->setSuchThat(relations);
		tree->setWith(withs);
		return tree;
	}
    else{
        tree->setValidity(false);
        return tree;
    }
}

bool QueryPreprocessor::isValidDeclaration(string declare){
	string str = declare;
	if (str.find(";") == string::npos) {
		return false;
	}

	vector<string> temp = stringToVector(str, ";");
	for (int i = 0; i<temp.size()-1; i++) {
		temp[i] = trim(temp[i]);
		int numWords = countWords(temp[i], " ");
		if (numWords<2) {
			return false;
		}
		string firstWord = getFirstToken(temp[i]);
		firstWord = trim(firstWord);
		string remainWord = removeFirstToken(temp[i]);
		remainWord = trim(remainWord);
		if (!containWord(firstWord, designEntity, 14)) {
			return false;
		}
		if (remainWord.find(",") != string::npos) {
			if (countWords(remainWord, ",")<2) {
				return false;
			}
			vector<string> aroundComma = stringToVector(remainWord, ",");
			if (aroundComma[aroundComma.size() - 1] == "") {
				return false;
			}
		}
		else {
			if (countWords(remainWord, " ")>1) {
				return false;
			}
		}
	}
	if (temp[temp.size() - 1] != "") {
		return false;
	}
	return true;
}

bool QueryPreprocessor::isValidSelection(string input){
	vector<string> extractSelect = extractContent(input, "Select");
	string str = trim(extractSelect[1]);
	str = removeSpace(str);

	if (str == "") {
		return false;
	}

	if (str.find("<") != string::npos && str.find(">") == string::npos) {
		return false;
	}

	if (str.find("<") == string::npos && str.find(">") != string::npos) {
		return false;
	}

	if (str.find("<") == 0 && str.find(">") == str.size()-1) {
		str = str.substr(1, str.length() - 2);
		vector<string> words = stringToVector(str, ",");
		for (int i = 0; i < words.size(); i++) {
			if (words[i] == "") {
				return false;
			}
		}
	}
	return true;
}

bool QueryPreprocessor::isValidSuchThat(){

	for (int i = 0; i < relations.size(); i++) {
		string clause = relations[i];
		if (clause.find("(") == string::npos || clause.find(")") == string::npos
			|| clause.find(",") == string::npos) {
			//cout << "wrong such that 1" << endl;
			return false;
		}
		if (countWords(clause, ",")>2) {
			//cout << "wrong such that 2" << endl;
			return false;
		}
		vector <string> seperate = stringToVector(clause, ")");
		if (seperate.size() > 0) {
			if (trim(seperate[1]) != "") {
				//cout << " extra word: " << seperate[1] << endl;
				return false;
			}
		}
	}

	return true;
}

bool QueryPreprocessor::isValidPattern(){
	for (int i = 0; i < patterns.size(); i++) {
		string clause = patterns[i];
		if (clause.find("(") == string::npos || clause.find(")") == string::npos
			|| clause.find(",") == string::npos) {
			//cout << "wrong pattern 1" << endl;
			return false;
		}
		if (countWords(clause, ",")>3) {
			//cout << "wrong pattern 2" << endl;
			return false;
		}
		vector <string> seperate = stringToVector(clause, ")");
		if (seperate.size() > 0) {
			if (trim(seperate[1]) != "") {
				//cout << " extra word: " << seperate[1] << endl;
				return false;
			}
		}
	}
	return true;
}

bool QueryPreprocessor::isValidWith() {
	for (int i = 0; i < withs.size(); i++) {
		string clause = withs[i];
		if (clause.find("=") == string::npos) {
			return false;
		}
		if (countWords(clause, ".") > 3) {
			return false;
		}
	}
	return true;
}

int QueryPreprocessor::identifyKeyword(string str) {
	int minIndex = 999999;
	for (int i = 0; i<sizeof(keywords) / sizeof(string); i++) {
		if (str.find(keywords[i]) != string::npos) {
			if (minIndex>str.find(keywords[i])) {
				minIndex = str.find(keywords[i]);
			}
		}
	}
	return minIndex;
}

//vector:  [clause][content][remain]
vector<string> QueryPreprocessor::extractContent(string str, string clause) {
	int keywordStart;
	int keywordEnd;
	int nextWordStart;
	string nextWord;
	string content;
	string remain;
	vector<string> separation;

	if (str.find(clause) == string::npos) {
		separation.push_back("");
		separation.push_back("");
		separation.push_back("");
		return separation;
	}

	separation.push_back(clause);
	keywordStart = str.find(clause);
	keywordEnd = keywordStart + clause.size() - 1;
	string removeKeyword = str.substr(keywordEnd + 1);

	if (identifyKeyword(removeKeyword) == 999999) {
		separation.push_back(removeKeyword);
		separation.push_back("");
		return separation;
	}

	nextWordStart = identifyKeyword(removeKeyword);
	content = removeKeyword.substr(0, nextWordStart);
	remain = removeKeyword.substr(nextWordStart);
	separation.push_back(content);
	separation.push_back(remain);

	return separation;
}

vector< vector<string> > QueryPreprocessor::seperateClause(string str, string clause) {
	vector< vector<string> > table;
	while (str.find(clause) != string::npos) {
		table.push_back(extractContent(str, clause));
		str = table[table.size() - 1][2];
	}
	return table;
}

vector<string> QueryPreprocessor::setClauseTable(vector< vector<string> > table) {
	vector<string> newTable;
	for (int i = 0; i<table.size(); i++) {
		string str = table[i][1];
		vector<string> seperateAnd = stringToVector(str, "and");
		for (int j = 0; j<seperateAnd.size(); j++) {
			string content = trim(seperateAnd[j]);
			newTable.push_back(content);
		}
	}
	return newTable;
}

vector<string> QueryPreprocessor::setClauseTableForSuchThat(vector< vector<string> > table) {
	vector<string> newTable;
	for (int i = 0; i<table.size(); i++) {
		string str = table[i][1];
		vector<string> seperateAnd = stringToVector(str, "and");
		for (int j = 0; j<seperateAnd.size(); j++) {
			string content = trim(seperateAnd[j]);
			newTable.push_back(content);
		}
	}

	newTable = sortAttribute(newTable);
	newTable = sortRelation(newTable);
	return newTable;
}

void QueryPreprocessor::setDeclarationTable(string declare){

    string str = removeMultipleSpace(declare);

    declarations = stringToVector(str, ";");

	for (int i = 0; i < declarations.size(); i++) {
		declarations[i] = trim(declarations[i]);
	}

    declarations.pop_back();
}

void QueryPreprocessor::setSuchThatTable(string input) {

	string str = removeMultipleSpace(input);
	vector< vector<string> > suchThatClauses = seperateClause(str, "such that");
	relations = setClauseTableForSuchThat(suchThatClauses);
}

vector<string> QueryPreprocessor::sortAttribute(vector<string> table) {
	vector<string> twoInteger;
	vector<string> oneInteger;
	vector<string> noInteger;
	vector<string> invalid;
	vector<string> sorted;
	for (int i = 0; i < table.size(); i++) {
		string str = table[i];
		str = removeSpace(str);

		if (str.find("(") != string::npos && str.find(")") != string::npos && str.find(",") != string::npos) {
			vector<string> words = stringToVector(str, "(");
			vector<string> attributes = stringToVector(words[1], ",");
			string first = trim(attributes[0]);
			vector<string> remain = stringToVector(attributes[1], ")");
			string second = trim(remain[0]);

			if (isInteger(first) && isInteger(second)) {
				twoInteger.push_back(str);
			}
			else if (isInteger(first) || isInteger(second)) {
				oneInteger.push_back(str);
			}
			else {
				noInteger.push_back(str);
			}
		}
		else {
			invalid.push_back(str);
		}
	}

	for (int i = 0; i < twoInteger.size(); i++) {
		sorted.push_back(twoInteger[i]);
	}
	for (int i = 0; i < oneInteger.size(); i++) {
		sorted.push_back(oneInteger[i]);
	}
	for (int i = 0; i < noInteger.size(); i++) {
		sorted.push_back(noInteger[i]);
	}
	for (int i = 0; i < invalid.size(); i++) {
		sorted.push_back(invalid[i]);
	}
	return sorted;
}

vector<string> QueryPreprocessor::sortRelation(vector<string> table) {
	vector<string> Calls;
	vector<string> CallsS;
	vector<string> Modifies;
	vector<string> Parent;
	vector<string> ParentS;
	vector<string> Follows;
	vector<string> FollowsS;
	vector<string> Uses;
	vector<string> Next;
	vector<string> NextS;
	vector<string> Affects;
	vector<string> AffectsS;
	vector<string> others;
	vector<string> sorted;

	for (int i = 0; i < table.size(); i++) {
		string str = table[i];
		str = removeSpace(str);

		vector<string> words = stringToVector(str, "(");
		string syn = trim(words[0]);

		if (syn=="Calls") {
			Calls.push_back(str);
		}
		else if (syn == "Calls*") {
			CallsS.push_back(str);
		}
		else if (syn == "Modifies") {
			Modifies.push_back(str);
		}
		else if (syn == "Parent") {
			Parent.push_back(str);
		}
		else if (syn == "Parent*") {
			ParentS.push_back(str);
		}
		else if (syn == "Follows") {
			Follows.push_back(str);
		}
		else if (syn == "Follows*") {
			FollowsS.push_back(str);
		}
		else if (syn == "Uses") {
			Uses.push_back(str);
		}
		else if (syn == "Next") {
			Next.push_back(str);
		}
		else if (syn == "Next*") {
			NextS.push_back(str);
		}
		else if (syn == "Affects") {
			Affects.push_back(str);
		}
		else if (syn == "Affects*") {
			AffectsS.push_back(str);
		}
		else {
			others.push_back(str);
		}
	}

	for (int i = 0; i < Calls.size(); i++) {
		sorted.push_back(Calls[i]);
	}
	for (int i = 0; i < CallsS.size(); i++) {
		sorted.push_back(CallsS[i]);
	}
	for (int i = 0; i < Modifies.size(); i++) {
		sorted.push_back(Modifies[i]);
	}
	for (int i = 0; i < Parent.size(); i++) {
		sorted.push_back(Parent[i]);
	}
	for (int i = 0; i < ParentS.size(); i++) {
		sorted.push_back(ParentS[i]);
	}
	for (int i = 0; i < Follows.size(); i++) {
		sorted.push_back(Follows[i]);
	}
	for (int i = 0; i < FollowsS.size(); i++) {
		sorted.push_back(FollowsS[i]);
	}
	for (int i = 0; i < Uses.size(); i++) {
		sorted.push_back(Uses[i]);
	}
	for (int i = 0; i < Next.size(); i++) {
		sorted.push_back(Next[i]);
	}
	for (int i = 0; i < NextS.size(); i++) {
		sorted.push_back(NextS[i]);
	}
	for (int i = 0; i < Affects.size(); i++) {
		sorted.push_back(Affects[i]);
	}
	for (int i = 0; i < AffectsS.size(); i++) {
		sorted.push_back(AffectsS[i]);
	}
	for (int i = 0; i < others.size(); i++) {
		sorted.push_back(others[i]);
	}

	if (sorted.size() > 1) {
		for (int i = 0; i < sorted.size() - 1; i++) {
			if (sorted[i] == sorted[i + 1]) {
				sorted[i + 1] = "";
			}
		}
	}
	vector<string> removeDuplicate;
	for (int i = 0; i < sorted.size(); i++) {
		if (sorted[i] != "") {
			removeDuplicate.push_back(sorted[i]);
		}
	}

	return removeDuplicate;
}

void QueryPreprocessor::setPatternTable(string input){

	string str = removeMultipleSpace(input);
	vector< vector<string> > patternClauses = seperateClause(str, "pattern");
	patterns = setClauseTable(patternClauses);
}

void QueryPreprocessor::setWithTable(string input) {

	string str = removeMultipleSpace(input);
	vector< vector<string> > withClauses = seperateClause(str, "with");
	withs = setClauseTable(withClauses);
}

void QueryPreprocessor::setResultTable(string input){

	string str = removeMultipleSpace(input);
	vector <string> extractResult = extractContent(str, "Select");
	string content = extractResult[1];
	content = trim(content);
	content = removeSpace(content);

	if (content.find("<") == string::npos && content.find(">") == string::npos) {
		selections.push_back(content);
	}

	if (content.find("<") == 0 && content.find(">") == content.size() - 1) {
		content = content.substr(1, content.length() - 2);
		vector<string> results = stringToVector(content, ",");
		for (int i = 0; i < results.size(); i++) {
			selections.push_back(results[i]);
		}
	}
}

vector<string> QueryPreprocessor::stringToVector(string original, string delimiter){

    string s = original;
    vector<string> words;

    int position = 0;
    string token;
    while ((position = s.find(delimiter)) != string::npos) {
        token = s.substr(0, position);
        words.push_back(token);
        s.erase(0, position + delimiter.length());
    }
    words.push_back(s);
    return words;
}

string QueryPreprocessor::toLowerCase(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int QueryPreprocessor::countWords(string str, string delimiter){
    vector<string> words = stringToVector(str,delimiter);
    return words.size();
}

string QueryPreprocessor::trim(string str){
    if (str==""){
        return str;
    }

	int i=0;
	int j=0;
	for (i=0;i<str.size();i++){
		if (str[i]!=' ')
			break;
	}
	for (j=str.size()-1;j>=i;j--){
		if (str[j]!=' ')
			break;
	}
	return str.substr(i,j-i+1);
}

string QueryPreprocessor::removeMultipleSpace(string str){

    string result = "";
    for(int i=0; i<str.size()-1; i++){
        if(!(str.at(i)==' ' && str.at(i+1)==' ')){
            result = result + str.at(i);
        }
    }
	if (str.at(str.size() - 1) != ' ') {
		result = result + str.at(str.size() - 1);
	}
    return result;
}

string QueryPreprocessor::removeSpace(string str) {
	string result = "";
	for (int i = 0; i<str.size(); i++) {
		if (str.at(i) != ' ') {
			result = result + str.at(i);
		}
	}
	return result;
}

string QueryPreprocessor::getFirstToken(string str){
    string result = "";
    for(int i=0; i<str.size(); i++){
        if(str.at(i) != ' ')
            result = result + str.at(i);
        else
            break;
    }
    return result;
}

string QueryPreprocessor::removeFirstToken(string str){
    str = trim(str);
    string result = "";
    int index=0;

    for(int i=0; i<str.size(); i++){
        if(str.at(i) == ' '){
           index=i;
           break;
        }
    }

    for(int j=index+1; j<str.size(); j++){
        result = result + str.at(j);
    }

    return result;
}

bool QueryPreprocessor::containWord(string str, string arr[], int size){
    for(int i=0; i<size; i++){
        if(arr[i]==str){
            return true;
        }
    }
    return false;
}

bool QueryPreprocessor::isInteger(string str) {
	if (str.size() == 0) {
		return false;
	}
	for (int i = 0; i < str.size(); i++) {
		if (!isdigit(str.at(i))) {
			return false;
		}
	}
	return true;
}

void QueryPreprocessor::printTable(vector<string> table) {
	for (int i = 0; i<table.size(); i++) {
		cout << table[i] << endl;
	}
}

vector<string> QueryPreprocessor::getDeclarationTable() {
	return declarations;
}

vector<string> QueryPreprocessor::getSuchThatTable() {
	return relations;
}

vector<string> QueryPreprocessor::getPatternTable() {
	return patterns;
}

vector<string> QueryPreprocessor::getWithTable() {
	return withs;
}

vector<string> QueryPreprocessor::getResultTable() {
	return selections;
}
