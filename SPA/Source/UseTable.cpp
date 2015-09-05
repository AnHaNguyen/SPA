#include <vector>
#include <string>
#include "UseTable.h"
#include "Utility.h"

UseTable::UseTable() {
}

UseTable::~UseTable() {
}

bool UseTable::add(int lineNo, string var) {
	if (isContained(lineNo) == false) {
		UseEntry_t *entry = new UseEntry_t(lineNo, var);
		useTable.push_back(*entry);
		return true;
	}
	for (unsigned i = 0; i < useTable.size(); i++) {
		if (useTable.at(i).lineNo == lineNo) {
			for (unsigned j = 0; j < useTable.at(i).usedVar.size(); j++) {
				if (useTable.at(i).usedVar.at(j) == var) {
					return false;
				}
			}
			useTable.at(i).usedVar.push_back(var);
			break;
		}
	}
	return true;
}

int UseTable::size() {
	return useTable.size();
}

vector<int> UseTable::getUser(string var) {
	vector<int> returnList;
	for (unsigned i = 0; i < useTable.size(); i++) {
		for (unsigned j = 0; j < useTable.at(i).usedVar.size(); j++) {
			if (useTable.at(i).usedVar.at(j) == var) {
				returnList.push_back(useTable.at(i).lineNo);
			}
		}
	}
	return returnList;
}

vector<string> UseTable::getUsed(int lineNo) {
	vector<string> returnList;
	for (unsigned i = 0; i < useTable.size(); i++) {
		if (useTable.at(i).lineNo == lineNo) {
			returnList = useTable.at(i).usedVar;
			break;
		}
	}
	return returnList;
}

bool UseTable::isContained(int lineNo) {
	for (unsigned i = 0; i < useTable.size(); i++) {
		if (useTable.at(i).lineNo == lineNo) {
			return true;
		}
	}
	return false;
}

bool UseTable::isUsed(int line, string var) {
	if (isContained(line) == false) {
		return false;
	}
	vector <string> usedList = getUsed(line);
	for (unsigned i = 0; i < usedList.size(); i++) {
		if (usedList.at(i) == var) {
			return true;
		}
	}
	return false;
}