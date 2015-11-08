#include "NextTable.h"
#include <vector>
#include <string>
#include "ProgLine.h"
#include "PKB.h"

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

/*vector<string> NextTable::getPrevSlow(string next) {
	vector<string> returnList;
	for (unsigned i = 0; i < nextTable.size(); i++) {
		for (unsigned j = 0; j < nextTable.at(i).nextStmts.size(); j++) {
			if (nextTable.at(i).nextStmts.at(j) == next) {
				returnList.push_back(nextTable.at(i).lineNo);
				break;
			}
		}
	}
	int line = atoi(next.c_str());
	ProgLine* pl = PKB::getProgLine();
	returnList = pl->getNext(line);
	return returnList;
}*/

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

vector<NextEntry_t> NextTable::getTable() {
	return nextTable;
}

vector<string> NextTable::getNextS(string prev) {
	vector<string> returnList;
	vector<bool> processed;
	int numLines = PKB::getProgLine()->numOfLines();
	for (int i = 0; i < numLines; i++) {
		processed.push_back(false);
	}
	queue<string> q;
	q.push(prev);
	while (!q.empty()) {
		vector<string> nextStmts = getNext(q.front());
		q.pop();
		for (unsigned i = 0; i < nextStmts.size(); i++) {
			if (!processed.at(atoi(nextStmts.at(i).c_str()) -1)) {
				processed.at(atoi(nextStmts.at(i).c_str()) -1) = true;
				returnList.push_back(nextStmts.at(i));
				q.push(nextStmts.at(i));
			}
		}
	}
	return returnList;
}

vector<string> NextTable::getPrevS(string next) {
	vector<string> returnList;
	for (unsigned i = 0; i < nextTable.size(); i++) {
		vector<string> nextStarStmts = getNextS(nextTable.at(i).lineNo);
		for (unsigned j = 0; j < nextStarStmts.size(); j++) {
			if (next == nextStarStmts.at(j)) {
				returnList.push_back(nextTable.at(i).lineNo);
				break;
			}
		}
	}
	return returnList;
}

bool NextTable::isNextS(string prev, string next) {
	vector<string> nextStarStmts = getNextS(prev);
	for (unsigned i = 0; i < nextStarStmts.size(); i++) {
		if (nextStarStmts.at(i) == next) {
			return true;
		}
	}
	return false;
}

vector<NextEntry_t> NextTable::getNextSTable() {
	NextTable* nextSTable = new NextTable();
	for (unsigned i = 0; i < nextTable.size(); i++) {
		string line = nextTable.at(i).lineNo;
		vector<string> nextStmts = getNextS(line);
		nextSTable->addListToTable(line, nextStmts);
	}
	return nextSTable->getTable();
}

void NextTable::addListToTable(string line, vector<string> list) {
	if (list.size() == 0) {
		return;
	}
	unsigned index = nextTable.size();
	NextEntry_t newEntry = NextEntry_t(line, list.at(0));
	nextTable.push_back(newEntry);
	for (unsigned i = 1; i < list.size(); i++) {
		nextTable.at(index).nextStmts.push_back(list.at(i));
	}
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