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
			return false;
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
		if (modifyTable.at(i).modifiedVar == var) {
			returnList.push_back(modifyTable.at(i).lineNo);
		}
	}
	return returnList;
}

string ModifyTable::getModified(int lineNo) {
	string modVar = "";
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).lineNo == lineNo) {
			modVar = modifyTable.at(i).modifiedVar;
			break;
		}
	}
	return modVar;
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
	string modVar = getModified(line);
	return (modVar == var);
}

vector<ModifyEntry_t> ModifyTable::getTable() {
	return modifyTable;
}