#include<string>
#include<stdlib.h>
#include<vector>

#pragma once
class Parser
{
public:
	Parser();
	bool parseInput(string fName);
	vector<string> Parser::result(string fName);

private:
	bool checkNextLine(char nextLine);
	void Parser::error();
	bool Parser::checkEmptyLine(string line);
	//~Parser();
};

