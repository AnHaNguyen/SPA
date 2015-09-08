#include "ConstTable.h"
#include <vector>
#include <string>

ConstTable::ConstTable() {
}

ConstTable::~ConstTable() {
}

bool ConstTable::addToTable(int line, string constant){
	if (isConstOfLine(line, constant)) {
		return false;
	} 
	ConstEntry_t *entry = new ConstEntry_t(line, constant);
	constTable.push_back(*entry);
	return true;
}

vector<string> ConstTable::getConst(int line) {
	vector<string> returnList;
	for (unsigned i = 0; i < constTable.size(); i++){
		if (constTable.at(i).line == line) {
			returnList.push_back(constTable.at(i).constant);
		}
	}
	return returnList;
}

vector<int> ConstTable::getLine(string constant) {
	vector<int> returnList;
	for (unsigned i = 0; i < constTable.size(); i++) {
		if (constTable.at(i).constant == constant) {
			returnList.push_back(constTable.at(i).line);
		}
	}
	return returnList;
}

bool ConstTable::isContained(string constant){
	for (unsigned i = 0; i < constTable.size(); i++) {
		if (constTable.at(i).constant == constant) {
			return true;
		}
	}
	return false;
}

bool ConstTable::isContained(int line) {
	for (unsigned i = 0; i < constTable.size(); i++) {
		if (constTable.at(i).line == line) {
			return true;
		}
	}
	return false;
}

bool ConstTable::isConstOfLine(int line, string constant) {
	for (unsigned i = 0; i < constTable.size(); i++) {
		if (constTable.at(i).constant == constant 
			&& constTable.at(i).line == line) {
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
