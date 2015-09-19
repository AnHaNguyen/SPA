#include "NextTable.h"
#include <vector>
#include <string>

NextTable::NextTable(){}
NextTable::~NextTable() {}

bool NextTable::addToTable(int prev, int next) {
	if (!isContained(prev)) {
		NextEntry_t *entry = new NextEntry_t(prev, next);
		nextTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			for (unsigned j = 0; j < nextTable.at(i).nextStmts.size(); j++) {
				if (nextTable.at(i).nextStmts.at(j) == next) {
					return false;
				}
			}
			nextTable.at(i).nextStmts.push_back(next);
			break;
		}
	}
	return true;
}

int NextTable::getPrev(int next) {
	for (unsigned i = 0; i < nextTable.size(); i++) {
		for (unsigned j = 0; j < nextTable.at(i).nextStmts.size(); j++) {
			if (nextTable.at(i).nextStmts.at(j) == next) {
				return nextTable.at(i).lineNo;
			}
		}
	}
	return -1;
}

vector<int> NextTable::getNext(int prev) {
	vector<int> returnList;
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			returnList = nextTable.at(i).nextStmts;
		}
	}
	return returnList;
}

int NextTable::size() {
	return nextTable.size();
}

bool NextTable::isContained(int prev) {
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			return true;
		}
	}
	return false;
}

bool NextTable::isNext(int prev, int next) {
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			for (unsigned j = 0; j < nextTable.at(i).nextStmts.size(); j++) {
				if (nextTable.at(i).nextStmts.at(j) == next) {
					return true;
				}
			}
		}
	}
	return false;
}