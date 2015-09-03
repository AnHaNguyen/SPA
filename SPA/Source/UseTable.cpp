#include <vector>
#include <string>
#include "UseTable.h"
#include "Utility.h"

UseTable::UseTable() {
}

UseTable::~UseTable() {
}

void UseTable::add(int lineNo, string var) {
	if (isContained(lineNo) == false) {
		UseEntry_t *entry = new UseEntry_t(lineNo, var);
		useTable.push_back(*entry);
		return;
	}
	for (unsigned i = 0; i < useTable.size(); i++) {
		if (useTable.at(i).lineNo == lineNo) {
			useTable.at(i).usedVar.push_back(var);
			useTable.at(i).usedVar =
				Utility::removeDuplicate(useTable.at(i).usedVar);
			break;
		}
	}
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
	return Utility::removeDuplicate(returnList);
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