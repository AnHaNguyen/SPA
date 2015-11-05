#include "FollowSTable.h"
#include <vector>
#include <string>
#include "ProgLine.h"
#include "PKB.h"

FollowSTable::FollowSTable() {
}

FollowSTable::~FollowSTable() {
}

bool FollowSTable::addToTable(string lineNo, string next) {
	if (isContained(lineNo) == false) {
		FollowSEntry_t *entry = new FollowSEntry_t(lineNo, next);
		followSTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < followSTable.size(); i++) {
		if (followSTable.at(i).lineNo == lineNo) {
			for (unsigned j = 0; j < followSTable.at(i).followStmts.size(); j++) {
				if (followSTable.at(i).followStmts.at(j) == next) {
					return false;
				}
			}
			followSTable.at(i).followStmts.push_back(next);
			break;
		}
	}
	return true;
}

vector <string> FollowSTable::getNextS(string prev) {
	vector <string> returnList;
	for (unsigned i = 0; i < followSTable.size(); i++) {
		if (followSTable.at(i).lineNo == prev) {
			returnList = followSTable.at(i).followStmts;
			break;
		}
	}
	return returnList;
}

vector<string> FollowSTable::getPrevSSlow(string next) {
	vector<string> returnList;
	/*for (unsigned i = 0; i < followSTable.size(); i++) {
		for (unsigned j = 0; j < followSTable.at(i).followStmts.size(); j++) {
			if (followSTable.at(i).followStmts.at(j) == next) {
				returnList.push_back(followSTable.at(i).lineNo);
			}
		}
	}*/
	int line = atoi(next.c_str());
	ProgLine* pl = PKB::getProgLine();
	returnList = pl->getFollowS(line);
	return returnList;
}

int FollowSTable::size() {
	return followSTable.size();
}

bool FollowSTable::isContained(string line) {
	for (unsigned i = 0; i < followSTable.size(); i++) {
		if (followSTable.at(i).lineNo == line) {
			return true;
		}
	}
	return false;
}

vector<FollowSEntry_t> FollowSTable::getTable() {
	return followSTable;
}

bool FollowSTable::isFollowS(string lineNo, string next) {
	if (!isContained(lineNo)) {
		return false;
	}
	vector<string> followList = getNextS(lineNo);
	for (unsigned i = 0; i < followList.size(); i++) {
		if (followList.at(i) == next) {
			return true;
		}
	}
	return false;
}

vector<string> FollowSTable::getPrevS(string next) {
	vector<string> returnList;
	for (unsigned i = 0; i < followSTable.size(); i++) {
		for (unsigned j = 0; j < followSTable.at(i).followStmts.size(); j++) {
			if (followSTable.at(i).followStmts.at(j) == next) {
				returnList.push_back(followSTable.at(i).lineNo);
			}
		}
	}
	return returnList;
}
