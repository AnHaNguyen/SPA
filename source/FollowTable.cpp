#include "FollowTable.h"
#include <string>
#include <vector>

FollowTable::FollowTable() {
}

FollowTable::~FollowTable() {
}

bool FollowTable::addToTable(int prev, int next) {
	if (isContained(prev)) {
		return false;
	}
	FollowEntry_t *entry = new FollowEntry_t(prev, next);
	followTable.push_back(*entry);
	return true;
}

int FollowTable::getNext(int prev) {
	for (unsigned i = 0; i < followTable.size(); i++) {
		if (followTable.at(i).prev == prev) {
			return followTable.at(i).next;
		}
	}
	return -1;
}

int FollowTable::getPrev(int next) {
	for (unsigned i = 0; i < followTable.size(); i++) {
		if (followTable.at(i).next == next) {
			return followTable.at(i).prev;
		}
	}
	return -1;
}

int FollowTable::size() {
	return followTable.size();
}

bool FollowTable::isContained(int prev) {
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