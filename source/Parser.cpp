#include "Parser.h"
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
	vector<string> programLines = result(fName);

	return removeAllSpaces(programLines);
}

vector<string> Parser::result(string fName) {

	vector<string> processedString = processFile(fName);
	return processedString;
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
	processedList = checkSyntax(stringList);
	return processedList;
}

vector<string> Parser::checkSyntax(vector<string> stringList) {

	int i = 0;
	string str, type;
	vector<string> processedList;

	if (checkAllBrackets(stringList) == false) {
		error(errorTypeList[1]);
	}

	str = processedList[i++];
	checkFirstLine(stringList);

	while (i < stringList.size()) {
		str = stringList[i];
		stringstream  sentence(str);
		sentence << type;
		processStmtType(stringList, i, type);
		i++;
	}

	return processedList;

}

void Parser::processStmtType(vector<string> stringList, int i, string type) {

	if (type == "procedure") {
		checkProcedure(stringList, i);
	}
	else if (type == "call") {
		checkCall(stringList[i]);
	}
	else if (type == "if") {
		checkIf(stringList, i);
	}
	else if (type == "while") {
		checkWhile(stringList, i);
	}
	else if (type == "else") {

	}
	else {
		checkAssign(stringList[i]);
	}
}

void Parser::checkFirstLine(vector<string> stringList) {
	string type;
	stringstream  sentence(stringList[0]);
	sentence << type;
	if (type != "procedure") {
		error("No such program ");
	}
	else {
		checkProcedure(stringList, 0);
	}
}

void Parser::checkProcedure(vector<string> stringList, int startLine) {
	string type, word2, word3;
	stringstream  sentence(stringList[startLine]);
	sentence >> type >> word2;

	if (isStmtLst(stringList, startLine) == -1) {
		error("Not valid statement list ");
	}

	if (word2.find('{') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3 != "{") {
			error("No { ");
		}
		if (!isName(word2)) {
			error("Not valid procedure name ");
		}

	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.size() - 1))) {
			error("Not valid procedure name ");
		}
	}
	else {
		error("\"Procedure\" syntax ");
	}

}

void Parser::checkCall(string str) {
	string type, word2, word3;
	stringstream sentence(str);
	sentence >> type >> word2;

	if (word2.find(';') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3 != ";") {
			error("No ; ");
		}
		if (!isName(word2)) {
			error("Not valid procedure name to call ");
		}
	}
	else if (word2.find(';') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.size() - 1))) {
			error("Not valid procedure name to call ");
		}
	}
	else {
		error("\"Call\" syntax ");
	}

}

void Parser::checkAssign(string str) {
	string varName, word2, word3;
	stringstream sentence(str);
	sentence >> varName;

	if (varName.find('=') == string::npos) {
		sentence >> word2;
		if (!isName(varName)) {
			error("Not valid variable name ");
		}
		if (word2.size() == 1) {
			if (word2 != "=") {
				error("Not valid procedure name to call ");
			}
			sentence >> word3;
			if (!isExpression(word3)) {
				errorTypeList[2];;
			}
		}
		else {
			if (word2.find("=") == string::npos) {
				error("Missing \"=\" ");
			}
			else if (!isExpression(word2.substr(1, word2.size() - 1))) {
				errorTypeList[2];
			}
		}

	}
	else if (varName.find('=') != string::npos) {
		if (varName.find('=') == varName.size() - 1) {
			if (!isName(varName.substr(0, varName.size() - 1))) {
				error("Not valid variable name ");
			}
			sentence >> word2;
			if (!isExpression(word2)) {
				errorTypeList[2];
			}
		}
		else {
			if (!isName(varName.substr(0, varName.find('=') - 1))) {
				error("Not valid variable name ");
			}
			if (!isExpression(varName.substr(varName.find('=') - 1, varName.size() - 1))) {
				errorTypeList[2];
			}

		}
	}
	else {
		error("\"Assign\" syntax ");
	}
}

void Parser::checkIf(vector<string> stringList, int i) {
	string type, word2, word3, word4;
	stringstream sentence(stringList[i]);
	sentence >> type >> word2 >> word3;

	int endLine = isStmtLst(stringList, i);
	if (endLine == -1) {
		error("Not valid statement list ");
	}
	else {
		checkElse(stringList, endLine);
	}

	if (!isName(word2)) {
		error("Not valid variable name ");
	}

	if (word3.find('{') == string::npos && word2.size() >= 1) {
		sentence >> word4;
		if (word3 != "then") {
			error("Missing then statement ");
		}
		if (word4 != "{") {
			error("Missing { ");
		}
	}
	else if (word3.find('{') != string::npos && word3.size() >= 2) {
		if (word3.find("then") == string::npos || word3.find('{') < word3.find("then")) {
			error("Not valid then statement ");
		}
	}
	else {
		error("\"If\" syntax ");
	}
}

void Parser::checkElse(vector<string> stringList, int i) {
	string elseStmt;
	int startLine = i + 1;
	elseStmt = stringList[startLine];
	if (elseStmt.find("else") != string::npos || elseStmt.find("{") != string::npos || elseStmt.find("else") < elseStmt.find("{")) {
		if (isStmtLst(stringList, startLine) == -1) {
			error("Invalid else statement ");
		}
	}
	else {
		error("Invalid else statement ");
	}
}

void Parser::checkWhile(vector<string> stringList, int i) {
	string type, word2, word3;
	stringstream sentence(stringList[i]);
	sentence >> type >> word2;
	int endLine = isStmtLst(stringList, i);

	if (endLine == -1) {
		error("Not valid statement list ");
	}

	if (word2.find('{') == string::npos && word2.size() >= 1) {
		sentence >> word3;
		if (word3 != "{") {
			error("No { ");
		}
		if (!isName(word2)) {
			error("Not valid variable name ");
		}
	}
	else if (word2.find('{') != string::npos && word2.size() >= 2) {
		if (!isName(word2.substr(0, word2.size() - 1))) {
			error("Not valid variable name ");
		}

	}
	else {
		error("\"While\" syntax ");
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
	letter = name.at(0);
	digit = name.substr(1, name.size());

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
	char c;

	if (expr.empty()) {
		return false;
	}
	expr = removeLineSpaces(expr);

	for (int i = 0; i < expr.size(); i++) {
		c = expr.at(i);

		if (c == '+' || c == '-' || c == '*' || c == '(') {
			if (c == '(') {
				if (!isPairedRoundBrackets(expr)) {
					return false;
				}
			}
			varOrConstant = expr.substr(start, i - start);
			start = i + 1;
			if (!isNameOrConstant(varOrConstant)) {
				return false;
			}
			if ((c == '+' || c == '-' || c == '*') && i < (expr.size() - 1) && expr.at(i + 1) == '(') {
				start += 1;
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
	string stmt;

	int endLine = pairedCurlyBracketsPos(stringList, startLine);
	if (endLine == -1) {
		return -1;
	}
	found1 = stringList[startLine].find("{");
	found2 = stringList[endLine].find("}");
	stmt = stringList[startLine].substr(found1, stringList[startLine].size() - found1 + 1) + stringList[startLine].substr(found2, stringList[endLine].size() - found2 + 1);
	if (existAlphaNumeric(stmt)) {
		return endLine;
	}

	while (startLine + 1 < endLine) {
		if (existAlphaNumeric(stringList[startLine + 1])) {
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
	cout << errorType + "ERROR";
	exit(1);
}


