#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include "DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(){}

DesignExtractor::DesignExtractor(vector<string>parsedInput){
	input = parsedInput;
	
	initialize();
	
	buildAST(input);
	processModTable();
	processUseTable();
	processProcTable();

	for (unsigned i = 0; i < ast.size(); i++) {
		processFollowRelationship(ast.at(i));
		processParentRelationship(ast.at(i));
	}

	storeToPKB();
}

DesignExtractor::~DesignExtractor(){}

void DesignExtractor::initialize() {
	followTable = new FollowTable();
	parentTable = new ParentTable();
	modTable = new ModifyTable();
	useTable = new UseTable();
	varTable = new VarTable();
	procTable = new ProcTable();
	constTable = new ConstTable();
	ast = vector<AST* >();
	ASTCurParent = vector<TNode*>();

	lineNumber = 0;
	stmtLstNumber = 0;
	procedureNumber = -1;
}

void DesignExtractor::storeToPKB() {
	PKB::setASTList(ast);
	PKB::setModifyTable(modTable);
	PKB::setUseTable(useTable);
	PKB::setProcTable(procTable);
	PKB::setVarTable(varTable);
	PKB::setConstTable(constTable);
	PKB::setParentTable(parentTable);
	PKB::setFollowTable(followTable);
}

//-------------------------AST-------------------------//
vector<AST*> DesignExtractor::buildAST(vector<string> input){
	processAST(input);
	return ast;
}

void DesignExtractor::processAST(vector<string> input){
	for (unsigned i = 0; i < input.size(); i++) {
		// Current line in input, not lineNumber marked for stmt
		string curLine = input.at(i);
		size_t curLineLen = curLine.length();

		if (curLine.find(PROCEDURE) != string::npos) {
			// new procedure, clear all old elements of old procedure
			ASTCurParent.clear();

			string theRestOfLine = curLine.substr(PROC_LEN);
			ast.push_back(new AST());
			procedureNumber++;
			processProcedure(theRestOfLine);
		}
		else if (curLine.find(WHILE) != string::npos) {
			string theRestOfLine = curLine.substr(WHILE_LEN);
			lineNumber++;
			processWhile(theRestOfLine, lineNumber);
		}
		else if (curLine.find(EQUAL) != string::npos) {
			size_t posEqualSign = curLine.find(EQUAL);
			size_t posSemicolon = curLine.find(SEMICOLON);

			string leftSide = curLine.substr(0, posEqualSign);
			string rightSide = curLine.substr(posEqualSign + 1, posSemicolon - posEqualSign - 1);
			lineNumber++;

			processAssign(leftSide, rightSide, lineNumber);
		}
		else {
			return;
		}

		// Check the appearance of }
		string tempLine = curLine;
		while (true) {
			size_t posCloseBracket = tempLine.find(CLOSE_BRACKET);
			
			if (posCloseBracket != string::npos) {
				tempLine.erase(posCloseBracket, 1);
				while (true) {
					TNode* removedParent = ASTCurParent.at(ASTCurParent.size() - 1);
					string typeRemovedParent = removedParent->getType();
					ASTCurParent.pop_back();
					
					if (typeRemovedParent != STMTLST)
						break;
				}
			}
			else {
				break;
			}
		}
	}
}

void DesignExtractor::processProcedure(string theRestOfLine){
	// remove bracket {
	size_t posOfOpenBracket = theRestOfLine.find(OPEN_BRACKET);
	string value = theRestOfLine.substr(0, posOfOpenBracket);

	// Put procedure into tree
	// procedure + stmtLst: no line number
	TNode* procNode = new TNode(value, PROCEDURE, 0);

	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	
	ast.at(procedureNumber)->addToTree(procNode);
	ASTCurParent.push_back(procNode);
	
	// Put statementList into tree
	ast.at(procedureNumber)->addToTree(stmtLstNode);
	procNode->setChild(stmtLstNode);
	stmtLstNode->setParent(procNode);
	//ast.at(procedureNumber)->makeChild(procNode, stmtLstNode);
	ASTCurParent.push_back(stmtLstNode);
}

void DesignExtractor::processWhile(string theRestOfLine, int lineNumber){
	// Put whileNode into tree
	TNode* whileNode = new TNode(NO_VALUE, WHILE, lineNumber);
	
	ast.at(procedureNumber)->addToTree(whileNode);
	ASTCurParent.at(ASTCurParent.size() - 1)->setChild(whileNode);
	whileNode->setParent(ASTCurParent.at(ASTCurParent.size() - 1));
	//ast.at(procedureNumber)->makeChild(ASTCurParent.at(ASTCurParent.size() - 1), whileNode);
	ASTCurParent.push_back(whileNode);

	// Put StmtLst of while into tree
	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	// Put control variable into tree
	size_t posOfOpenBracket = theRestOfLine.find(OPEN_BRACKET);
	string controlVar = theRestOfLine.substr(0, posOfOpenBracket);

	TNode* controlVarNode = new TNode(controlVar, VARIABLE, lineNumber);
	//ast.at(procedureNumber)->makeChild(whileNode, controlVarNode);
	whileNode->setChild(controlVarNode);
	controlVarNode->setParent(whileNode);
	ast.at(procedureNumber)->addToTree(controlVarNode);

	//ast.at(procedureNumber)->makeChild(whileNode, stmtLstNode);
	whileNode->setChild(stmtLstNode);
	stmtLstNode->setParent(whileNode);
	ast.at(procedureNumber)->addToTree(stmtLstNode);
	ASTCurParent.push_back(stmtLstNode);

	
}

// No ;} in right side
void DesignExtractor::processAssign(string leftSide, string rightSide, int lineNumber){
	// Put assign node into tree
	TNode* curParent = ASTCurParent.at(ASTCurParent.size() - 1);
	TNode* assignNode = new TNode(NO_VALUE, ASSIGN, lineNumber);

	ast.at(procedureNumber)->addToTree(assignNode);
	curParent->setChild(assignNode);
	assignNode->setParent(curParent);
	//ast.at(procedureNumber)->makeChild(curParent, assignNode);
	ASTCurParent.push_back(assignNode);

	// Supposed that left side is always variable
	TNode* leftVar = new TNode(leftSide, VARIABLE, lineNumber);

	ast.at(procedureNumber)->addToTree(leftVar);
	assignNode->setChild(leftVar);
	leftVar->setParent(assignNode);
	//ast.at(procedureNumber)->makeChild(assignNode, leftVar);

	processRightSideAssign(ast.at(procedureNumber), assignNode, rightSide, lineNumber);
	ASTCurParent.pop_back();
}

void DesignExtractor::processRightSideAssign(AST* subAST, TNode* curParent, string rightSide, int lineNumber){
	vector<int> plusList;

	// Find the position of each plus sign
	string tempStr = rightSide;
	while (true) {
		int posOfPlus = tempStr.find(PLUS);

		if(posOfPlus != string::npos){
			// replace the first occurence + by @
			tempStr.replace(posOfPlus, 1, "@");
			plusList.push_back(posOfPlus);
		} else {
			// Fake plus sign
			plusList.push_back(tempStr.length());
			break;
		}
	}

	// Create subtree of assignment
	string leftSubTree = rightSide.substr(0, plusList.at(0));
	string typeOfLeft = exprType(leftSubTree);

	TNode* leftSubTreeNode = new TNode(leftSubTree, typeOfLeft, lineNumber);
	subAST->addToTree(leftSubTreeNode);

	for(unsigned i = 0; i < plusList.size() - 1; i++){
		int prevPlus = plusList.at(i);
		int nextPlus = plusList.at(i + 1);

		string rightSubTree = rightSide.substr(prevPlus + 1, nextPlus - prevPlus -1);	
		string typeOfRight = exprType(rightSubTree);

		TNode* rightSubTreeNode = new TNode(rightSubTree, typeOfRight, lineNumber);
		TNode* plusNode = new TNode(NO_VALUE, PLUS_TEXT, lineNumber);

		subAST->addToTree(rightSubTreeNode);
		
		plusNode->setChild(leftSubTreeNode);
		leftSubTreeNode->setParent(plusNode);
		plusNode->setChild(rightSubTreeNode);
		rightSubTreeNode->setParent(plusNode);
		
		leftSubTreeNode = plusNode;
		subAST->addToTree(leftSubTreeNode);
	}

	// Stick subtree to main tree
	if (curParent->getType() != "") {
		curParent->setChild(leftSubTreeNode);
		leftSubTreeNode->setParent(curParent);
	}
}

//-------------------Create Follow Table---------------------//
void DesignExtractor::processFollowRelationship(AST* ast) {
	for (unsigned i = 0; i < ast->getTree().size(); i++) {
		string value = convertStmtLstNumber(i - 1);

		// check unique StmtLst
		TNode* parent = ast->getTree().at(i);
		if (parent->getType() == STMTLST) {
			vector<TNode*> childLst = parent->getChildList();

			for (unsigned j = 0; j < childLst.size() - 1; j++) {
				TNode* preChild = childLst.at(j);
				TNode* nextChild = childLst.at(j + 1);

				int prev = preChild->getLine();
				int next = nextChild->getLine();

				followTable->addToTable(prev, next);
			}
		}
	}
}

//--------------------Create Parent Table-----------------------//
// stmtLst's parent is the parent of stmtLst's child
void DesignExtractor::processParentRelationship(AST* ast){
	for (unsigned i = 0; i < ast->getTree().size(); i++) {
		string value = convertStmtLstNumber(i);
		TNode* middleNode = ast->getTree().at(i);

		TNode* parStmtLst = middleNode->getParent();
		string typeOfPar = parStmtLst->getType();
		int parLine = parStmtLst->getLine();

		if(typeOfPar == WHILE) {
			vector<TNode*> childStmtLst = middleNode->getChildList();

			for(unsigned j = 0; j < childStmtLst.size(); j++){
				TNode* child = childStmtLst.at(j);
				int childLine = child->getLine();

				parentTable->addToTable(parLine, childLine);
			}
		}
	}
}

bool DesignExtractor::processModTable() {	
	int lineNumber = 0;
	for (unsigned i = 0; i < input.size(); i++) {			
		string line = input.at(i);
		lineNumber = getRealLineNumber(lineNumber, line);
		unsigned pos = line.find(EQUAL);

		if (pos != string::npos){
			string var = line.substr(0, pos);
			modTable->add(lineNumber, var);							
			varTable->addVar(var);
		}
	}
	return true;
}

bool DesignExtractor::processUseTable() {
	int lineNumber = 0;

	for (unsigned i = 0; i < input.size(); i++) {
		string line = input.at(i);
		string type = line.substr(0, 5);
		if (type == WHILE) {
			string var = line.substr(5, line.size() - 6);
			lineNumber = getRealLineNumber(lineNumber, line);

			useTable->add(lineNumber, var);			// start after while (5), length = size - start - '{'
			varTable->addVar(var);
		}
		else {								
			unsigned pos = line.find(EQUAL);
			lineNumber = getRealLineNumber(lineNumber, line);

			if (pos != string::npos) {
				line = line.substr(pos + 1, line.size() - pos - 1);			//remove =
				pos = line.find(PLUS);
				while (pos != string::npos) {
					string var = line.substr(0, pos);

					if (!isConst(var)) {	
						useTable->add(lineNumber, var);				
						varTable->addVar(var);
					}
					else {
						constTable->addToTable(lineNumber, var);		
					}
					line = line.substr(pos + 1, line.size() - pos - 1);		//remove +
					pos = line.find(PLUS);
				}
				if (line.find(CLOSE_BRACKET) != string::npos) {
					line = line.substr(0, line.size() - 2);			//remove '}' and ';'
				}
				else {
					line = line.substr(0, line.size() - 1);		//remove ';'
				}
				if (!isConst(line)) {
					useTable->add(lineNumber, line);					
					varTable->addVar(line);
				}
				else {
					constTable->addToTable(lineNumber, line);
				}
			}
		}
	}
	return true;
}

bool DesignExtractor::isConst(string var){
	return (isdigit(var[0]));
}


bool DesignExtractor::processProcTable() {
	for (unsigned i = 0; i < input.size(); i++) {
		string proc = PROCEDURE;
		unsigned pos = input.at(i).find(proc);
		if (pos != string::npos) {
			procTable->addProc(input.at(i).substr(9, input.at(i).size() - 10));	//start after "Proc.." minus '{'
		}
	}	
	return true;
}


// Getter
FollowTable* DesignExtractor::getFollowTable(){
	return followTable;
}

ParentTable* DesignExtractor::getParentTable(){
	return parentTable;
}

ModifyTable* DesignExtractor::getModTable() {
	return modTable;
}

UseTable* DesignExtractor::getUseTable() {
	return useTable;
}

VarTable* DesignExtractor::getVarTable() {
	return varTable;
}

ConstTable* DesignExtractor::getConstTable() {
	return constTable;
}

ProcTable* DesignExtractor::getProcTable() {
	return procTable;
}

vector<AST*> DesignExtractor::getASTList() {
	return ast;
}

AST* DesignExtractor::buildSubtree(string pattern) {
	AST* subAST = new AST();
	processRightSideAssign(subAST, new TNode(), pattern, 0);

	return subAST;
}

// Smaller modules
string DesignExtractor::convertStmtLstNumber(int stmtLstNumber){
	stmtLstNumber++;
	ostringstream osstream;
	osstream << stmtLstNumber;
	string stmtLstNumText = osstream.str();

	return stmtLstNumText;
}

// Check the string is number or variable
// Return the type of VARIABLE or CONSTANT.
string DesignExtractor::exprType(string numText){
	char firstDigit = numText.at(0);

	if(isdigit(firstDigit)){
		return CONSTANT;
	} else {
		return VARIABLE;
	}
}

int DesignExtractor::getRealLineNumber(int lineNumber, string line) {
	// Keep track lineNumber from input
	unsigned posOfProc = line.find(PROCEDURE);
	if (posOfProc == string::npos) {
		lineNumber++;
	}

	return lineNumber;
}