#ifndef DesignExtractor_H
#define DesignExtractor_H

#include <string>
#include <vector>
#include "TNode.h"
#include "AST.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "ModifyTable.h"
#include "UseTable.h"
#include "VarTable.h"
#include "ProcTable.h"

using namespace std;

const string PROCEDURE = "procedure";
const size_t PROC_LEN = PROCEDURE.length();

const string STMTLST = "stmtLst";
const string WHILE = "while";
const size_t WHILE_LEN = WHILE.length();

const string ASSIGN = "assign";
const size_t ASSIGN_LEN = ASSIGN.length();

const string VARIABLE = "variable";
const string CONSTANT = "constant";
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
	vector<string> input;
	vector<AST*> ast;
	int lineNumber;
	int stmtLstNumber;
	int procedureNumber;
	vector<TNode*> currentParent;

	
	void processAST(vector<string> input);
	void processProcedure(string theRestOfLine);
	void processWhile(string theRestOfLine, int lineNumber);
	void processAssign(string leftSide, string rightSide, int lineNumber);
	bool processModTable(ModifyTable* modTable);
	bool processUseTable(UseTable* useTable);
	bool processVarTable(VarTable* varTable);
	bool processProcTable(ProcTable* procTable);

	bool isConst(string var);
	void processRightSideAssign(TNode* curParent, string rightSide, int lineNumber);

	FollowTable* processFollowRelationship(AST* ast);
	ParentTable* processParentRelationship(AST* ast);
	string convertStmtLstNumber(int stmtLstNumber);
	string exprType(string numberText);

public:
	DesignExtractor(vector<string> input);
	~DesignExtractor();
	
	vector<AST*> buildAST(vector<string> input);
	FollowTable* getFollowTable(AST* ast);
	ParentTable* getParentTable(AST* ast);

};

#endif