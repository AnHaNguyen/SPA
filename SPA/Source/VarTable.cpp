#include <string>
#include <vector>
#include "VarTable.h"

VarTable::VarTable() {		//constructor
}

VarTable::~VarTable(){
}

void VarTable::addVar(string var) {
	if (VarTable::indexOf(var) != -1) {
		return;
	}
	VarTable::varTable.push_back(var);
}

string VarTable::getVar(int index) {
	if (index < 0 || index >= VarTable::size()) {
		return NULL;
	}
	return VarTable::varTable.at(index);
}

int VarTable::indexOf(string var) {
	for (int i = 0; i < VarTable::size(); i++) {
		if (VarTable::getVar(i) == var) {
			return i;
		}
	}
	return -1;
}

int VarTable::size() {
	return VarTable::varTable.size();
}

string VarTable::printVarTable() {
	string out = "";
	for (int i = 0; i < VarTable::size(); i++) {
		out += i + ". " + VarTable::getVar(i);
		out += '\n';		//end of line
	}
	return out;
}