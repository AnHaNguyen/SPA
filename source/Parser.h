#ifndef Parser_H
#define Parser_H

#include<string>
#include<stdlib.h>
#include<vector>

using namespace std;
//#pragma once
class Parser
{
private:
	//string errorDescription;
	vector<string> errorTypeList = { "File cannot open ", "Missing brackets ", "Invalid Expression ", "Invalid Statement List ", "Procedure syntax",
		"Invalid naming ", "No valid procedure ",  "Call statement ", "Missing semi-colon",
		"While statement ", "If statement ", "Else statement ", "Assign statement" };
	//vector<string> programLines;

public:
	Parser();
	~Parser();

	vector<string> parseInput(string fName);
	vector<string> result(string fName);

	vector<string> processFile(string fName);
	vector<string> checkSyntax(vector<string> stringList);

	void processStmtType(vector<string> stringList, int i, string word);
	string checkFirstLine(vector<string> stringList);

	string checkProcedure(vector<string> stringList, int startLine);
	string checkCall(vector<string> stringList, int startLine);
	void checkAssign(string str);
	void checkIf(vector<string> stringList, int i);
	void checkElse(vector<string> stringList, int i);
	void checkWhile(vector<string> stringList, int i);

	bool isInteger(string const_value);
	bool isLetter(string const_value);
	bool isAlphaNumeric(string str);
	bool existAlphaNumeric(string str);
	bool isName(string name);
	bool isNameOrConstant(string expr);
	bool isExpression(string expr);

	vector<string> removeAllSpaces(vector<string> stringList);
	string removeLineSpaces(string line);

	int isStmtLst(vector<string> stringList, int startLine);

	int pairedCurlyBracketsPos(vector<string> stringList, int startIndex);
	bool isPairedRoundBrackets(string str);
	bool checkAllBrackets(vector<string> stringList);

	vector<string> trimmedList(vector <string> list);
	bool checkNotEmptyLine(string line);
	string trim(string str);

	bool isEqualVector(vector<string> v1, vector<string> v2);
	void error(string errorType);
};

#endif

