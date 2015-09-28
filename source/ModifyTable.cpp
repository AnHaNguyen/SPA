#include <vector>
#include <string>
#include "ModifyTable.h"
#include "Utility.h"

ModifyTable::ModifyTable() {
}

ModifyTable::~ModifyTable() {
}


bool ModifyTable::addToTable(string modifier, string var) {
	if (isContained(modifier) == false) {
		ModifyEntry_t *entry = new ModifyEntry_t(modifier, var);
		modifyTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).modifier == modifier) {
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

vector<string> ModifyTable::getModifier(string var) {
	vector<string> returnList;
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		for (unsigned j = 0; j < modifyTable.at(i).modifiedVar.size(); j++){
			if (modifyTable.at(i).modifiedVar.at(j) == var) {
				returnList.push_back(modifyTable.at(i).modifier);
			}
		}
	}
	return returnList;
}

vector<string> ModifyTable::getModified(string modifier) {
	vector<string> returnList;
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).modifier == modifier) {
			returnList = modifyTable.at(i).modifiedVar;
			break;
		}
	}
	return returnList;
}

bool ModifyTable::isContained(string lineNo) {
	for (unsigned i = 0; i < modifyTable.size(); i++) {
		if (modifyTable.at(i).modifier == lineNo) {
			return true;
		}
	}
	return false;
}

bool ModifyTable::isModified(string modifier, string var) {
	if (isContained(modifier) == false) {
		return false;
	}
	vector<string> modVar = getModified(modifier);
	for (unsigned i = 0; i < modVar.size(); i++) {
		if (modVar.at(i) == var) {
			return true;
		}
	}
	return false;
}

vector<ModifyEntry_t> ModifyTable::getTable() {
	return modifyTable;
}
