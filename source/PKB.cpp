#include "PKB.h"
#include "AST.h"
#include "FollowTable.h"
#include "ModifyTable.h"
#include "ParentTable.h"
#include "UseTable.h"
#include "DesignExtractor.h"
#include "VarTable.h"
#include "ProcTable.h"

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

AST* PKB::getAST() {
	return ast;
}

void PKB::setAST(AST* ast) {
	PKB::ast = ast;
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
