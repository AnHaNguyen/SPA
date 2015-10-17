#include <vector>
#include "CallTable.h"
#include "Utility.h"


CallTable::CallTable() {
}

CallTable::~CallTable() {
}

bool CallTable::addToTable(string caller, string callee) {
	if (isContained(caller) == false) {
		callEntry_t *entry = new callEntry_t(caller, callee);
		callTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < callTable.size(); i++) {
		if (callTable.at(i).caller == caller) {
			for (unsigned j = 0; j < callTable.at(i).callees.size(); j++) {
				if (callTable.at(i).callees.at(j) == callee) {
					return false;
				}
			}
			callTable.at(i).callees.push_back(callee);
		}
	}
	return true;
}

vector<string> CallTable::getCallees(string caller) {
	vector <string> returnList;
	for (unsigned i = 0; i < callTable.size(); i++) {
		if (callTable.at(i).caller == caller) {
			returnList = callTable.at(i).callees;
			break;
		}
	}
	return returnList;
}

vector<string> CallTable::getCallers(string callee) {
	vector <string> returnList;
	for (unsigned i = 0; i < callTable.size(); i++) {
		for (unsigned j = 0; j < callTable.at(i).callees.size(); j++) {
			if (callTable.at(i).callees.at(j) == callee) {
				returnList.push_back(callTable.at(i).caller);
			}
		}
	}
	return returnList;
}

int CallTable::size() {
	return callTable.size();
}

bool CallTable::isContained(string caller) {
	for (unsigned i = 0; i < callTable.size(); i++) {
		if (callTable.at(i).caller == caller) {
			return true;
		}
	}
	return false;
}

bool CallTable::isCall(string caller, string callee) {
	if (!isContained(caller)) {
		return false;
	}
	vector<string> callees = getCallees(caller);
	for (unsigned i = 0; i < callees.size(); i++) {
		if (callees.at(i) == callee) {
			return true;
		}
	}
	return false;
}

bool CallTable::isContainedRecur() {
	for (unsigned i = 0; i < callTable.size(); i++) {
		string caller = callTable.at(i).caller;
		vector<string> callees = callTable.at(i).callees;
		vector<string> processed;
		if (checkRecurDFS(caller, callees, processed)) {
			return true;
		}
	}
	return false;
}

//check is call table contains any recursion using DFS
bool CallTable::checkRecurDFS(string caller, vector<string> callees, vector<string> processed) {
	for (unsigned i = 0; i < callees.size(); i++) {
		if (callees.at(i) == caller) {
			return true;
		}
		else {
			for (unsigned j = 0; j < processed.size(); j++) {
				if (processed.at(j) == callees.at(i)) {
					return false;
				}
			}

			vector<string> newCallees = getCallees(callees.at(i));
			processed.push_back(callees.at(i));
			if (checkRecurDFS(caller, newCallees, processed)) {
				return true;
			}
		}
	}
	return false;
}

vector<callEntry_t> CallTable::getTable() {
	return callTable;
}

void CallTable::generateCallSTable() {
	if (isContainedRecur()) {
		return;
	}
	CallSTable* callSTable = new CallSTable();
	for (unsigned i = 0; i < callTable.size(); i++) {
		queue<string> lineQ;
		string line = callTable.at(i).caller;
		lineQ.push(line);
		while (!lineQ.empty) {
			string cur = lineQ.front();
			lineQ.pop();
			vector<string> callees = getCallees(cur);
			for (unsigned j = 0; j < callees.size(); j++) {
				lineQ.push(callees.at(j));
				callSTable->addToTable(line, callees.at(j));
			}
		}
	}
}