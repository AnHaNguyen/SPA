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

void HUtility::getUseTable(vector<pair<string, vector<string>>> &useTable) {
	vector<UseEntry_t> useTab = PKB::getUseTable()->getTable();
	for (size_t i = 0; i < useTab.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = useTab[i].userLine;
		temp.second = useTab[i].usedVar;
		useTable.push_back(temp);
	}
}

void HUtility::getParentTable(vector<pair<string, vector<string>>> &parTable) {
	vector<ParentEntry_t> parTab = PKB::getParentTable()->getTable();
	for (size_t i = 0; i < parTab.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = parTab[i].lineNo;
		temp.second = parTab[i].child;
		parTable.push_back(temp);
	}
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

bool HUtility::isInt(string att)
{
	try {
		int number = stoi(att);
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

bool HUtility::contain(vector<int> vec, int str) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (find(begin(vec), end(vec), str) != end(vec)) {
			return true;
		}
	}
	return false;
}

int HUtility::contain(vector<attEntry_t> vec, string str) {
	if (str == "") {
		return -2;
	}
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i].att == str) {
			return i;
		}
	}
	return -1;
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
	}
	else {
		for (size_t i = 0; i < table.size(); i++) {
			for (size_t j = 0; j < table[i].usedVar.size(); j++) {
				ansVec.push_back(table[i].usedVar[j]);
			}
		}
	}
	return ansVec;
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
		for (size_t j = 0; j < (vec2.size()); j++) {
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
		for (size_t j = 0; j < (vec2.size()); j++) {
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
		for (size_t j = 0; j < (vec2.size()); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}

int HUtility::intersectionSS(vector<string> &vec1, vector<string> &vec2, int check) {
	vector<string> ansVec;
	if (vec1.size() > 0 && vec2.size()>0) {
		if (vec1[0] == "true") {
			return 0;
		}
		if (vec2[0] == "true") {
			return 0;
		}
		for (size_t i = 0; i < vec1.size(); i++) {
			string current = vec1[i];
			for (size_t j = 0; j < vec2.size(); j++) {
				if (current == vec2[j]) {
					ansVec.push_back(current);
					break;
				}
			}
		}
	}
	if (vec1.size() == ansVec.size() && vec2.size() == ansVec.size()) {
		return 0;
	}
	vec1 = ansVec;
	vec2 = ansVec;
	return 1;
}

int HUtility::intersectionPSS(vector<string> &vec1, vector<pair<string, string>> &vec2, int check) {
	vector<string> ansVec1;
	vector<pair<string, string>> ansVec2;
	if (vec1.size() > 0 && vec2.size() > 0) {
		if (vec1[0] == "true") {
			return 0;
		}
	}
	if (check == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec2[j].first == vec1[i]) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (check == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec2[j].second == vec1[i]) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (vec1.size() == ansVec1.size() && vec2.size() == ansVec2.size()) {
		return 0;
	}
	vec1 = ansVec1;
	vec2 = ansVec2;
	return 1;
}

int HUtility::intersectionPSV(vector<string> &vec1, vector<pair<string, vector<string>>> &vec2, int check) {
	vector<string> ansVec1;
	vector < pair<string, vector<string>>> ansVec2;
	if (vec1.size() > 0 && vec2.size() > 0) {
		if (vec1[0] == "true") {
			return 0;
		}
	}
	if (check == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec2[j].first == vec1[i]) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (check == 2) {
		vector<string> total1;
		vector<string> total2;
		ansVec1 = vec1;
		ansVec2 = vec2;
		int temp;
		for (size_t i = 0; i < vec1.size(); i++) {
			if (!HUtility().contain(total1, vec1[i])) {
				total1.push_back(vec1[i]);
			}
		}
		for (size_t i = 0; i < vec2.size(); i++) {
			for (size_t j = 0; j < vec2[i].second.size(); j++) {
				if (!HUtility().contain(total2, vec2[i].second[j])) {
					total2.push_back(vec2[i].second[j]);
				}
			}
		}

		HUtility().intersectionSS(total1, total2, 1);
		size_t i = 0;
		size_t j = 0;
		while (vec1.size() > 0 && i < vec1.size()) {
			if (!HUtility().contain(total1, vec1[i])) {
				vec1.erase(vec1.begin() + i);
			}
			else {
				i++;
			}
		}
		i = 0;
		j = 0;
		while (vec2.size() > 0 && i < vec2.size()) {
			while (vec2[i].second.size() > 0 && j < vec2[i].second.size()) {
				if (!HUtility().contain(total1, vec2[i].second[j])) {
					vec2[i].second.erase(vec2[i].second.begin() + j);
				}
				else {
					j++;
				}
			}
			if (vec2[i].second.size() == 0) {
				vec2.erase(vec2.begin() + i);
			}
			else {
				i++;
			}
		}
	}
	if (vec1.size() == ansVec1.size() && vec2.size() == ansVec2.size()) {
		return 0;
	}
	if (check != 2) {
		vec1 = ansVec1;
		vec2 = ansVec2;
	}
	return 1;
}

int HUtility::intersectionPPSS(vector<pair<string, string>> &vec1, vector<pair<string, string>> &vec2, int att1, int att2) {
	vector<pair<string, string>> ansVec1;
	vector<pair<string, string>> ansVec2;
	if (att1 == 1 && att2 == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].first == vec2[j].first) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 1 && att2 == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].first == vec2[j].second) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 2 && att2 == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].second == vec2[j].first) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 2 && att2 == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].second == vec2[j].second) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (vec1.size() == ansVec1.size() && vec2.size() == ansVec2.size()) {
		return 0;
	}
	vec1 = ansVec1;
	vec2 = ansVec2;
	return 1;
}

int HUtility::intersectionPPSM(vector<pair<string, string>> &vec1, vector<pair<string, vector<string>>> &vec2, int att1, int att2) {
	vector<pair<string, string>> ansVec1;
	vector<pair<string, vector<string>>> ansVec2;
	if (att1 == 1 && att2 == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].first == vec2[j].first) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 1 && att2 == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			bool added = 0;
			for (size_t j = 0; j < vec2.size(); j++) {
				if (find(vec2[j].second.begin(), vec2[j].second.end(), vec1[i].first) != vec2[j].second.end()) {
					ansVec2.push_back(vec2[j]);
					added = 1;
				}
			}
			if (added) {
				ansVec1.push_back(vec1[i]);
			}
		}
	}
	if (att1 == 2 && att2 == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].second == vec2[j].first) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 2 && att2 == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			bool added = 0;
			for (size_t j = 0; j < vec2.size(); j++) {
				if (find(vec2[j].second.begin(), vec2[j].second.end(), vec1[i].second) != vec2[j].second.end()) {
					ansVec2.push_back(vec2[j]);
					added = 1;
				}
			}
			if (added) {
				ansVec1.push_back(vec1[i]);
			}
		}
	}
	if (vec1.size() == ansVec1.size() && vec2.size() == ansVec2.size()) {
		return 0;
	}
	vec1 = ansVec1;
	vec2 = ansVec2;
	return 1;
}

int HUtility::intersectionPPSV(vector<pair<string, vector<string>>> &vec1, vector<pair<string, vector<string>>> &vec2, int att1, int att2) {
	vector<pair<string, vector<string>>> ansVec1;
	vector<pair<string, vector<string>>> ansVec2;

	if (att1 == 1 && att2 == 1) {
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec2.size(); j++) {
				if (vec1[i].first == vec2[j].first) {
					ansVec1.push_back(vec1[i]);
					ansVec2.push_back(vec2[j]);
					break;
				}
			}
		}
	}
	if (att1 == 1 && att2 == 2) {
		for (size_t i = 0; i < vec1.size(); i++) {
			bool added = 0;
			for (size_t j = 0; j < vec2.size(); j++) {
				if (find(vec2[j].second.begin(), vec2[j].second.end(), vec1[i].first) != vec2[j].second.end()) {
					ansVec2.push_back(vec2[j]);
					added = 1;
				}
			}
			if (added) {
				ansVec1.push_back(vec1[i]);
			}
		}
	}

	if (att1 == 2 && att2 == 1) {
		for (size_t i = 0; i < vec2.size(); i++) {
			bool added = 0;
			for (size_t j = 0; j < vec1.size(); j++) {
				if (find(vec1[j].second.begin(), vec1[j].second.end(), vec2[i].first) != vec1[j].second.end()) {
					ansVec1.push_back(vec1[j]);
					added = 1;
				}
			}
			if (added) {
				ansVec1.push_back(vec2[i]);
			}
		}
	}

	if (att1 == 2 && att2 == 2) {
		vector<string> total1;
		vector<string> total2;
		ansVec1 = vec1;
		ansVec2 = vec2;
		int temp;
		for (size_t i = 0; i < vec1.size(); i++) {
			for (size_t j = 0; j < vec1[i].second.size(); j++) {
				if (!HUtility().contain(total1, vec1[i].second[j])) {
					total1.push_back(vec1[i].second[j]);
				}
			}
		}
		for (size_t i = 0; i < vec2.size(); i++) {
			for (size_t j = 0; j < vec2[i].second.size(); j++) {
				if (!HUtility().contain(total2, vec2[i].second[j])) {
					total2.push_back(vec2[i].second[j]);
				}
			}
		}

		HUtility().intersectionSS(total1, total2, 1);
		size_t i = 0;
		size_t j = 0;
		while (vec1.size() > 0 && i < vec1.size()) {
			while (vec1[i].second.size() > 0 && j < vec1[i].second.size()) {
				if (!HUtility().contain(total1, vec1[i].second[j])) {
					vec1[i].second.erase(vec1[i].second.begin() + j);
				}
				else {
					j++;
				}
			}
			if (vec1[i].second.size() == 0) {
				vec1.erase(vec1.begin() + i);
			}
			else {
				i++;
			}
		}
		i = 0;
		j = 0;
		while (vec2.size() > 0 && i < vec2.size()) {
			while (vec2[i].second.size() > 0 && j < vec2[i].second.size()) {
				if (!HUtility().contain(total1, vec2[i].second[j])) {
					vec2[i].second.erase(vec2[i].second.begin() + j);
				}
				else {
					j++;
				}
			}
			if (vec2[i].second.size() == 0) {
				vec2.erase(vec2.begin() + i);
			}
			else {
				i++;
			}
		}
	}
	if (vec1.size() == ansVec1.size() && vec2.size() == ansVec2.size()) {
		return 0;
	}
	if (att1 != 2 || att2 != 2) {
		vec1 = ansVec1;
		vec2 = ansVec2;
	}
	return 1;
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