#ifndef Parser_H
#define Parser_H

#include<string>
#include<stdlib.h>
#include<vector>

using namespace std;
//#pragma once
class Parser
{
public:
	Parser();
	~Parser();

	vector<string> parseInput(string fName);
	vector<string> result(string fName);
	bool isInteger(string const_value);

	vector<string> processFile(string fName);
	vector<string> checkSyntax(vector<string> stringList);
	void processStmtType(vector<string> stringList, int i, string word);
	void checkFirstLine(vector<string> stringList);
	void checkProcedure(vector<string> stringList, int startLine);
	void checkCall(string str);
	void checkAssign(string str);
	void checkIf(vector<string> stringList, int i);
	void checkElse(vector<string> stringList, int i);
	void checkWhile(vector<string> stringList, int i);
	bool isLetter(string const_value);
	bool isAlphaNumeric(string str);
	bool existAlphaNumeric(string str);
	bool isName(string name);
	bool isExpression(string expr);
	vector<string> trimmedString(string str);
	bool checkNotEmptyLine(string line);
	string trim(string str);
	vector<string> removeAllSpaces(vector<string> stringList);
	string removeLineSpaces(string line);
	int isStmtLst(vector<string> stringList, int startLine);
	void checkAllBrackets(vector<string> stringList);
	int pairedCurlyBracketsPos(vector<string> stringList, int startIndex);
	bool isPairedRoundBrackets(string str);
	void error(string errorType);
};

#endif

