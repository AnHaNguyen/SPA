#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include "DesignExtractor.h"

using namespace std;

int main(){
	vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
			"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;" ,
			"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
			"procedureThird{", "z=5;", "v=z;}" };
	for(unsigned i = 0; i < code.size(); i++){
		cout << "out put = " << code.at(i) << endl;
	}

	DesignExtractor object = DesignExtractor(code);

}

DesignExtractor::DesignExtractor(vector<string>parsedInput){
	input = parsedInput;
	
	initialize();
	
	//buildAST(input);
	processModTable();
	processUseTable();
	processProcTable();

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

	lineNumber = 0;
	stmtLstNumber = 0;
	procedureNumber = 1;
}

void DesignExtractor::storeToPKB() {
	//PKB::setASTList(ast);
	PKB::setModifyTable(modTable);
	PKB::setUseTable(useTable);
	PKB::setProcTable(procTable);
	PKB::setVarTable(varTable);
	PKB::setConstTable(constTable);
	//PKB::setParentTable(parentTable);
	//PKB::setFollowTable(followTable);
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

		if (curLine.find(PROCEDURE)) {
			// new procedure, clear all old elements of old procedure
			ASTCurParent.clear();

			string theRestOfLine = curLine.substr(PROC_LEN);
			processProcedure(theRestOfLine);
		}
		else if (curLine.find(WHILE)) {
			string theRestOfLine = curLine.substr(WHILE_LEN);
			lineNumber++;
			processWhile(theRestOfLine, lineNumber);
		}
		else if (curLine.find(EQUAL)) {
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
	string value = theRestOfLine.substr(0, posOfOpenBracket - 1);

	// Put procedure into tree
	// procedure + stmtLst: no line number
	TNode* procNode = new TNode(value, PROCEDURE, 0);

	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	ast.at(procedureNumber)->addToTree(procNode);
	ASTCurParent.push_back(procNode);
	
	// Put statementList into tree
	ast.at(procedureNumber)->addToTree(stmtLstNode);
	ast.at(procedureNumber)->makeChild(procNode, stmtLstNode);
	ASTCurParent.push_back(stmtLstNode);
}

void DesignExtractor::processWhile(string theRestOfLine, int lineNumber){
	// Put whileNode into tree
	TNode* whileNode = new TNode(NO_VALUE, WHILE, lineNumber);
	
	ast.at(procedureNumber)->addToTree(whileNode);
	ast.at(procedureNumber)->makeChild(ASTCurParent.at(ASTCurParent.size() - 1), whileNode);
	ASTCurParent.push_back(whileNode);

	// Put StmtLst of while into tree
	string stmtLstNumText = convertStmtLstNumber(stmtLstNumber);
	TNode* stmtLstNode = new TNode(stmtLstNumText, STMTLST, 0);

	ast.at(procedureNumber)->makeChild(whileNode, stmtLstNode);
	ast.at(procedureNumber)->addToTree(stmtLstNode);
	ASTCurParent.push_back(stmtLstNode);

	// Put control variable into tree
	size_t posOfOpenBracket = theRestOfLine.find(OPEN_BRACKET);
	string controlVar = theRestOfLine.substr(0, posOfOpenBracket);
	
	TNode* controlVarNode = new TNode(controlVar, VARIABLE, lineNumber);
	ast.at(procedureNumber)->makeChild(stmtLstNode, controlVarNode);
	ast.at(procedureNumber)->addToTree(controlVarNode);
}

// No ;} in right side
void DesignExtractor::processAssign(string leftSide, string rightSide, int lineNumber){
	// Put assign node into tree
	TNode* curParent = ASTCurParent.at(ASTCurParent.size() - 1);
	TNode* assignNode = new TNode(NO_VALUE, ASSIGN, lineNumber);

	ast.at(procedureNumber)->addToTree(assignNode);
	ast.at(procedureNumber)->makeChild(curParent, assignNode);
	ASTCurParent.push_back(assignNode);

	// Supposed that left side is always variable
	TNode* leftVar = new TNode(leftSide, VARIABLE, lineNumber);

	ast.at(procedureNumber)->addToTree(leftVar);
	ast.at(procedureNumber)->makeChild(assignNode, leftVar);

	processRightSideAssign(assignNode, rightSide, lineNumber);
	ASTCurParent.pop_back();
}

void DesignExtractor::processRightSideAssign(TNode* curParent, string rightSide, int lineNumber){
	vector<int> plusList;

	// Find the position of each plus sign
	string tempStr = rightSide;
	while (true) {
		int posOfPlus = tempStr.find(PLUS);

		if(posOfPlus != string::npos){
			tempStr.erase(posOfPlus, 1);
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
	ast.at(procedureNumber)->addToTree(leftSubTreeNode);

	for(unsigned i = 0; i < plusList.size() - 1; i++){
		int prevPlus = plusList.at(i);
		int nextPlus = plusList.at(i + 1);

		string rightSubTree = rightSide.substr(prevPlus + 1, nextPlus - prevPlus - 1);	
		string typeOfRight = exprType(rightSubTree);

		TNode* rightSubTreeNode = new TNode(rightSubTree, typeOfRight, lineNumber);
		TNode* plusNode = new TNode(NO_VALUE, PLUS_TEXT, lineNumber);

		ast.at(procedureNumber)->addToTree(rightSubTreeNode);
		ast.at(procedureNumber)->makeChild(plusNode, rightSubTreeNode);
		ast.at(procedureNumber)->makeChild(plusNode, leftSubTreeNode);

		leftSubTreeNode = plusNode;
		ast.at(procedureNumber)->addToTree(leftSubTreeNode);
	}

	// Stick subtree to main tree
	ast.at(procedureNumber)->makeChild(curParent, leftSubTreeNode);
}

//-------------------Create Follow Table---------------------//
FollowTable* DesignExtractor::processFollowRelationship(AST* ast){
	FollowTable* followTable = new FollowTable();

	for(int i = 1; i <= stmtLstNumber; i++){
		string value = convertStmtLstNumber(i);
		// unique StmtLst
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
	ParentTable* parentTable = new ParentTable();

	for(int i = 1; i <= stmtLstNumber; i++) {
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

bool DesignExtractor::processModTable() {	
	for (unsigned i = 0; i < input.size(); i++) {			
		string line = input.at(i);
		unsigned pos = line.find(EQUAL);
		if (pos != string::npos){
			string var = line.substr(0, pos);
			modTable->add(i, var);							//to replace i with actual line no
			varTable->addVar(var);
		}
	}
	return true;
}

bool DesignExtractor::processUseTable() {
	for (unsigned i = 0; i < input.size(); i++) {
		string line = input.at(i);
		string type = line.substr(0, 5);
		if (type == WHILE) {
			string var = line.substr(5, line.size() - 6);
			useTable->add(i, var);			//start after while (5), length = size - start - '{'
											//to replace i with actual line no
			varTable->addVar(var);
		}
		else {								//i stands for line number, not index
			unsigned pos = line.find(EQUAL);
			if (pos != string::npos) {
				line = line.substr(pos + 1, line.size() - pos - 1);			//remove =
				pos = line.find(PLUS);
				while (pos != string::npos) {
					string var = line.substr(0, pos);
					if (!isConst(var)) {
						useTable->add(i, var);				//to replace i with actual line no
						varTable->addVar(var);
					}
					else {
						constTable->addToTable(i, var);		//to replace i with actual line no
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
					useTable->add(i, line);					//to replace i with actual line no
					varTable->addVar(line);
				}
				else {
					constTable->addToTable(i, line);		//to replace i with actual line no
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
FollowTable* DesignExtractor::getFollowTable(AST* ast){
	return processFollowRelationship(ast);
}

ParentTable* DesignExtractor::getParentTable(AST* ast){
	return processParentRelationship(ast);
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