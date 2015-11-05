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

string ProgLine::getType(string lineNo) {
	unsigned int line = atoi(lineNo.c_str());
	if (progLine.size() < line || line <= 0) {
		return "";
	}
	else return progLine.at(line - 1).type;
}

string ProgLine::getProcedure(string lineNo) {
	unsigned int line = atoi(lineNo.c_str());
	if (progLine.size() < line || line <= 0) {
		return "";
	}
	else return progLine.at(line - 1).procedure;
}

int ProgLine::numOfLines() {
	return progLine.size();
}

vector<string> ProgLine::getLinesOfProc(string procedure) {
	vector<string> returnList;
	for (unsigned i = 1; i <= progLine.size(); i++) {
		if (getProcedure(to_string(i)) == procedure) {
			returnList.push_back(to_string(i));
			int j = i + 1;
			while (getProcedure(to_string(j)) == procedure) {
				returnList.push_back(to_string(j));
				j++;
			}
			break;
		}
	}
	return returnList;
}

vector<string> ProgLine::getLinesOfType(string type){
	vector<string> returnList;
	if (type == "prog_line" || type == "stmt") {
		for (unsigned i = 1; i <= progLine.size(); i++) {
			returnList.push_back(to_string(i));
		}
		return returnList;
	}
	for (unsigned i = 1; i <= progLine.size(); i++) {
		if (getType(to_string(i)) == type) {
			returnList.push_back(to_string(i));
		}
	}
	return returnList;
}

vector<stmt_t> ProgLine::getTable() {
	return progLine;
}

vector<string> ProgLine::getAssignsOfProc(string procedure) {
	vector<string> returnList;
	for (unsigned i = 1; i <= progLine.size(); i++) {
		if (getProcedure(to_string(i)) == procedure && 
					getType(to_string(i)) == "assign") {
			returnList.push_back(to_string(i));
			int j = i + 1;
			while (getProcedure(to_string(j)) == procedure) {
				if (getType(to_string(j)) == "assign") {
					returnList.push_back(to_string(j));
				}
				j++;
			}
			break;
		}
	}
	return returnList;
}

/*void ProgLine::updateFollowS(int line, string s) {
	progLine.at(line).followS.push_back(s);
}

void ProgLine::updateParent(int line, string s) {
	progLine.at(line).parent = s;
}

void ProgLine::updateParentS(int line, string s) {
	progLine.at(line).parentS.push_back(s);
}

void ProgLine::updateNext(int line, string s) {
	progLine.at(line).next.push_back(s);
}

vector<string> ProgLine::getFollowS(int line) {
	return progLine.at(line - 1).followS;
}

string ProgLine::getParent(int line) {
	return progLine.at(line - 1).parent;
}

vector<string> ProgLine::getParentS(int line) {
	return progLine.at(line - 1).parentS;
}

vector<string> ProgLine::getNext(int line) {
	return progLine.at(line - 1).next;
}*/