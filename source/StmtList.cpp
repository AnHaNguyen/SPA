#include <string>
#include <vector>
#include "StmtList.h"

StmtList::StmtList() {
}

StmtList::~StmtList() {
}

bool StmtList::addToList(int lineNo, string type, string procedure) {
	if (stmtList.size() != lineNo - 1) {
		return false;
	}
	stmt_t *entry = new stmt_t(procedure, type);
	stmtList.push_back(*entry);
	return true;
}

string StmtList::getType(int lineNo) {
	if (stmtList.size() < lineNo) {
		return NULL;
	}
	else return stmtList.at(lineNo - 1).type;
}

string StmtList::getProcedure(int lineNo) {
	if (stmtList.size() < lineNo) {
		return NULL;
	}
	else return stmtList.at(lineNo - 1).procedure;
}

int StmtList::numOfLines() {
	return stmtList.size();
}

vector<int> StmtList::getLinesOfProc(string procedure) {
	vector<int> returnList;
	for (unsigned i = 1; i <= stmtList.size(); i++) {
		if (getProcedure(i) == procedure) {
			returnList.push_back(i);
			int j = i + 1;
			while (getProcedure(j) == procedure) {
				returnList.push_back(j);
				j++;
			}
			break;
		}
	}
	return returnList;
}

vector<int> StmtList::getLinesOfType(string type){
	vector<int> returnList;
	for (unsigned i = 1; i <= stmtList.size(); i++) {
		if (getType(i) == type) {
			returnList.push_back(i);
		}
	}
	return returnList;
}
