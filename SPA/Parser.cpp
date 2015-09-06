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
		error();
	}
	//processedList = checkSyntax(stringList);
	return processedList;
}

/*vector<string> Parser::checkSyntax(vector<string> stringList) {
int i = 0;
string str, word;
vector<string> processedList;
while(i<stringList.size()) {
str = stringList[i];
stringstream  sentence(str);
sentence << word;
if(word=="procedure") {

}
checkProcedure();
checkStmtType();

}
return processedList;

}*/

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

std::string Parser::trim(std::string str) {
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

string Parser::removeSpaces(string str) {

	return str;
}

void Parser::error() {
	cout << "ERROR";
}

/*Parser::~Parser(){
}*/
