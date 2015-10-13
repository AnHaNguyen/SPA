#ifndef DesignExtractor_H
#define DesignExtractor_H

#include <string>
#include <vector>
#include <queue>
#include "TNode.h"
#include "AST.h"
#include "CFG.h"
#include "CallTable.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "ModifyTable.h"
#include "UseTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstTable.h"
#include "ProgLine.h"
#include "PKB.h"
#include "NextTable.h"

using namespace std;

const string PROCEDURE = "procedure";
const size_t PROC_LEN = PROCEDURE.length();

const string STMTLST = "stmtLst";
const string WHILE = "while";
const size_t WHILE_LEN = WHILE.length();

const string IF = "if";
const size_t IF_LEN = IF.length();
const string THEN = "then";
const string ELSE = "else";

const string ASSIGN = "assign";
const size_t ASSIGN_LEN = ASSIGN.length();

const string CALL = "call";
const size_t CALL_LEN = CALL.length();

const string VARIABLE = "variable";
const string CONSTANT = "constant";
const string WHITE_SPACE = " ";
const string NO_VALUE = "";

const string OPEN_BRACKET = "{";
const string CLOSE_BRACKET = "}";
const string ROUND_OPEN_BRACKET = "(";
const string FAKE_ROUND_BRACKET = "@";
const string ROUND_CLOSE_BRACKET = ")";
const string SEMICOLON = ";";
const string EQUAL = "=";

const string PLUS = "+";
const string PLUS_TEXT = "plus";
const string MINUS = "-";
const string MINUS_TEXT = "minus";
const string TIMES = "*";
const string TIMES_TEXT = "times";

const string USE_VAR = "Use";
const string MODIFY_VAR = "Modify";

class DesignExtractor {
private:
	string curProcName, rightSideText;
	TNode* curNodeInRightSideAss;
	vector<string> input;
	vector<AST*> ast;
	vector<CFG*> cfg;
	queue<TNode*> seperateNodeBracket;

	CallTable* callTable;
	FollowTable* followTable;
	ParentTable* parentTable;
	UseTable* useTable;
	ModifyTable* modTable;
	VarTable* varTable;
	ProcTable* procTable;
	ConstTable* constTable;
	ProgLine* progLine;
	NextTable* nextTable;

	int lineNumber;
	int stmtLstNumber;
	int procedureNumber;
	// all var in right side of assignment
	vector<string> useVarList;
	vector<TNode*> ASTCurParent;

	void initialize();			//initialize tables
	void storeToPKB();			//store tables after process to pkb
	void processAST(vector<string> input);
	void processProcedure(string theRestOfLine);
	void processWhile(string theRestOfLine, int lineNumber);
	// Process assignment
	void processAssign(string leftSide, int lineNumber);
	void processRightSideAssign(AST* curProcAst, TNode* curPar, int lineNumber);
	TNode* processInsideBracket(AST* curProcAst, string subStr, int lineNumber);
	void addToVarConstTable(string var, int lineNum);

	void processIfThen(string controlVar, int lineNumber);
	void processElse();
	void processCallAST(string value, int lineNumber);

	void processCallTable(AST* ast);
	void processModUseTable();
	// mod/usetable with call
	void processModUseTableWCall();
	void updateModUseTableWCall(string caller, string callee);
	// for usetable
	void processRightBranchAST(TNode* rightBranch, string lineNumStr);
	void addVarToContainer(string container, vector<string> varList, string type);

	void processProcTable();
	void processFollowTable(AST* ast);
	void processParentTable(AST* ast);

	bool isConst(string var);

	string convertNumToStr(int stmtLstNumber);
	string exprType(string numberText);
	int getRealLineNumber(int lineNumber, string input);
	
	string findLast(string line);
	string nearestNext(string line);
	bool processNextTable();

public:
	DesignExtractor();
	DesignExtractor(vector<string> input);
	~DesignExtractor();

	vector<AST*> buildAST(vector<string> input);
	CallTable* getCallTable();
	FollowTable* getFollowTable();
	ParentTable* getParentTable();
	ModifyTable* getModTable();
	UseTable* getUseTable();
	VarTable* getVarTable();
	ProcTable* getProcTable();
	ConstTable* getConstTable();
	NextTable* getNextTable();

	ProgLine* getProgLine();
	vector<AST*> getASTList();
	AST* buildSubtree(string pattern);

	string getRightSideText();
};

#endif