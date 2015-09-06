#ifndef Parser_H
#define Parser_H

#include<string>
#include<stdlib.h>
#include<vector>
using namespace std;
#pragma once
class Parser
{
public:
	Parser();
	bool parseInput(string fName);
	std::vector<string> Parser::result(string fName);

private:
	vector<string> Parser::processFile(string fName);
	//	vector<string> Parser::checkSyntax(vector<string> stringList);
	vector<string> Parser::trimmedString(string str);
	bool Parser::checkNotEmptyLine(string line);
	string Parser::trim(string str);
	string Parser::removeSpaces(string str);
	void Parser::error();
	//~Parser();
};

#endif

