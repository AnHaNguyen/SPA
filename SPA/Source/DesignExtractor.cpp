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

DesignExtractor::DesignExtractor(AST* a){
	ast = a;
	lineNumber = 0;
	stmtLstNumber = 0;
}

//-------------------------AST-------------------------//
AST* DesignExtractor::buildAST(vector<string> input){
	processAST(input);

	return ast;
}

void DesignExtractor::processAST(vector<string> input){
	for(unsigned i = 0; i < input.size(); i++){
		string curLine = input.at(lineNumber);
		cout << "input = " << curLine <<endl;
		size_t curLineLen = curLine.length(); 

		if(curLine.find(PROCEDURE)){
			string theRestOfLine = curLine.substr(PROC_LEN);
			processProcedure(theRestOfLine);
		} else if (curLine.find(WHILE)){
			string theRestOfLine = curLine.substr(WHILE_LEN);
			lineNumber++;
			processWhile(theRestOfLine, lineNumber);
		} else {
			size_t posEqualSign = curLine.find(EQUAL);
			size_t posCloseBracket = curLine.find(CLOSE_BRACKET);
			size_t posSemicolon = curLine.find(SEMICOLON);

			string leftSide = curLine.substr(0, posEqualSign - 1);
			string rightSide = curLine.substr(posEqualSign + 1, posSemicolon - posEqualSign - 1);
			lineNumber++;

			processAssign(leftSide, rightSide, lineNumber);

			// Check the appearance of }
			if(curLine.find(CLOSE_BRACKET) != string::npos){
				currentParent.pop_back();
			}
		}
	}
}

void DesignExtractor::processProcedure(string theRestOfLine){
	// trim bracket {
	size_t posOfOpenBracket = theRestOfLine.find(OPEN_BRACKET);
	string value = theRestOfLine.substr(0, posOfOpenBracket - 1);

	// Put procedure into tree
	TNode* procNode = new TNode(value, PROCEDURE, 0);

	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	ast->addToTree(procNode);
	ast->makeChild(procNode, stmtLstNode);
	
	// Put statementList into tree
	ast->addToTree(stmtLstNode);
	currentParent.push_back(stmtLstNode);
}

void DesignExtractor::processWhile(string theRestOfLine, int lineNumber){
	// Put while into tree
	TNode* whileNode = new TNode(NO_VALUE, WHILE, lineNumber);
	ast->makeChild(currentParent.at(currentParent.size() - 1), whileNode);
	ast->addToTree(whileNode);
	currentParent.push_back(whileNode);

	// Put StmtLst of while into tree
	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	ast->makeChild(whileNode, stmtLstNode);
	ast->addToTree(stmtLstNode);
	currentParent.push_back(stmtLstNode);

	// Put control variable into tree
	size_t posOfOpenBracket = theRestOfLine.find(OPEN_BRACKET);
	string controlVar = theRestOfLine.substr(0, posOfOpenBracket - 1);
	
	TNode* controlVarNode = new TNode(controlVar, VARIABLE, lineNumber);
	ast->makeChild(stmtLstNode, controlVarNode);
	ast->addToTree(controlVarNode);
}

// No } in right side
void DesignExtractor::processAssign(string leftSide, string rightSide, int lineNumber){
	// Put assignment into tree
	TNode* leftVar = new TNode(leftSide, VARIABLE, lineNumber);
	TNode* curParent = currentParent.at(currentParent.size() - 1);
	ast->makeChild(curParent, leftVar);
	ast->addToTree(leftVar);

	processRightSideAssign(curParent, rightSide, lineNumber);
}

void DesignExtractor::processRightSideAssign(TNode* curParent, string rightSide, int lineNumber){
	vector<int> plusList;

	// Find the position of each plus sign
	while(true){
		int posOfPlus = rightSide.find(PLUS);

		if(posOfPlus != string::npos){
			plusList.push_back(posOfPlus);
		} else {
			// Fake plus sign
			plusList.push_back(rightSide.length());
			break;
		}
	}

	// Create subtree of assignment
	string leftVar = rightSide.substr(0, plusList.at(0) - 1);
	string typeOfLeftVar = exprType(leftVar);

	TNode* leftVarNode = new TNode(leftVar, typeOfLeftVar, lineNumber);
	ast->addToTree(leftVarNode);

	for(unsigned i = 0; i < plusList.size() - 1; i++){
		// Havent implemented checking variable/ constants
		string rightVar = rightSide.substr(plusList.at(i) + 1, plusList.at(i+1) - 1);	
		string typeOfRightVar = exprType(rightVar);

		TNode* rightVarNode = new TNode(rightVar, typeOfRightVar, lineNumber);
		TNode* plusNode = new TNode(NO_VALUE, PLUS_TEXT, lineNumber);

		ast->addToTree(rightVarNode);
		ast->makeChild(plusNode, rightVarNode);
		ast->makeChild(plusNode, leftVarNode);

		leftVarNode = plusNode;
		ast->addToTree(leftVarNode);
	}

	// Stick subtree to main tree
	ast->makeChild(curParent, leftVarNode);
}

//-------------------Create Follow Table---------------------//
FollowTable* DesignExtractor::processFollowRelationship(AST* ast){
	FollowTable* followTable;

	for(unsigned i = 1; i <= stmtLstNumber; i++){
		string value = convertStmtLstNumber(i);
		TNode* parent = new TNode(value, STMTLST, 0);

		vector<TNode*> childLst = ast->findChild(parent);
		for(unsigned j = 0; j < childLst.size() - 1; j++){
			TNode* preChild = childLst.at(j);
			TNode* nextChild = childLst.at(j + 1);

			int prev = preChild->getLine();
			int next = nextChild->getLine();

			followTable->addToTable(prev, next);
		}
	}

	return followTable;
}

//--------------------Create Parent Table-------------------//
ParentTable* DesignExtractor::processParentRelationship(AST* ast){
	ParentTable* parentTable;

	for(unsigned i = 1; i <= stmtLstNumber; i++) {
		string value = convertStmtLstNumber(i);
		TNode* stmtLst = new TNode(value, STMTLST, 0);

		TNode* parStmtLst = ast->findParent(stmtLst);
		string typeOfPar = parStmtLst->getType();
		int parLine = parStmtLst->getLine();

		if(typeOfPar.compare(WHILE)){
			vector<TNode*> childStmtLst = ast->findChild(stmtLst);

			for(unsigned j = 0; j < childStmtLst.size(); j++){
				TNode* child = childStmtLst.at(j);
				int childLine = child->getLine();

				parentTable->addToTable(parLine, childLine);
			}
		}
	}

	return parentTable;
}

// Getter
FollowTable* getFollowTable(AST* ast){
	return processFollowRelationship(ast);
}

ParentTable* getParentTable(AST* ast){
	return processParentRelationship(ast);
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