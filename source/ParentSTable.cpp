#include "ParentSTable.h"
#include <vector>
#include <string>
#include "ParentTable.h"

ParentSTable::ParentSTable() {}
ParentSTable::~ParentSTable() {  }

bool ParentSTable::addToTable(string lineNo, string child) {
	if (isContained(lineNo) == false) {
		ParentSEntry_t *entry = new ParentSEntry_t(lineNo, child);
		parentSTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < parentSTable.size(); i++) {
		if (parentSTable.at(i).lineNo == lineNo) {
			for (unsigned j = 0; j < parentSTable.at(i).child.size(); j++) {
				if (parentSTable.at(i).child.at(j) == child) {
					return false;
				}
			}
			parentSTable.at(i).child.push_back(child);
			break;
		}
	}
	return true;
}

vector<string> ParentSTable::getChildS(string parent) {
	vector <string> returnList;
	for (unsigned i = 0; i < parentSTable.size(); i++) {
		if (parentSTable.at(i).lineNo == parent) {
			returnList = parentSTable.at(i).child;
			break;
		}
	}
	return returnList;
}

vector<string> ParentSTable::getParentS(string child) {
	vector<string> returnList;
	for (unsigned i = 0; i < parentSTable.size(); i++) {
		for (unsigned j = 0; j < parentSTable.at(i).child.size(); j++) {
			if (parentSTable.at(i).child.at(j) == child) {
				returnList.push_back(parentSTable.at(i).lineNo);
			}
		}
	}
	return returnList;
}

int ParentSTable::size() {
	return parentSTable.size();
}

bool ParentSTable::isContained(string line) {
	for (unsigned i = 0; i < parentSTable.size(); i++) {
		if (parentSTable.at(i).lineNo == line) {
			return true;
		}
	}
	return false;
}

vector<ParentSEntry_t> ParentSTable::getTable() {
	return parentSTable;
}

bool ParentSTable::isParentS(string lineNo, string child) {
	if (!isContained(lineNo)) {
		return false;
	}
	vector<string> childList = getChildS(lineNo);
	for (unsigned i = 0; i < childList.size(); i++) {
		if (childList.at(i) == child) {
			return true;
		}
	}
	return false;
}
