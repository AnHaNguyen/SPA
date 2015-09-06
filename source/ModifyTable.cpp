#include <vector>
#include <string>
#include "ModifyTable.h"
#include "Utility.h"

ModifyTable::ModifyTable() {
}

ModifyTable::~ModifyTable() {
}

bool ModifyTable::add(int lineNo, string var) {
	if (isContained(lineNo) == false) {
		ModifyEntry_t *entry = new ModifyEntry_t(lineNo, var);
		modifyTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).lineNo == lineNo) {
			for (unsigned j = 0; j < modifyTable.at(i).modifiedVar.size(); j++) {
				if (modifyTable.at(i).modifiedVar.at(j) == var) {
					return false;
				}
			}
			modifyTable.at(i).modifiedVar.push_back(var);
			break;
		}
	}
	return true;
}

int ModifyTable::size() {
	return modifyTable.size();
}

vector<int> ModifyTable::getModifier(string var) {
	vector<int> returnList;
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		for (unsigned j = 0; j < modifyTable.at(i).modifiedVar.size(); j++) {
			if (modifyTable.at(i).modifiedVar.at(j) == var) {
				returnList.push_back(modifyTable.at(i).lineNo);
			}
		}
	}
	return returnList;
}

vector<string> ModifyTable::getModified(int lineNo) {
	vector<string> returnList;
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).lineNo == lineNo) {
			returnList = modifyTable.at(i).modifiedVar;
			break;
		}
	}
	return returnList;
}

bool ModifyTable::isContained(int lineNo) {
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).lineNo == lineNo) {
			return true;
		}
	}
	return false;
}

bool ModifyTable::isModified(int line, string var) {
	if (isContained(line) == false) {
		return false;
	}
	vector <string> modifiedList = getModified(line);
	for (unsigned i = 0; i < modifiedList.size(); i++) {
		if (modifiedList.at(i) == var) {
			return true;
		}
	}
	return false;
}