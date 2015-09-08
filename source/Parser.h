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
	vector<string> Parser::parseInput(string fName);
	std::vector<string> Parser::result(string fName);

private:
	vector<string> Parser::processFile(string fName);
	vector<string> Parser::checkSyntax(vector<string> stringList);
	vector<string> Parser::removeSpaces(vector<string> stringList);
	void Parser::processStmtType(vector<string> stringList, int i, string word);
	void Parser::checkFirstLine(vector<string> stringList);
	void Parser::checkProcedure(vector<string> stringList, int startLine);
	void Parser::checkCall(string str);
	void Parser::checkAssign(string str);
	void Parser::checkIf(vector<string> stringList, int i);
	void Parser::checkElse(vector<string> stringList, int i);
	void Parser::checkWhile(vector<string> stringList, int i);
	bool Parser::isInteger(string const_value);
	bool Parser::isLetter(string const_value);
	bool Parser::isAlphaNumeric(string str);
	bool Parser::existAlphaNumeric(string str);
	bool Parser::isName(string name);
	bool Parser::isExpression(string expr);
	bool Parser::isTerm(string term);
	bool Parser::isFactor(string factor);
	vector<string> Parser::trimmedString(string str);
	bool Parser::checkNotEmptyLine(string line);
	string Parser::trim(string str);
	vector<string> Parser::removeAllSpaces(vector<string> stringList);
	string Parser::removeLineSpaces(string line);
	int Parser::isStmtLst(vector<string> stringList, int startLine);
	void Parser::checkAllBrackets(vector<string> stringList);
	int Parser::pairedCurlyBracketsPos(vector<string> stringList, int startIndex);
	bool Parser::isPairedRoundBrackets(string str);
	void Parser::error(string errorType);
	~Parser();
};

#endif

