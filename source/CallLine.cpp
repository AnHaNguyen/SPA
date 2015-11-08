#include "CallLine.h"

CallLine::CallLine(){}

CallLine::~CallLine(){}

void CallLine::addToTable(string line, string callee) {
	CallLineEntry_t *entry = new CallLineEntry_t(line, callee);
	callLine.push_back(*entry);
}

string CallLine::getCallee(string line) {
	for (unsigned i = 0; i < callLine.size(); i++) {
		if (callLine.at(i).lineNo == line) {
			return callLine.at(i).callee;
		}
	}
	return "";
}
vector<string> CallLine::getCallers(string callee) {
	vector<string> returnList;
	for (unsigned i = 0; i < callLine.size(); i++) {
		if (callLine.at(i).callee == callee) {
			returnList.push_back(callLine.at(i).lineNo);
		}
	}
	return returnList;
}

vector < pair<string, string>> CallLine::getTable() {
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < callLine.size(); i++) {
		pair < string, string > newPair;
		newPair.first = callLine.at(i).lineNo;
		newPair.second = callLine.at(i).callee;
		returnList.push_back(newPair);
	}
	return returnList;
}

int CallLine::size() {
	return callLine.size();
}