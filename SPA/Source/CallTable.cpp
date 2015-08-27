#include <vector>
#include "CallTable.h"
#include "Utility.h"


CallTable::CallTable() {
}

CallTable::~CallTable() {
}

void CallTable::add(string caller, string callee) {
	for (unsigned i = 0; i < callTable.size(); i++) {
		if (callTable.at(i).caller == caller) {
			callTable.at(i).callees.push_back(callee);
			callTable.at(i).callees = Utility::removeDuplicate(callTable.at(i).callees);
			break;
		}
	}
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
	return Utility::removeDuplicate(returnList);
}

int CallTable::size() {
	return callTable.size();
}