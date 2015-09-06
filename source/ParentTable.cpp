#include "ParentTable.h"
#include <vector>
#include <string>

ParentTable::ParentTable() {
}

ParentTable::~ParentTable() {
}

bool ParentTable::addToTable(int lineNo, int child) {
	if (isContained(lineNo) == false) {
		ParentEntry_t *entry = new ParentEntry_t(lineNo, child);
		parentTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < parentTable.size(); i++) {
		if (parentTable.at(i).lineNo == lineNo) {
			for (unsigned j = 0; j < parentTable.at(i).child.size(); j++) {
				if (parentTable.at(i).child.at(j) == child) {
					return false;
				}
			}
			parentTable.at(i).child.push_back(child);
			break;
		}
	}
	return true;
}

vector <int> ParentTable::getChild(int parent) {
	vector <int> returnList;
	for (unsigned i = 0; i < parentTable.size(); i++) {
		if (parentTable.at(i).lineNo == parent) {
			returnList = parentTable.at(i).child;
			break;
		}
	}
	return returnList;
}

int ParentTable::getParent(int child) {
	for (unsigned i = 0; i < parentTable.size(); i++) {
		for (unsigned j = 0; j < parentTable.at(i).child.size(); j++) {
			if (parentTable.at(i).child.at(j) == child) {
				return parentTable.at(i).lineNo;
			}
		}
	}
	return -1;
}

int ParentTable::size() {
	return parentTable.size();
}

bool ParentTable::isContained(int line) {
	for (unsigned i = 0; i < parentTable.size(); i++) {
		if (parentTable.at(i).lineNo == line) {
			return true;
		}
	}
	return false;
}

vector<ParentEntry_t> ParentTable::getTable() {
	return parentTable;
}
