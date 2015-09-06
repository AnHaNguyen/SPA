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
	vector<string> Parser::checkSyntax(vector<string> stringList);
	vector<string> Parser::removeSpaces(vector<string> stringList);
	int Parser::processStmtType(vector<string> stringList, int i, string word);
	void Parser::checkFirstLine(string str);
	void Parser::checkProcedure(string procedure);
	void Parser::checkCall(string str);
	void Parser::checkAssign(string str);
	int Parser::checkIf(vector<string> stringList, int i);
	int Parser::checkWhile(vector<string> stringList, int i);
	vector<string> Parser::trimmedString(string str);
	bool Parser::checkNotEmptyLine(string line);
	string Parser::trim(string str);
	vector<string> Parser::removeSpaces(vector<string> stringList);
	void Parser::error(string errorType);
	~Parser();
};

#endif

