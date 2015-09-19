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

