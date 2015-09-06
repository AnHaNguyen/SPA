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

Parser::Parser()
{
}

bool Parser::parseInput(string fName) {
	result(fName);
	return true;
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
	string str, word;
	vector<string> processedList;

	str = processedList[i++];
	checkFirstLine(str);

	while (i<stringList.size()) {
		str = stringList[i];
		stringstream  sentence(str);
		sentence << word;
		i = processStmtType(stringList, i, word) + 1;
	}

	processedList = removeSpaces(stringList);

	return processedList;

}

int Parser::processStmtType(vector<string> stringList, int i, string word) {
	int moveLineNum = 0;
	if (word == "procedure") {
		checkProcedure(stringList[i]);
		moveLineNum = 1;
	}
	else if (word == "call") {
		checkCall(stringList[i]);
		moveLineNum = 1;
	}
	else if (word == "if") {
		moveLineNum = checkIf(stringList, i);
	}
	else if (word == "while") {
		moveLineNum = checkWhile(stringList, i);
	}
	else {
		checkAssign(stringList[i]);
		moveLineNum = 1;
	}
	return moveLineNum;
}

void Parser::checkFirstLine(string str) {
	string word;
	stringstream  sentence(str);
	sentence << word;
	if (word != "procedure") {
		error("No such program ");
	}
	else {
		checkProcedure(str);
	}
}

void Parser::checkProcedure(string str) {
	string word, word2;
	stringstream  sentence(str);
	sentence << word << word2;

	if (str.back() == '{' && str.size() > = ) {

	}
	else {
		error("\"Procedure\" syntax error ");
	}

}

void Parser::checkCall(string str) {
	error("\"Call\" syntax error ");
}

void Parser::checkAssign(string str) {
	error("\"Assign\" syntax error ");
}

int Parser::checkIf(vector<string> stringList, int i) {
	error("\"If\" syntax error ");
}

int Parser::checkWhile(vector<string> stringList, int i) {
	error("\"While\" syntax error ");
}

vector<string> Parser::removeSpaces(vector<string> stringList) {
	string str;
	for (int i = 0; i < stringList.size(); i++) {
		str = stringList[i];
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	}
	return stringList;
}

vector<string> Parser::trimmedString(string str) {
	vector<string> trimmedList;
	if (checkNotEmptyLine) {
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
	exit;
}

Parser::~Parser() {
}
