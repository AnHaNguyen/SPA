#ifndef DesignExtractor_H
#define DesignExtractor_H

#include <string>
#include <vector>
#include "TNode.h"
#include "AST.h"

using namespace std;

const string PROCEDURE = "procedure";
const size_t PROC_LEN = PROCEDURE.length();

const string STMTLST = "stmtLst";
const string WHILE = "while";
const size_t WHILE_LEN = WHILE.length();

const string ASSIGN = "assign";
const size_t ASSIGN_LEN = ASSIGN.length();

const string VARIABLE = "variable";
const string WHITE_SPACE = " ";
const string NO_VALUE = "";
const string OPEN_BRACKET = "{";
const string CLOSE_BRACKET = "}";
const string SEMICOLON = ";";
const string EQUAL = "=";
const string PLUS = "+";
const string PLUS_TEXT = "plus";

class DesignExtractor{
private:
	AST *ast;
	int lineNumber;
	vector<TNode*> currentParent;

	void processProcedure(string theRestOfLine);
	void processWhile(string theRestOfLine, int lineNumber);
	void processAssign(string leftSide, string rightSide, int lineNumber);
	void processRightSideAssign(string rightSide, int lineNumber);

public:
	DesignExtractor();
	DesignExtractor(AST* a);
	void checkStmType(vector<string> input);
};

#endif