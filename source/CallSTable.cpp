#include "CallSTable.h"
#include <vector>
#include <string>
#include "CallTable.h"

CallSTable::CallSTable() {}
CallSTable::~CallSTable() {  }

bool CallSTable::addToTable(string caller, string callee) {
	if (isContained(caller) == false) {
		CallSEntry_t *entry = new CallSEntry_t(caller, callee);
		callSTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < callSTable.size(); i++) {
		if (callSTable.at(i).caller == caller) {
			for (unsigned j = 0; j < callSTable.at(i).callees.size(); j++) {
				if (callSTable.at(i).callees.at(j) == callee) {
					return false;
				}
			}
			callSTable.at(i).callees.push_back(callee);
			break;
		}
	}
	return true;
}

vector<string> CallSTable::getCalleeS(string caller) {
	vector <string> returnList;
	for (unsigned i = 0; i < callSTable.size(); i++) {
		if (callSTable.at(i).caller == caller) {
			returnList = callSTable.at(i).callees;
			break;
		}
	}
	return returnList;
}

vector<string> CallSTable::getCallerS(string callees) {
	vector<string> returnList;
	for (unsigned i = 0; i < callSTable.size(); i++) {
		for (unsigned j = 0; j < callSTable.at(i).callees.size(); j++) {
			if (callSTable.at(i).callees.at(j) == callees) {
				returnList.push_back(callSTable.at(i).caller);
			}
		}
	}
	return returnList;
}

int CallSTable::size() {
	return callSTable.size();
}

bool CallSTable::isContained(string line) {
	for (unsigned i = 0; i < callSTable.size(); i++) {
		if (callSTable.at(i).caller == line) {
			return true;
		}
	}
	return false;
}

vector<CallSEntry_t> CallSTable::getTable() {
	return callSTable;
}

bool CallSTable::isCallS(string caller, string callees) {
	if (!isContained(caller)) {
		return false;
	}
	vector<string> calleesList = getCalleeS(caller);
	for (unsigned i = 0; i < calleesList.size(); i++) {
		if (calleesList.at(i) == callees) {
			return true;
		}
	}
	return false;
}
