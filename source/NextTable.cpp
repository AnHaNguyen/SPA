#include "NextTable.h"
#include <vector>
#include <string>

NextTable::NextTable(){}
NextTable::~NextTable() {}

bool NextTable::addToTable(string prev, string next) {
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

vector<string> NextTable::getPrev(string next) {
	vector<string> returnList;
	for (unsigned i = 0; i < nextTable.size(); i++) {
		for (unsigned j = 0; j < nextTable.at(i).nextStmts.size(); j++) {
			if (nextTable.at(i).nextStmts.at(j) == next) {
				returnList.push_back(nextTable.at(i).lineNo);
				break;
			}
		}
	}
	return returnList;
}

vector<string> NextTable::getNext(string prev) {
	vector<string> returnList;
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			returnList = nextTable.at(i).nextStmts;
			break;
		}
	}
	return returnList;
}

int NextTable::size() {
	return nextTable.size();
}

bool NextTable::isContained(string prev) {
	for (unsigned i = 0; i < nextTable.size(); i++) {
		if (nextTable.at(i).lineNo == prev) {
			return true;
		}
	}
	return false;
}

bool NextTable::isNext(string prev, string next) {
	if (!isContained(prev)) {
		return false;
	}
	vector<string> nextStmts = getNext(prev);
	for (unsigned i = 0; i < nextStmts.size(); i++) {
		if (nextStmts.at(i) == next) {
			return true;
		}
	}
	return false;
}