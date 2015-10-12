#include <string>
#include <algorithm>
#include <iostream>
#include "QueryTree.h"
#include "QueryPreprocessor.h"
//#include "..\QueryProcessor\QueryHandler.h"

using namespace std;

string designEntity[] = {"assign","stmt","while","variable","constant","prog_line"};
string keywords[] = { "such that", "pattern" };

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
	vector<string> result;
	return result;
	//QueryHandler handler;
	//return handler.queryRec(tree);
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
		cout << "wrong select" << endl;
		tree->setValidity(false);
		return tree;
	}

	setSuchThatTable(input);
	//	cout << "-----suchThat table----" << endl;
	//	printTable(relations);

	setPatternTable(input);
	//	cout << "-----pattern table----" << endl;
	//	printTable(patterns);



	if (isValidSuchThat() && isValidPattern()) {
		tree->setSymbolTable(declarations);
		tree->setPattern(patterns);
		tree->setResult(selections);
		tree->setSuchThat(relations);
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
	for (int i = 0; i<temp.size() - 1; i++) {
		temp[i] = trim(temp[i]);
		int numWords = countWords(temp[i], " ");
		if (numWords<2) {
			return false;
		}
		string firstWord = getFirstToken(temp[i]);
		firstWord = trim(firstWord);
		string remainWord = removeFirstToken(temp[i]);
		remainWord = trim(remainWord);
		if (!containWord(firstWord, designEntity, 6)) {
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
	extractSelect[1] = trim(extractSelect[1]);

	if (extractSelect[1] == "") {
		return false;
	}

	if (countWords(extractSelect[1], " ") > 1) {
		return false;
	}

	return true;
}
bool QueryPreprocessor::isValidSuchThat(){
	for (int i = 0; i < relations.size(); i++) {
		string clause = relations[i];
		if (clause.find("(") == string::npos || clause.find(")") == string::npos
			|| clause.find(",") == string::npos) {
			cout << "wrong such that 1" << endl;
			return false;
		}
		if (countWords(clause, ",")>2) {
			cout << "wrong such that 2" << endl;
			return false;
		}
	}
	return true;
}
bool QueryPreprocessor::isValidPattern(){
	for (int i = 0; i < patterns.size(); i++) {
		string clause = patterns[i];
		if (clause.find("(") == string::npos || clause.find(")") == string::npos
			|| clause.find(",") == string::npos) {
			cout << "wrong pattern 1" << endl;
			return false;
		}
		if (countWords(clause, ",")>2) {
			cout << "wrong pattern 2" << endl;
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

void QueryPreprocessor::setDeclarationTable(string declare){

    string str = removeMultipleSpace(declare);

    declarations = stringToVector(str, ";");

	for (int i = 0; i < declarations.size(); i++) {
		declarations[i] = trim(declarations[i]);
	}

    declarations.pop_back();
}

void QueryPreprocessor::setSuchThatTable(string input){

    string str = removeMultipleSpace(input);

	vector <string> extractSuchThat = extractContent(str, "such that");

	vector< vector<string> > suchThatClauses = seperateClause(str, "such that");
	relations = setClauseTable(suchThatClauses);	
}

void QueryPreprocessor::setPatternTable(string input){

	string str = removeMultipleSpace(input);

	vector <string> extractPattern = extractContent(str, "pattern");

	string content = extractPattern[1];
	if (content != "") {
		content = trim(content);
		patterns.push_back(content);
	}
}

void QueryPreprocessor::setResultTable(string input){

	string str = removeMultipleSpace(input);

	vector <string> extractSuchThat = extractContent(str, "Select");

	string content = extractSuchThat[1];
	content = trim(content);
	selections.push_back(content);
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

vector<string> QueryPreprocessor::getResultTable() {
	return selections;
}
