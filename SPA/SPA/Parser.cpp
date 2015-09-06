#include "Parser.h"
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

Parser::Parser()
{
}

bool Parser::parseInput(std::string fName) {
	result(fName);
	return true;
}

bool Parser::checkEmptyLine(std::string line) {

}

bool Parser::checkNextLine(char nextLine) {
	return true;
}

void Parser::error() {
	cout << "ERROR";// << line << endl;
	getchar();
	exit(1);
}

vector<string> Parser::result(std::string fName) {
	std::ifstream file(fName);
	std::string str;
	vector<string> stringList;
	while (std::getline(file, str))
	{
		stringList.push_back(str);
	}
	return stringList;

}

/*Parser::~Parser(){
}*/
