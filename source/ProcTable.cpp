#include <string>
#include <vector>
#include "ProcTable.h"

ProcTable::ProcTable() {
}

ProcTable::~ProcTable() {
}

void ProcTable::addProc(string proc) {
	if (ProcTable::indexOf(proc) != -1) {
		return;
	}
	ProcTable::procTable.push_back(proc);
}

int ProcTable::indexOf(string proc) {
	for (int i = 0; i < ProcTable::size(); i++) {
		if (ProcTable::getProc(i) == proc) {
			return i;
		}
	}
	return -1;
}

string ProcTable::getProc(int index) {
	if (index < 0 || index >= ProcTable::size()) {
		return NULL;
	}
	return ProcTable::procTable.at(index);
}

int ProcTable::size() {
	return ProcTable::procTable.size();
}

string ProcTable::printProcTable() {
	string out = "";
	for (int i = 0; i < ProcTable::size(); i++) {
		out += i + ". " + ProcTable::getProc(i);
		out += '\n';		//end of line
	}
	return out;
}

vector<string> ProcTable::getTable() {
	return procTable;
}