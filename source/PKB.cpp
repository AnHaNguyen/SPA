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

vector<int> PKB::checkAssign(string pattern, bool contains_) {
	vector<int> returnList;
	DesignExtractor ext = DesignExtractor();
	AST* subtree = ext.buildSubtree(pattern);
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<int> temp = astList.at(i)->findSubtree(subtree, contains_);
		returnList.insert(returnList.end(), temp.begin(), temp.end());
	}
	return returnList;
}
