#include "FollowTable.h"
#include <string>
#include <vector>

FollowTable::FollowTable() {
}

FollowTable::~FollowTable() {
}

bool FollowTable::addToTable(string prev, string next) {
	if (isContained(prev)) {
		return false;
	}
	FollowEntry_t *entry = new FollowEntry_t(prev, next);
	followTable.push_back(*entry);
	return true;
}

string FollowTable::getNext(string prev) {
	for (unsigned i = 0; i < followTable.size(); i++) {
		if (followTable.at(i).prev == prev) {
			return followTable.at(i).next;
		}
	}
	return "";
}

string FollowTable::getPrev(string next) {
	for (unsigned i = 0; i < followTable.size(); i++) {
		if (followTable.at(i).next == next) {
			return followTable.at(i).prev;
		}
	}
	return "";
}

int FollowTable::size() {
	return followTable.size();
}

bool FollowTable::isContained(string prev) {
	for (unsigned i = 0; i < followTable.size(); i++) {
		if (followTable.at(i).prev == prev) {
			return true;
		}
	}
	return false;
}

vector<FollowEntry_t> FollowTable::getTable() {
	return followTable;
}

bool FollowTable::isFollows(string prev, string next) {
	if (!isContained(prev)) {
		return false;
	}
	return (next == getNext(prev));
}

void FollowTable::generateFollowSTable() {
	FollowSTable* followSTable = new FollowSTable();
	for (unsigned i = 0; i < followTable.size(); i++) {
		queue<string> lineQ;
		string line = followTable.at(i).prev;
		lineQ.push(line);
		while (!lineQ.empty) {
			string cur = lineQ.front();
			lineQ.pop();
			string next = getNext(cur);
			if (next != ""){
				lineQ.push(next);
				followSTable->addToTable(line, next);
			}
		}
	}
}