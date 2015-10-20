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

