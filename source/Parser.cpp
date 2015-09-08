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

int curlyBrackets = 0, roundBrackets = 0;


Parser::Parser()
{
}

vector<string> Parser::parseInput(string fName) {
	vector<string> programLines = result(fName);

	return removeSpaces(programLines);
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
			stringList = trimmedString(str);
		}
		file.close();
	}
	else {
		error("File cannot open ");
	}
	processedList = checkSyntax(stringList);
	return processedList;
}

vector<string> Parser::checkSyntax(vector<string> stringList) {

	int i = 0;
	string str, type;
	vector<string> processedList;

	checkAllBrackets(stringList);

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
				error("Not a valid expression ");
			}
		}
		else {
			if (word2.find("=") == string::npos) {
				error("Missing \"=\" ");
			}
			else if (!isExpression(word2.substr(1, word2.size() - 1))) {
				error("Not a valid expression ");
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
				error("Not a valid expression ");
			}
		}
		else {
			if (!isName(varName.substr(0, varName.find('=') - 1))) {
				error("Not valid variable name ");
			}
			if (!isExpression(varName.substr(varName.find('=') - 1, varName.size() - 1))) {
				error("Not a valid expression ");
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
		if (!isalnum(c)) {
			return false;
		}
	}
	return true;
}

bool Parser::isName(string name) {
	string letter, digit;
	letter = name.at(0);
	digit = name.substr(1, name.size());

	if (isLetter(letter) && isAlphaNumeric(digit)) {
		return true;
	}
	error("Not valid name ");
	return false;

}

bool Parser::isExpression(string expr) {
	int i = 0, start = -1, end;
	string name;
	char c;

	if (expr.empty()) {
		return false;
	}
	removeLineSpaces(expr);

	while (!expr.empty()) {
		c = expr.at(i);
		if (c == '+' || c == '-' || c == '*' || c == '(') {
			if (c == '(') {
				if (!isPairedRoundBrackets(expr)) {
					error("Missing round brackets ");
				}
			}
			end = i;
			name = expr.substr(start + 1, end - (start + 1));
			if (!isName(name)) {
				error("Invalid variable name ");
			}
			expr = expr.substr(end, expr.size() - end);
			i = 0;
		}
		else {
			i++;
		}
	}
	return true;
}

vector<string> Parser::removeAllSpaces(vector<string> stringList) {
	string str;
	for (int i = 0; i < stringList.size(); i++) {
		str = stringList[i];
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
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
	found1 = stringList[startLine].find("{");
	found2 = stringList[endLine].find("}");
	stmt = stringList[startLine].substr(found1, stringList[startLine].size() - found1 + 1) + stringList[startLine].substr(found2, stringList[endLine].size() - found2 + 1);
	if (isAlphaNumeric(stmt)) {
		return endLine;
	}

	while (startLine + 1 < endLine) {
		if (isAlphaNumeric(stringList[startLine + 1])) {
			return endLine;
		}
		startLine++;
	}

	return -1;
}

int Parser::pairedCurlyBracketsPos(vector<string> stringList, int startIndex) {

	string line;
	int endIndex = stringList.size() - startIndex - 1;

	for (int i = startIndex; i < endIndex; i++) {
		line = stringList[i];
		if (line.find("{") != string::npos) {
			curlyBrackets++;
		}
		if (line.find("{") != string::npos) {
			curlyBrackets--;
		}
		if (curlyBrackets == 0) {
			return i;
		}
	}

	return endIndex;
}

bool Parser::isPairedRoundBrackets(string str) {

	if (str.find("(") != string::npos) {
		roundBrackets++;
	}
	if (str.find(")") != string::npos) {
		roundBrackets--;
	}

	if (roundBrackets != 0) {
		return false;
	}
	else {
		return true;
	}
}

void Parser::checkAllBrackets(vector<string> stringList) {
	string str;

	for (int i = 0; i < stringList.size(); i++) {
		str = stringList[i];
		if (str.find("{") != string::npos) {
			curlyBrackets++;
		}
		if (str.find("}") != string::npos) {
			curlyBrackets--;
		}
		if (str.find("(") != string::npos) {
			roundBrackets++;
		}
		if (str.find(")") != string::npos) {
			roundBrackets--;
		}
	}

	if (curlyBrackets != 0) {
		error("Unpaired curly brackets ");
	}
	if (roundBrackets != 0) {
		error("Unpaired round brackets ");
	}
}

vector<string> Parser::trimmedString(string str) {
	vector<string> trimmedList;
	if (checkNotEmptyLine(str)) {
		trimmedList.push_back(trim(str));
	}
	trimmedList.push_back(str);
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

void Parser::error(string errorType) {
	cout << errorType + "ERROR";
	exit(0);
}

Parser::~Parser() {
}
