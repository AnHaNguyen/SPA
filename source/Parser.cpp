#include "Parser.h"
#include "DesignExtractor.h"
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


Parser::Parser() {
}

Parser::~Parser() {
}

vector<string> Parser::parseInput(string fName) {
	//return result(fName);
	DesignExtractor ext = DesignExtractor(result(fName));
	return result(fName);
}

vector<string> Parser::result(string fName) {

	vector<string> processedString = processFile(fName);
	return removeAllSpaces(processedString);
}

vector<string> Parser::processFile(string fName) {

	string str, errorMsg;
	ifstream file(fName);
	vector<string> stringList, stringList1, stringList2, stringList3, processedList;

	if (file.is_open())
	{
		while (getline(file, str))
		{
			stringList.push_back(str);
		}
		file.close();
	}
	else {
		error("File cannot open 48");
	}

	stringList1 = moveCloseCurlyBracket(trimmedList(stringList));
	stringList2 = moveOpenCurlyBracket(stringList1);

	errorMsg = checkSemiColon(stringList2);
	if (!errorMsg.empty()) {
		error(errorMsg);
	}
	processedList = checkSyntax(stringList2);

	return processedList;
}

vector<string> Parser::checkSyntax(vector<string> stringList) {

	int i = 0;
	string str, type, errorMsg;

	if (checkAllBrackets(stringList) == false) {
		error(errorTypeList[1] + "62");
	}

	str = stringList[i++];
	errorMsg = checkFirstLine(stringList);

	if (!errorMsg.empty()) {
		error(errorMsg + "check first line 69");
	}

	errorMsg = processStmtType(stringList);
	if (!errorMsg.empty()) {
		error(errorMsg);
	}

	return stringList;

}

string Parser::processStmtType(vector<string> stringList) {
	string errorMsg, str, type;
	int isStmt;
	for (int i = 1; i < stringList.size(); i++) {

		str = stringList[i];

		stringstream  sentence(str);
		sentence >> type;
		//cout <<"type is " <<type << "\n";
		if (type == "procedure") {
			errorMsg = checkProcedure(stringList, i);

		}
		else if (type == "call") {
			errorMsg = checkCall(stringList, i);
		}
		else if (type == "if") {
			errorMsg = checkIf(stringList, i);

		}
		else if (type == "while") {
			errorMsg = checkWhile(stringList, i);

		}
		else if (type == "else" || type == "else{") {
			errorMsg = checkElse(stringList, i);

		}
		else {

			errorMsg = checkAssign(stringList[i]);
		}
		if (!errorMsg.empty()) {
			return errorMsg;
		}
	}
	return errorMsg;
}

string Parser::checkFirstLine(vector<string> stringList) {
	string type, error;
	stringstream  sentence(stringList[0]);
	sentence >> type;
	if (type != "procedure") {
		return errorTypeList[6] + "126";
	}
	else {
		error = checkProcedure(stringList, 0);
		return error;//+"check procedure at 131    ";
	}
}

string Parser::checkProcedure(vector<string> stringList, int startLine) {
	string type, word2, word3;
	stringstream  sentence(stringList[startLine]);
	sentence >> type >> word2;

	if (word2.empty()) {
		startLine++;
		stringstream  sentenceNew(stringList[startLine]);
		sentenceNew >> word2;
	}

	if (word2.find('{') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3.empty()) {
			startLine++;
			stringstream  sentenceNew(stringList[startLine]);
			sentenceNew >> word3;
		}
		else if (word3 != "{") {
			return errorTypeList[1] + "153";
		}
		if (!isName(word2)) {
			return errorTypeList[5] + "156";
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 159    ";
		}
		return "";
	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.find('{')))) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 168    ";
		}
		return "";
	}
	else {
		return errorTypeList[4] + "173";
	}
}


string Parser::checkCall(vector<string> stringList, int startLine) {
	string type, word2, word3;
	string str = stringList[startLine];
	stringstream sentence(str);
	sentence >> type >> word2;
	//	cout << "\n\n" << "The string at 224 is " << str << "\n\n";
	if (word2.empty()) {
		startLine++;
		stringstream  sentenceNew(stringList[startLine]);
		sentenceNew >> word2;
	}

	if (word2.find(';') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3.empty()) {
			startLine++;
			stringstream  sentenceNew(stringList[startLine]);
			sentenceNew >> word3;
		}
		else if (word3 != ";") {
			return errorTypeList[8] + "197";
		}
		if (!isName(word2)) {
			return errorTypeList[5] + "200";
		}
		return "";
	}
	else if (word2.find(';') != string::npos && word2.size() >= 2) {

		if (!isName(word2.substr(0, word2.find(';')))) {
			return errorTypeList[5];
		}
		return "";
	}
	else {
		return errorTypeList[7] + "211";
	}

}

string Parser::checkAssign(string str) {
	string varName, expr;
	//	cout <<"\n\n"<< "The string at 224 is "<<str<<"\n\n";
	size_t found1, found2;
	found1 = str.find('=');
	found2 = str.find(';');
	if (found1 != string::npos && found2 != string::npos && found1 < found2) {
		varName = str.substr(0, found1);
		if (!isName(varName)) {
			return errorTypeList[5];
		}
		expr = str.substr(found1 + 1, found2 - (found1 + 1));
		if (!isExpression(expr)) {
			return errorTypeList[2] + "229";
		}
		return "";
	}
	else {
		return errorTypeList[13] + "234";
	}

}

string Parser::checkIf(vector<string> stringList, int startLine) {
	string type, word2, word3, word4;
	stringstream  sentence(stringList[startLine]);
	sentence >> type >> word2 >> word3;
	//cout << stringList[startLine] << "End here \n\n";
	if (word2.empty()) {
		startLine++;
		stringstream  sentenceNew(stringList[startLine]);
		sentenceNew >> word2 >> word3;
	}

	if (word3.empty()) {
		startLine++;
		stringstream  sentenceNew(stringList[startLine]);
		sentenceNew >> word3;
	}


	if (word3.find('{') == string::npos && word3.size() >= 1) {
		sentence >> word4;
		if (word4.empty()) {
			startLine++;
			stringstream  sentenceNew(stringList[startLine]);
			sentenceNew >> word4;
		}
		else if (word4 != "{") {
			return errorTypeList[1] + "265";
		}
		if (!isName(word2)) {
			return " " + word2 + errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 271    ";
		}
		return "";
	}
	else if (word3.find('{') != string::npos && word3.size() >= 2) {

		if (!isName(word2.substr(0, word2.size()))) {

			return errorTypeList[5];
		}
		//	cout << "REACH HERE2";
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 280    ";
		}
		//cout << "REACH HERE3";
		return "";
	}
	else {
		return errorTypeList[11] + "285";
	}
	return "Should not reach here";
}

string Parser::checkElse(vector<string> stringList, int startLine) {
	size_t found;
	string elseStmt, elseStmt1, type, word2, word3;

	elseStmt = stringList[startLine];
	if (elseStmt.find("else") == string::npos) {
		startLine += 1;
		elseStmt = stringList[startLine];
		elseStmt1 = stringList[startLine + 1];
	}
	else {
		elseStmt1 = stringList[startLine + 1];
	}

	if (elseStmt.find('{') != string::npos && elseStmt.find('{') > elseStmt.find("else")) {
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 306    ";
		}
		return "";
	}
	else if (elseStmt1.find('{') != string::npos) {
		if (isStmtLst(stringList, startLine + 1) == -1) {
			return errorTypeList[3] + "error at 312    ";
		}
		return "";
	}
	else {
		return errorTypeList[12] + "317";
	}
}

string Parser::checkWhile(vector<string> stringList, int startLine) {
	string type, word2, word3;
	stringstream  sentence(stringList[startLine]);
	sentence >> type >> word2;

	if (word2.empty()) {
		startLine++;
		stringstream  sentenceNew(stringList[startLine]);
		sentenceNew >> word2;
	}

	if (word2.find('{') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3.empty()) {
			startLine++;
			stringstream  sentenceNew(stringList[startLine]);
			sentenceNew >> word3;
		}
		else if (word3 != "{") {
			return errorTypeList[1] + "340";
		}
		if (!isName(word2)) {
			return errorTypeList[5] + "344";
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 346    ";
		}
		return "";
	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.find('{')))) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3] + "error at 355    ";
		}
		return "";
	}
	else {
		return errorTypeList[10] + "error at 361";
	}

}

bool Parser::isInteger(string const_value) {
	char c;
	for (int i = 0; i < const_value.size(); i++) {
		c = const_value.at(i);
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool Parser::isLetter(string letter) {
	char c;
	for (int i = 0; i < letter.size(); i++) {
		c = letter.at(i);
		if (!isalpha(c)) {
			return false;
		}
	}
	return true;
}

bool Parser::isAlphaNumeric(string str) {
	char c;
	for (int i = 0; i < str.size(); i++) {
		c = str.at(i);
		if (!isalnum(c)) {
			return false;
		}
	}
	return true;
}

bool Parser::existAlphaNumeric(string str) {
	char c;
	for (int i = 0; i < str.size(); i++) {
		c = str.at(i);
		if (isalnum(c)) {
			return true;
		}
	}
	return false;
}

bool Parser::isName(string name) {
	string letter, digit;
	name = removeLineSpaces(name);
	letter = name.at(0);
	digit = name.substr(1, name.size());
	if (name.size() == 1) {
		if (isLetter(letter)) {
			return true;
		}
	}
	if (isLetter(letter) && isAlphaNumeric(digit)) {
		return true;
	}
	return false;

}

bool Parser::isNameOrConstant(string variable) {
	if (isInteger(variable) || isName(variable)) {
		return true;
	}
	return false;
}

string Parser::replaceAll(string str, const string& from, const string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

bool Parser::isExpression(string expr) {
	string word, word2;

	int start = 0;
	string varOrConstant;
	char c, d;

	if (expr.empty()) {
		return false;
	}
	if (!isPairedRoundBrackets(expr)) {
		return false;
	}

	replace(expr.begin(), expr.end(), '(', ' ');
	replace(expr.begin(), expr.end(), ')', ' ');
	expr = replaceAll(expr, "+", " + ");
	expr = replaceAll(expr, "-", " - ");
	expr = replaceAll(expr, "*", " * ");

	stringstream  sentence(expr);
	sentence >> word;

	if (word.empty() || !isNameOrConstant(word)) {
		return false;
	}

	while (!word.empty()) {
		sentence >> word2;

		if (word == word2 || word2.empty()) {
			break;
		}

		if ((word == "+") || (word == "-") || (word == "*")) {
			if (!isNameOrConstant(word2)) {
				return false;
			}
		}
		else {

			if (!isNameOrConstant(word)) {
				return false;
			}
			if ((word2 != "+") && (word2 != "-") && (word2 != "*")) {

				return false;
			}
		}
		word = word2;
	}

	return true;
}

vector<string> Parser::removeAllSpaces(vector<string> stringList) {
	string str;
	for (int i = 0; i < stringList.size(); i++) {
		str = stringList[i];
		stringList[i] = removeLineSpaces(str);
	}
	return stringList;
}

string Parser::removeLineSpaces(string line) {
	string str = line;
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	return str;
}

int Parser::isStmtLst(vector<string> stringList, int startLine) {
	std::size_t found1, found2;
	string stmt, start, end;

	int endLine = pairedCurlyBracketsPos(stringList, startLine);

	if (endLine == -1) {
		//	cout << "512";
		return -1;
	}
	start = stringList[startLine];
	end = stringList[endLine];

	found1 = start.find_first_of("{");
	found2 = end.find_last_of("}");

	stmt = start.substr(found1 + 1, start.size() - found1) + end.substr(found2 + 1, end.size() - found2);

	if (existAlphaNumeric(stmt)) {
		return endLine;
	}
	startLine++;
	while (startLine <= endLine) {
		if (existAlphaNumeric(stringList[startLine])) {
			return endLine;
		}
		startLine++;
	}

	return -1;
}

string Parser::checkSemiColon(vector<string> stringList) {
	vector<string> list = removeAllSpaces(stringList);
	int countSemiColon;
	string line, errorMsg;

	for (int i = 0; i < stringList.size(); i++) {
		line = stringList[i];
		countSemiColon = count(line.begin(), line.end(), ';');
		if (countSemiColon > 1) {
			errorMsg = "Wrong \" ; \" at " + to_string(i);
			return errorMsg;
		}
	}
	return "";
}

int Parser::pairedCurlyBracketsPos(vector<string> stringList, int startIndex) {
	string line;
	int curlyBrackets = 0;
	int countOpen, countClose;
	int endIndex = stringList.size() - 1;

	for (int i = startIndex; i <= endIndex; i++) {
		line = stringList[i];
		countOpen = count(line.begin(), line.end(), '{');
		curlyBrackets += countOpen;

		countClose = count(line.begin(), line.end(), '}');
		curlyBrackets -= countClose;

		if (countOpen < countClose) {
			return i;
		}
		if (curlyBrackets == 0) {
			return i;
		}
	}

	return -1;
}

bool Parser::isPairedRoundBrackets(string str) {
	int roundBrackets = 0;
	int countRoundOpen, countRoundClose;

	countRoundOpen = count(str.begin(), str.end(), '(');
	roundBrackets += countRoundOpen;

	countRoundClose = count(str.begin(), str.end(), ')');
	roundBrackets -= countRoundClose;

	if (roundBrackets != 0) {
		return false;
	}
	else {
		return true;
	}
}

bool Parser::checkAllBrackets(vector<string> stringList) {
	string str;
	int curlyBrackets = 0, roundBrackets = 0;
	int countCurlyOpen, countCurlyClose;
	int countRoundOpen, countRoundClose;

	for (int i = 0; i < stringList.size(); i++) {
		str = stringList[i];
		countCurlyOpen = count(str.begin(), str.end(), '{');
		curlyBrackets += countCurlyOpen;

		countCurlyClose = count(str.begin(), str.end(), '}');
		curlyBrackets -= countCurlyClose;

		if (curlyBrackets < 0) {
			return false;
		}

		countRoundOpen = count(str.begin(), str.end(), '(');
		roundBrackets += countRoundOpen;

		countRoundClose = count(str.begin(), str.end(), ')');
		roundBrackets -= countRoundClose;

		if (roundBrackets < 0) {
			return false;
		}
	}

	if (curlyBrackets != 0 || roundBrackets != 0) {
		return false;
	}

	return true;
}

vector<string> Parser::trimmedList(vector <string> list) {
	vector<string> trimmedList;
	string str;
	int index = 0;

	for (int i = 0; i < list.size(); i++) {
		str = list[i];
		if (checkNotEmptyLine(str)) {
			trimmedList.push_back(trim(str));
		}
	}
	return trimmedList;
}

vector<string> Parser::moveCloseCurlyBracket(vector <string> list) {
	vector<string> result;
	for (int i = 0; i < list.size(); i++) {
		if (list[i].substr(0, 1) == "}") {
			result[result.size() - 1] += list[i];
		}
		else {
			result.push_back(list[i]);
		}
	}
	return result;
}

vector<string> Parser::moveOpenCurlyBracket(vector <string> list) {
	vector<string> result;
	for (int i = 0; i < list.size(); i++) {
		if (list[i].substr(0, 1) == "{") {
			result[result.size() - 1] += list[i];
		}
		else {
			result.push_back(list[i]);
		}
	}
	return result;
}

bool Parser::checkNotEmptyLine(string line) {
	if (trim(line).size() == 0) {
		return false;
	}
	return true;
}

string Parser::trim(string str) {
	string temp;
	if (str == "") {
		return str;
	}

	int i = 0;
	int j = 0;

	for (i = 0; i < str.size(); i++) {
		temp = str.at(i);

		if (str[i] != ' ' && str[i] != '	') {
			break;
		}
	}

	for (j = str.size() - 1; j >= i; j--) {
		if (str[j] != ' ')
			break;
	}

	return str.substr(i, j - i + 1);
}

bool Parser::isEqualVector(vector<string> v1, vector<string> v2) {
	int start = 0, end = v1.size();
	if (v1.size() == v2.size()) {
		for (int i = 0; i < v1.size(); i++) {
			if (v1[i].compare(v2[i]) != 0) {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;
}


void Parser::error(string errorType) {
	if (errorType.size() != 0) {
		cout << errorType + " ERROR";
		exit(1);
	}
}


