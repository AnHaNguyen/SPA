#include "../QueryProcessor/HUtility.h"

HUtility::HUtility() {}
HUtility::~HUtility() {}
static vector <vector<string>> symTable;
//Set symbol table
void HUtility::setSymTable(vector<vector<string>> symbolTable) {
	symTable = symbolTable;
}
//Get tables
vector<string> HUtility::getAssignTable() {
	vector<string> ansVec;
	ProgLine* progLine = PKB::getProgLine();
	ansVec = progLine->getLinesOfType("assign");
	return ansVec;
}

void HUtility::getUseTable(vector<UseEntry_t> &useTab, vector<string> &userTable, vector<string> &usedTable) {
	UseTable* UseTable = PKB::getUseTable();
	useTab = UseTable->getTable();
	userTable = getUseRelated(useTab, 1);
	usedTable = getUseRelated(useTab, 2);
}

bool HUtility::getParentTable(PreResultNode * result, string &firstAtt, vector<string> &parTable,
	string &secondAtt, vector<string> &nestTable) {
	vector<ParentEntry_t> parTab;
	ParentTable* ParentTable = PKB::getParentTable();
	parTab = ParentTable->getTable();
	if (parTab.empty()) {
		return false;
	}
	if (result->getResult().getSynonym() == firstAtt) {
		parTable = getParentRelated(parTab, 1);
	}
	if (result->getResult().getSynonym() == secondAtt) {
		nestTable = getParentRelated(parTab, 2);
	}
	if (result->getResult().getSynonym() != firstAtt && result->getResult().getSynonym() != secondAtt) {
		parTable = getParentRelated(parTab, 1);
	}
	return true;
}

void HUtility::getModifyTable(vector<pair<string, vector<string>>> &modTable) {
	vector<ModifyEntry_t> modTab = PKB::getModifyTable()->getTable();
	for (size_t i = 0; i < modTab.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = modTab[i].modifier;
		temp.second = modTab[i].modifiedVar;
		modTable.push_back(temp);
	}
}

void HUtility::getFollowTable(vector<pair<string, string>> &folTable) {
	vector<FollowEntry_t> folTab = PKB::getFollowTable()->getTable();
	for (size_t i = 0; i < folTab.size(); i++) {
		pair<string, string> temp;
		temp.first = folTab[i].prev;
		temp.second = folTab[i].next;
		folTable.push_back(temp);
	}
}

void HUtility::getNextTable(vector<pair<string, vector<string>>> &nextTable) {
	vector<NextEntry_t> nextTab = PKB::getNextTable()->getTable();
	for (size_t i = 0; i < nextTab.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = nextTab[i].lineNo;
		temp.second = nextTab[i].nextStmts;
		nextTable.push_back(temp);
	}
}

void HUtility::getCallTable(vector<pair<string, vector<string>>> &callTable) {
	vector<CallEntry_t> callTab = PKB::getCallTable()->getTable();
	for (size_t i = 0; i < callTab.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = callTab[i].caller;
		temp.second = callTab[i].callees;
		callTable.push_back(temp);
	}
}

vector<pair<string, vector<string>>> HUtility::getConstTable() {
	vector<ConstEntry_t> temp1 = PKB::getConstTable()->getTable();
	vector<pair<string, vector<string>>> ansVec;
	for (size_t i = 0; i < temp1.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = temp1[i].line;
		temp.second = temp1[i].constants;
		ansVec.push_back(temp);
	}
	return ansVec;
}

//Other utilities
string HUtility::getSymMean(string sym) {
	for (size_t i = 0; i < symTable.size(); i++) {
		vector<string> current = symTable[i];
		for (size_t j = 0; j < current.size(); j++) {
			if (current[j] == sym) {
				return current[0];
			}
		}
	}
	return "";
}

int HUtility::getPos(vector<int> intVec) {
	for (size_t i = 0; i < intVec.size(); i++) {
		if (intVec[i] == 1) {
			return i;
		}
	}
	return -1;
}

bool HUtility::isInt(string &secondAtt)
{
	try {
		int number = stoi(secondAtt);
		return true;
	}
	catch (exception e) {
		return false;
	}
}

bool HUtility::contain(vector<string> vec, string str) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (find(begin(vec), end(vec), str) != end(vec)) {
			return true;
		}
	}
	return false;
}

//To convetion
vector<string> HUtility::getParentRelated(vector<ParentEntry_t>  table, int x) {
	vector<string> ansVec;
	if (x == 1) {
		for (size_t i = 0; i < table.size(); i++) {
			ansVec.push_back(table[i].lineNo);
		}
	}
	else {
		for (size_t i = 0; i < table.size(); i++) {
			for (size_t j = 0; j < table[i].child.size(); j++) {
				ansVec.push_back(table[i].child[j]);
			}
		}
	}
	return ansVec;
}
vector<string> HUtility::getUseRelated(vector<UseEntry_t> table, int x) {
	vector<string> ansVec;
	if (x == 1) {
		for (size_t i = 0; i < table.size(); i++) {
			ansVec.push_back(table[i].userLine);
		}
		return ansVec;
	}
	else {
		for (size_t i = 0; i < table.size(); i++) {
			for (size_t j = 0; j < table[i].usedVar.size(); j++) {
				ansVec.push_back(table[i].usedVar[j]);
			}
		}
	}
}

//Implement intersection method (case pair (n1, v1) and select v or n
vector<string> HUtility::intersection(vector<string> vec1, vector<string> vec2) {
	vector<string> ansVec;
	if (vec1.size() > 0 && vec2.size()>0) {
		if (vec1[0] == "true") {
			ansVec = vec2;
			return ansVec;
		}
		if (vec2[0] == "true") {
			ansVec = vec1;
			return ansVec;
		}
		for (size_t i = 0; i < vec1.size(); i++) {
			string current = vec1[i];
			for (size_t j = 0; j < vec2.size(); j++) {
				if (current == vec2[j]) {
					ansVec.push_back(current);
				}
			}
		}
	}
	return ansVec;
}

vector<string> HUtility::intersection(vector<string> vec1, vector<pair<string, string>> vec2) {
	vector<string> ansVec;
	for (size_t i = 0; i < vec1.size(); i++) {
		string current = vec1[i];
		for (size_t j = 0; j < (sizeof vec2); j++) {
			if (vec2[j].second == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}

vector < pair<string, vector<string>>> HUtility::intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2) {
	vector < pair<string, vector<string>>> ansVec;
	for (size_t i = 0; i < vec1.size(); i++) {
		string current = vec1[i];
		for (size_t j = 0; j < (sizeof vec2); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(vec2[j]);
			}
		}
	}
	return ansVec;
}

vector <string> HUtility::intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2, bool check) {
	vector <string> ansVec;
	for (size_t i = 0; i < vec1.size(); i++) {
		string current = vec1[i];
		for (size_t j = 0; j < (sizeof vec2); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}

void HUtility::checkQuotation(pair<string, bool> &AttQ, string &Att) {

	//Case "x"
	if (Att.substr(0, 1) == "\"") {
		AttQ.first = Att.substr(1, Att.size() - 2);
		AttQ.second = true;
	}
	//Case v or _
	else {
		AttQ.first = Att;
		AttQ.second = false;
	}
}