#include "PKB.h"
#include "AST.h"
#include "FollowTable.h"
#include "ModifyTable.h"
#include "ParentTable.h"
#include "UseTable.h"
#include "DesignExtractor.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstTable.h"
#include "ProgLine.h"

FollowTable* PKB::followTable;
UseTable* PKB::useTable;
ParentTable* PKB::parentTable;
ModifyTable* PKB::modifyTable;
vector<AST*> PKB::astList;
VarTable* PKB::varTable;
ProcTable* PKB::procTable;
ConstTable* PKB::constTable;
CallTable* PKB::callTable;
NextTable* PKB::nextTable;
vector<CFG* > PKB::cfgList;
ProgLine* PKB::progLine;
FollowSTable* PKB::followSTable;
CallSTable* PKB::callSTable;
ParentSTable* PKB::parentSTable;

PKB::PKB() {
}

FollowTable* PKB::getFollowTable() {
	return followTable;
}

void PKB::setFollowTable(FollowTable* fTable) {
	followTable = fTable;
}

UseTable* PKB::getUseTable() {
	return useTable;
}

void PKB::setUseTable(UseTable* useTable) {
	PKB::useTable = useTable;
}

ParentTable* PKB::getParentTable() {
	return parentTable;
}

void PKB::setParentTable(ParentTable* parTable) {
	PKB::parentTable = parTable;
}

ModifyTable* PKB::getModifyTable() {
	return modifyTable;
}

void PKB::setModifyTable(ModifyTable* modTable) {
	modifyTable = modTable;
}

vector<AST*> PKB::getASTList() {
	return astList;
}

void PKB::setASTList(vector<AST* > astList) {
	PKB::astList = astList;
}

VarTable* PKB::getVarTable() {
	return varTable;
}

void PKB::setVarTable(VarTable* varTable) {
	PKB::varTable = varTable;
}

ProcTable* PKB::getProcTable() {
	return procTable;
}

void PKB::setProcTable(ProcTable* procTable) {
	PKB::procTable = procTable;
}

ConstTable* PKB::getConstTable() {
	return constTable;
}

void PKB::setConstTable(ConstTable* constTable) {
	PKB::constTable = constTable;
}

CallTable* PKB::getCallTable() {
	return callTable;
}

void PKB::setCallTable(CallTable* callTable) {
	PKB::callTable = callTable;
}

NextTable* PKB::getNextTable() {
	return nextTable;
}

void PKB::setNextTable(NextTable* nextTable) {
	PKB::nextTable = nextTable;
}

vector<CFG*> PKB::getCFGList() {
	return cfgList;
}

void PKB::setCFGList(vector<CFG* > cfgList) {
	PKB::cfgList = cfgList;
}

ProgLine* PKB::getProgLine() {
	return progLine;
}

void PKB::setProgLine(ProgLine* progLine) {
	PKB::progLine = progLine;
}

vector<string> PKB::checkAssign(string pattern, bool contains_) {
	vector<string> returnList;
	DesignExtractor ext = DesignExtractor();
	AST* subtree = ext.buildSubtree(pattern);
	/*for (unsigned i = 0; i < subtree->getTree().size(); i++) {
		returnList.push_back(subtree->getTree().at(i)->getType());
	}*/
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<string> temp = astList.at(i)->findSubtree(subtree, contains_);
		returnList.insert(returnList.end(), temp.begin(), temp.end());
	}
	return returnList;
}

vector<string> PKB::patternIf(string controlVar, bool contains) {
	vector<string> returnList;
	if (contains) {
		if (controlVar == "_") {
			vector<string> result = progLine->getLinesOfType("if");
			for (unsigned i = 0; i < result.size(); i++) {
				returnList.push_back(result.at(i));
			}
		}
		else {
			vector<TNode*> ifNodes;
			for (unsigned i = 0; i < astList.size(); i++) {
				vector<TNode*>ifs = astList.at(i)->getType("if");
				for (unsigned j = 0; j < ifs.size(); j++) {
					ifNodes.push_back(ifs.at(j));
				}
			}
			for (unsigned i = 0; i < ifNodes.size(); i++) {
				if (ifNodes.at(i)->getChildList().at(0)->getValue() == controlVar) {
					returnList.push_back(to_string(ifNodes.at(i)->getLine()));
				}
			}
		}
	}
	else {
		returnList = progLine->getLinesOfType("if");
	}
	return returnList;
}

vector<string> PKB::patternWhile(string controlVar, bool contains) {
	vector<string> returnList;
	if (contains) {
		if (controlVar == "_") {
			vector<string> result = progLine->getLinesOfType("while");
			for (unsigned i = 0; i < result.size(); i++) {
				returnList.push_back(result.at(i));
			}
		}
		else {
			vector<TNode*> whileNodes;
			for (unsigned i = 0; i < astList.size(); i++) {
				vector<TNode*> whiles = astList.at(i)->getType("while");
				for (unsigned j = 0; j < whiles.size(); j++) {
					whileNodes.push_back(whiles.at(j));
				}
			}
			for (unsigned i = 0; i < whileNodes.size(); i++) {
				if (whileNodes.at(i)->getChildList().at(0)->getValue() == controlVar) {
					returnList.push_back(to_string(whileNodes.at(i)->getLine()));
				}
			}
		}
	}
	else {
		returnList = progLine->getLinesOfType("while");
	}
	return returnList;
}

FollowSTable* PKB::getFollowSTable() {
	return followSTable;
}

void PKB::setFollowSTable(FollowSTable* followSTable) {
	PKB::followSTable = followSTable;
}

CallSTable* PKB::getCallSTable() {
	return callSTable;
}

void PKB::setCallSTable(CallSTable* callSTable) {
	PKB::callSTable = callSTable;
}

ParentSTable* PKB::getParentSTable() {
	return parentSTable;
}

void PKB::setParentSTable(ParentSTable* parentSTable) {
	PKB::parentSTable = parentSTable;
}

bool PKB::checkExistMod(string start, string end, string modVar,vector<string> processed) {
	/*vector<string> modifier = modifyTable->getModifier(modVar);
	queue<string> q;
	q.push(start);
	while (!q.empty()) {
		vector<string> nextStmts = nextTable->getNext(q.front());
		q.pop();
		for (int i = 0; i < nextStmts.size(); i++) {
			if (nextStmts.at(i) == end) {
				return true;
			}
			int contain = 0;
			for (int j = 0; j < modifier.size(); j++) {
				if (nextStmts.at(i) == modifier.at(j)) {
					contain = 1;
				}
			}
			if (contain == 0) {
				q.push(nextStmts.at(i));
			}
		}
	}
	return false;*/
	vector<string> nextStmts = nextTable->getNext(start);
	if (nextStmts.size() == 0) {
		return false;
	}
	vector<bool> nextAns;
	for (unsigned i = 0; i < nextStmts.size(); i++) {
		string nextLine = nextStmts.at(i);
		if (nextLine == end) {
			return true;
		}
		else if (find(processed.begin(), processed.end(), nextLine) != processed.end()) {
			nextAns.push_back(false);
		}
		else if (progLine->getType(nextLine) == "assign" && 
						modifyTable->isModified(nextLine,modVar)) {
			nextAns.push_back(false);
		}
		else {
			processed.push_back(nextLine);
			nextAns.push_back(checkExistMod(nextLine, end, modVar, processed));
		}
	}
	bool finalAns = false;
	for (unsigned i = 0; i < nextAns.size(); i++) {
		finalAns = (finalAns || nextAns.at(i));
	}
	return finalAns;
}

vector<pair<string,string>> PKB::patternIf() {
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> ast = astList.at(i)->getTree();
		for (unsigned j = 0; j < ast.size(); j++) {
			if (ast.at(j)->getType() == "if") {
				pair<string, string> pr;
				pr.first = ast.at(j)->getLine();
				pr.second = ast.at(j)->getChildList().at(0)->getValue();
				returnList.push_back(pr);
			}
		}
	}
	return returnList;
}

vector<pair<string, string>> PKB::patternWhile() {
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> ast = astList.at(i)->getTree();
		for (unsigned j = 0; j < ast.size(); j++) {
			if (ast.at(j)->getType() == "while") {
				pair<string, string> pr;
				pr.first = ast.at(j)->getLine();
				pr.second = ast.at(j)->getChildList().at(0)->getValue();
				returnList.push_back(pr);
			}
		}
	}
	return returnList;
}

vector<string> PKB::affect(string n1, string n2) {
	vector<string> returnList;
	vector<string> processed;
	if (n1 != "_" && n2 != "_") {	//affect (1,2)
	//	return true or false;
		if (progLine->getType(n1) != "assign" || progLine->getType(n2) != "assign") {
			return returnList;
		}
		string modVar = modifyTable->getModified(n1).at(0);
		if (!useTable->isUsed(n2, modVar)) {
			returnList.push_back("false");
			return returnList;
		}

		if (checkExistMod(n1, n2, modVar, processed)) {
			returnList.push_back("true");
		}
		else {
			returnList.push_back("false");
		}
		return returnList;
	}
	else if (n1 != "_" && n2 == "_") {	//affect(1,n2)
		if (progLine->getType(n1) != "assign") {
			return returnList;
		}
		string modVar = modifyTable->getModified(n1).at(0);
		
		// for all assign in this procedure
		//		if (isUsed(assign, modVar) && exist...(n1,assign, var)
		//			add assign
		string curProc = progLine->getProcedure(n1);
		vector<string> assignList = progLine->getAssignsOfProc(curProc);
		for (unsigned i = 0; i < assignList.size(); i++) {
			if (useTable->isUsed(assignList.at(i), modVar) &&
				checkExistMod(n1, assignList.at(i), modVar,processed)) {
				returnList.push_back(assignList.at(i));
			}
		}
		return returnList;
		//find all assignments 1->as and no modify of modVar in that path and as uses modVar
	}
	else if (n2 != "_" && n1 == "_") {	//affect(n1,2)
		if (progLine->getType(n2) != "assign") {
			return returnList;
		}
		vector<string> usedVar = useTable->getUsed(n2);
		string curProc = progLine->getProcedure(n2);
		vector<string> assignList = progLine->getAssignsOfProc(curProc);
		for (unsigned i = 0; i < assignList.size(); i++) {
			for (unsigned j = 0; j < usedVar.size(); j++) {
				if (modifyTable->isModified(assignList.at(i),usedVar.at(j)) &&
					checkExistMod(assignList.at(i), n2, usedVar.at(j),processed)) {
					returnList.push_back(assignList.at(i));
				}
			}
		}
		return returnList;
		// for all assign in this procedure
		//		for each var in usedVar
		//			if (isMod(assign, var) && exist...(assign,n2,var)
		//				add assign
		
		//find all assignments as->1 and as mod var and no modify of var in that path
	}
	return returnList;
}

vector<pair<string, vector<string>>> PKB::affect() {
	vector<pair<string, vector<string>>> returnList;
	vector<string> assignList = progLine->getLinesOfType("assign");
	for (unsigned i = 0; i < assignList.size(); i++) {
		vector<string> affectList = affect(assignList.at(i), "_");
		pair<string, vector<string>> pr;
		pr.first = assignList.at(i);
		pr.second = affectList;
		returnList.push_back(pr);
	}
	return returnList;
}