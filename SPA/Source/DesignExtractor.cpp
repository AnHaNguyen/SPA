#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "DesignExtractor.h"

using namespace std;

int main(){
	vector<string> input;
	input.at(0) = "procedure First";

	DesignExtractor obj;
	obj.checkStmType(input);
}

DesignExtractor::DesignExtractor(){}

DesignExtractor::DesignExtractor(AST* a){
	ast = a;
	lineNumber = 0;
	currentParent.at(0) = NULL;
}

void DesignExtractor::checkStmType(vector<string> input){
	for(unsigned i = 0; i < input.size(); i++){
		string curLine = input.at(lineNumber);
		size_t curLineLen = curLine.length(); 

		if(curLine.find(PROCEDURE)){
			string theRestOfLine = curLine.substr(PROC_LEN);
			processProcedure(theRestOfLine);
		}else if(curLine.find(WHILE)){
			string theRestOfLine = curLine.substr(WHILE_LEN);
			lineNumber++;
			processWhile(theRestOfLine, lineNumber);
		}else {
			size_t posEqualSign = curLine.find(EQUAL);
			size_t posCloseBracket = curLine.find(CLOSE_BRACKET);

			string leftSide = curLine.substr(0, posEqualSign - 1);
			string rightSide = curLine.substr(posEqualSign + 1, posCloseBracket);
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
	TNode* stmtLst = new TNode(NO_VALUE, STMTLST, 0);
	ast->addToTree(procNode);
	ast->makeChild(procNode, stmtLst);
	
	// Put statementList into tree
	ast->addToTree(stmtLst);
	currentParent.push_back(stmtLst);
}

void DesignExtractor::processWhile(string theRestOfLine, int lineNumber){
	// Put while into tree
	TNode* whileNode = new TNode(NO_VALUE, WHILE, lineNumber);
	ast->makeChild(currentParent.at(currentParent.size() - 1), whileNode);
	ast->addToTree(whileNode);
	currentParent.push_back(whileNode);

	// Put StmtLst of while into tree
	TNode* stmtLstNode = new TNode(NO_VALUE, STMTLST, 0);
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
	TNode* leftVarNode = new TNode(leftVar, VARIABLE, lineNumber);
	ast->addToTree(leftVarNode);

	for(unsigned i = 0; i < plusList.size() - 1; i++){
		string rightVar = rightSide.substr(plusList.at(i) + 1, plusList.at(i+1) - 1);	
		TNode* rightVarNode = new TNode(rightVar, VARIABLE, lineNumber);
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