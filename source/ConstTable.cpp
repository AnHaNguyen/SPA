#include "ConstTable.h"
#include <vector>
#include <string>

ConstTable::ConstTable() {
}

ConstTable::~ConstTable() {
}

bool ConstTable::addToTable(string line, string constant){
	if (!isContained(line)) {
		ConstEntry_t *entry = new ConstEntry_t(line, constant);
		constTable.push_back(*entry);
		return true;
	} 
	for (unsigned i = 0; i < constTable.size(); i++){
		if (constTable.at(i).line == line){
			for (unsigned j = 0; j < constTable.at(i).constants.size(); j++) {
				if (constTable.at(i).constants.at(j) == constant) {
					return false;
				}
			}
			constTable.at(i).constants.push_back(constant);
			break;
		}
	}
	return true;
}

vector<string> ConstTable::getConst(string line) {
	vector<string> returnList;
	for (unsigned i = 0; i < constTable.size(); i++){
		if (constTable.at(i).line == line) {
			returnList = constTable.at(i).constants;
			break;
		}
	}
	return returnList;
}

vector<string> ConstTable::getLine(string constant) {
	vector<string> returnList;
	for (unsigned i = 0; i < constTable.size(); i++) {
		for (unsigned j = 0; j < constTable.at(i).constants.size(); j++){
			if (constTable.at(i).constants.at(j) == constant) {
				returnList.push_back(constTable.at(i).line);
			}
		}
	}
	return returnList;
}

bool ConstTable::isContained(string line) {
	for (unsigned i = 0; i < constTable.size(); i++) {
		if (constTable.at(i).line == line) {
			return true;
		}
	}
	return false;
}

bool ConstTable::isConstOfLine(string line, string constant) {
	if (!isContained(line)) {
		return false;
	}
	vector<string> constants = getConst(line);
	for (unsigned i = 0; i < constants.size(); i++) {
		if (constants.at(i) == constant) {
			return true;
		}
	}
	return false;
}

int ConstTable::size() {
	return constTable.size();
}

vector<ConstEntry_t> ConstTable::getTable() {
	return constTable;
}
