#include <string>
#include <vector>
#include "ProgLine.h"

ProgLine::ProgLine() { }

ProgLine::~ProgLine() {
}

bool ProgLine::addToList(int lineNo, string type, string procedure) {
	if (progLine.size() != lineNo - 1) {
		return false;
	}
	stmt_t *entry = new stmt_t(procedure, type);
	progLine.push_back(*entry);
	return true;
}

string ProgLine::getType(int lineNo) {
	if (progLine.size() < lineNo || lineNo <= 0) {
		return "";
	}
	else return progLine.at(lineNo - 1).type;
}

string ProgLine::getProcedure(int lineNo) {
	if (progLine.size() < lineNo || lineNo <= 0) {
		return "";
	}
	else return progLine.at(lineNo - 1).procedure;
}

int ProgLine::numOfLines() {
	return progLine.size();
}

vector<int> ProgLine::getLinesOfProc(string procedure) {
	vector<int> returnList;
	for (unsigned i = 1; i <= progLine.size(); i++) {
		if (getProcedure(i) == procedure) {
			returnList.push_back(i);
			int j = i + 1;
			while (getProcedure(j) == procedure) {
				returnList.push_back(j);
				j++;
			}
			break;
		}
	}
	return returnList;
}

vector<int> ProgLine::getLinesOfType(string type){
	vector<int> returnList;
	for (unsigned i = 1; i <= progLine.size(); i++) {
		if (getType(i) == type) {
			returnList.push_back(i);
		}
	}
	return returnList;
}
