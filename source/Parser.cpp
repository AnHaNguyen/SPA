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

void Parser::parseInput(string fName) {
	vector<string> programLines = result(fName);
	DesignExtractor ext = DesignExtractor(result(fName));

}

vector<string> Parser::result(string fName) {

	vector<string> processedString = processFile(fName);
	return removeAllSpaces(processedString);
}

vector<string> Parser::processFile(string fName) {

	string str;
	ifstream file(fName);
	vector<string> stringList, processedList;

	if (file.is_open())
	{
		while (getline(file, str))
		{
			stringList.push_back(str);
		}
		file.close();
	}
	else {
		error("File cannot open ");
	}
	stringList = trimmedList(stringList);
	processedList = moveCloseCurlyBracket(checkSyntax(stringList));
	return processedList;
}

vector<string> Parser::checkSyntax(vector<string> stringList) {

	int i = 0;
	string str, type, errorMsg;

	if (checkAllBrackets(stringList) == false) {
		error(errorTypeList[1]);
	}

	str = stringList[i++];
	errorMsg = checkFirstLine(stringList);
	if (!errorMsg.empty()) {
		error(errorMsg);
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

		if (type == "procedure") {
			errorMsg = checkProcedure(stringList, i);
			return errorMsg;

		}
		else if (type == "call") {
			errorMsg = checkCall(stringList, i);
			return errorMsg;
		}
		else if (type == "if") {
			errorMsg = checkIf(stringList, i);
			return errorMsg;
		}
		else if (type == "while") {
			errorMsg = checkWhile(stringList, i);
			return errorMsg;
		}
		else if (type == "else") {
			errorMsg = checkElse(stringList, i);
			return errorMsg;

		}
		else if (type == "{" || type == "}") {
			isStmt = isStmtLst(stringList, i);
			if (isStmt == -1) {
				errorMsg = errorTypeList[3];
			}
			return errorMsg;
		}
		else {
			errorMsg = checkAssign(stringList[i]);
			return errorMsg;
		}
	}

}

string Parser::checkFirstLine(vector<string> stringList) {
	string type, error;
	stringstream  sentence(stringList[0]);
	sentence >> type;
	if (type != "procedure") {
		return errorTypeList[6];
	}
	else {
		error = checkProcedure(stringList, 0);
		return error;
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
			return errorTypeList[1];
		}
		if (!isName(word2)) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.find('{')))) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else {
		return errorTypeList[4];
	}
}

string Parser::checkCall(vector<string> stringList, int startLine) {
	string type, word2, word3;
	string str = stringList[startLine];
	stringstream sentence(str);
	sentence >> type >> word2;

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
			return errorTypeList[8];
		}
		if (!isName(word2)) {
			return errorTypeList[5];
		}
		return "";
	}
	else if (word2.find(';') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.find('{')))) {
			return errorTypeList[5];
		}
		return "";
	}
	else {
		return errorTypeList[7];
	}

}

string Parser::checkAssign(string str) {
	string varName, expr;

	size_t found1, found2;
	found1 = str.find('=');
	found2 = str.find(';');
	if (found1 != string::npos && found2 != string::npos && found1 < found2) {
		varName = str.substr(0, found1);
		if (!isName(varName)) {
			return "\"" + varName + "\"" + errorTypeList[5];
		}
		expr = str.substr(found1 + 1, found2 - (found1 + 1));
		if (!isExpression(expr)) {
			return errorTypeList[2];
		}
		return "";
	}
	else {
		return errorTypeList[13];
	}

}

string Parser::checkIf(vector<string> stringList, int startLine) {
	string type, word2, word3, word4;
	stringstream  sentence(stringList[startLine]);
	sentence >> type >> word2 >> word3;

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
			return errorTypeList[1];
		}
		if (!isName(word2)) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else if (word3.find('{') != string::npos && word3.size() >= 2) {
		if (!isName(word2.substr(0, word2.size() - 1))) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else {
		return errorTypeList[11];
	}

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
			return errorTypeList[3];
		}
		return "";
	}
	else if (elseStmt1.find('{') != string::npos) {
		if (isStmtLst(stringList, startLine + 1) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else {
		return errorTypeList[12];
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
			return errorTypeList[1];
		}
		if (!isName(word2)) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.size() - 1))) {
			return errorTypeList[5];
		}
		if (isStmtLst(stringList, startLine) == -1) {
			return errorTypeList[3];
		}
		return "";
	}
	else {
		return errorTypeList[10];
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

bool Parser::isExpression(string expr) {
	int start = 0;
	string varOrConstant;
	char c, d;

	if (expr.empty()) {
		return false;
	}
	expr = removeLineSpaces(expr);

	if (expr.at(0) == '(') {
		if (!isPairedRoundBrackets(expr)) {
			cout << "Not paired Round Brackets";
		}
	}
	for (int i = 1; i < expr.size(); i++) {
		c = expr.at(i);
		d = expr.at(i - 1);
		if (c == '(') {
			if (d == '+' || d == '-' || d == '*') {
			}
			else {
				cout << "Wrong";
			}
		}
	}
	replace(expr.begin(), expr.end(), '(', ' ');
	replace(expr.begin(), expr.end(), ')', ' ');
	expr = removeLineSpaces(expr);

	for (int i = 0; i < expr.size(); i++) {
		c = expr.at(i);

		if (c == '+' || c == '-' || c == '*' || c == '(') {
			varOrConstant = expr.substr(start, i - start);
			start = i + 1;
			if (!isNameOrConstant(varOrConstant)) {
				return false;
			}

		}
		else if (i == (expr.size() - 1)) {
			varOrConstant = expr.substr(start, expr.size());
			if (!isNameOrConstant(varOrConstant)) {
				return false;
			}
		}

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
		return -1;
	}
	start = stringList[startLine];
	end = stringList[endLine];
	found1 = start.find_first_of("{");
	found2 = end.find_last_of("}");

	stmt = start.substr(found1, start.size() - found1) + end.substr(found2, end.size() - found2);

	if (existAlphaNumeric(stmt)) {
		return endLine;
	}
	startLine++;
	while (startLine < endLine) {
		if (existAlphaNumeric(stringList[startLine])) {
			return endLine;
		}
		startLine++;
	}

	return -1;
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
		if (list[i] == "}") {
			result[result.size() - 1] += "}";
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
	if (str == "") {
		return str;
	}

	int i = 0;
	int j = 0;

	for (i = 0; i<str.size(); i++) {
		if (str[i] != ' ')
			break;
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
		cout << errorType + "ERROR";
		exit(1);
	}
}


