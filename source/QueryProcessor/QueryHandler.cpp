#include <string>
#include <vector>
#include <algorithm>
#include <queue>

#include "../PKB.h"
#include "../QueryProcessor/QueryHandler.h"
#include "../Preprocessor/QueryTree.h"
#include "../FollowTable.h"
#include "../ModifyTable.h"
#include "../ParentTable.h"



QueryHandler::QueryHandler() {}
QueryHandler::~QueryHandler() {}


vector<string> QueryHandler::queryRec(QueryTree* query) {

	//check validity
	if (query->getValidity() == false)
		return{};

	//Initiate nodes
	PreResultNode* result;
	PreSuchThatNode* suchThat;
	PrePatternNode* pattern;
	if (query->getSymbolTable().size() != 0) {
		symTable = query->getSymbolTable();
	}
	else {
		return{};
	}

	//Handle select
	string selType = handleSelect(query, result);

	//Handle suchThat
	if (query->getSuchThat() != NULL) {
		suchThat = query->getSuchThat();
		string syn = suchThat->getSynonym();
		string firstAtt = suchThat->getFirstAttr();
		string secondAtt = suchThat->getSecondAttr();

		//Handle follows
		vector<int> folVec;
		vector<FollowEntry_t> folTab;
		if (syn == "follows") {
			folVec.push_back(handleFollows(firstAtt, secondAtt));
			if (folVec.front() == -1) {
				FollowTable* FollowTable = PKB::getFollowTable();
				folTab = FollowTable->getTable();
			}
		}

		//Handle follows*
		if (syn == "follows*") {
			folVec.push_back(handleFollows(firstAtt, secondAtt));
			queue<int> folQ;
			folQ.push(folVec[0]);
			if (folVec.front() == -1) {
				FollowTable* FollowTable = PKB::getFollowTable();
				folTab = FollowTable->getTable();
			}
			else {
				while (!folQ.empty()) {
					int temp = folQ.front();
					folQ.pop();
					int nextFol;
					if (isInt(firstAtt)) {
						nextFol = handleFollows(to_string(temp), secondAtt);
						if (nextFol != -1) {
							folQ.push(nextFol);
							folVec.push_back(nextFol);
						}
					}
					else {
						nextFol = handleFollows(firstAtt, to_string(temp));
						if (nextFol != -1) {
							folQ.push(nextFol);
							folVec.push_back(nextFol);
						}
					}
				}
			}
		}

		//Handle modifies
		vector<string> mvarVec;
		vector<int> modVec;
		vector<ModifyEntry_t> modTab;
		if (syn == "modifies") {
			handleModifies(firstAtt, secondAtt, modVec, mvarVec);
			if (modVec.front() == -1) {
				ModifyTable* ModifyTable = PKB::getModifyTable();
				modTab = ModifyTable->getTable();
			}
		}

		//Handle parent
		vector<int> parVec;
		vector<ParentEntry_t> parTab;
		if (syn == "parent") {
			handleParent(firstAtt, secondAtt, parVec);
			if (parVec.front() == -1) {
				ParentTable* ParentTable = PKB::getParentTable();
				parTab = ParentTable->getTable();
			}
		}

		//Handle parent*
		if (syn == "parent*") {
			handleParent(firstAtt, secondAtt, parVec);
			queue<int> parQ;

			if (parVec.front() == -1) {
				ParentTable* ParentTable = PKB::getParentTable();
				parTab = ParentTable->getTable();
			}
			else {
				parQ.push(parVec.front());
				while (!parQ.empty()) {
					int temp = parQ.front();
					int oldSize = parVec.size();
					parQ.pop();
					if (isInt(firstAtt)) {
						handleParent(to_string(temp), secondAtt, parVec);
						if (oldSize < parVec.size()) {
							for (int i = oldSize; i < parVec.size(); i++) {
								parQ.push(parVec[i]);
							}
						}
					}
					else {
						handleParent(firstAtt, to_string(temp), parVec);
						if (oldSize < parVec.size()) {
							for (int i = oldSize; i < parVec.size(); i++) {
								parQ.push(parVec[i]);
							}
						}
					}
				}
			}
		}

		//Handle uses
		vector<string> uvarVec;
		vector<int> useVec;
		vector<UseEntry_t> useTab;
		if (syn == "uses") {
			handleUses(firstAtt, secondAtt, useVec, uvarVec);
			if (useVec.front() == -1) {
				UseTable* UseTable = PKB::getUseTable();
				useTab = UseTable->getTable();
			}
		}

		/*Handle affecs - next (next iteration)
		if (syn == "affects") {
		ansVec = getAffect(firstAtt, secondAtt);
		}
		if (syn == "affects*") {
		ansVec = getAffectS(firstAtt, secondAtt);
		}
		if (syn == "next") {
		ansVec = getNext(firstAtt, secondAtt);
		}
		if (syn == "next") {
		ansVec = getNextS(firstAtt, secondAtt);
		}*/
	}

	//Handle pattern (this iteration only assign patter)
	if (query->getPattern() != NULL) {
		pattern = query->getPattern();
		string syn = pattern->getSynonym();
		string pType = getSymMean(syn);
		string firstAtt = pattern->getFirstAttr();
		string secondAtt = pattern->getSecondAttr();

		pair<string, bool> firstAttx;
		pair<string, bool> secondAttx;
		atoPair(firstAttx, firstAtt);
		atoPair(secondAttx, secondAtt);

		vector<int> patVec;
		vector<string> pvarVec;
		vector<string> pconVec;

		//Case 1st att = _
		if (firstAttx.first == "") {
			//Case 2nd att = _
			if (secondAttx.first == "_") {
				vector<AST*> ast = PKB::getASTList();
				for (int i = 0; i < ast.size(); i++) {
					vector<int> current = ast[i]->getAssign();
					for (int j = 0; j < current.size(); j++) {
						patVec.push_back(current[j]);
					}
				}
			}
			//Case 2nd att = "x123"
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "") {
				patVec = PKB::getUseTable()->getUser(secondAttx.first);
			}
			//Case 2nd att = v
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "variable") {
				//Select type = variable
				if (selType == "variable") {
					vector<AST*> ast = PKB::getASTList();
					vector<int> tempVec;
					for (int i = 0; i < ast.size(); i++) {
						vector<int> current = ast[i]->getAssign();
						for (int j = 0; j < current.size(); j++) {
							tempVec.push_back(current[j]);
						}
					}
					for (int i = 0; i < tempVec.size(); i++) {
						vector<string> current = PKB::getUseTable()->getUsed(tempVec[i]);
						for (int j = 0; j < current.size(); j++) {
							pvarVec.push_back(current[j]);
						}
					}
				}
				//Select type = assignment
				if (selType == "assignment") {
					vector<AST*> ast = PKB::getASTList();
					vector<int> tempVec;
					for (int i = 0; i < ast.size(); i++) {
						vector<int> current = ast[i]->getAssign();
						for (int j = 0; j < current.size(); j++) {
							tempVec.push_back(current[j]);
						}
					}
					for (int i = 0; i < tempVec.size(); i++) {
						vector<string> current = PKB::getUseTable()->getUsed(tempVec[i]);
						for (int j = 0; j < current.size(); j++) {
							if (current.size() > 0) {
								patVec.push_back(tempVec[i]);
							}
						}
					}
				}
			}
		}
	}
}

bool QueryHandler::containSign(string str) {
	if (str.find("+") != string::npos || str.find("-") != string::npos || str.find("*") != string::npos) {
		return true;
	}
	return false;
}
void QueryHandler::atoPair(pair<string, bool> &Attx, string &Att) {

	//Case _"x+y"_ or _
	if (Att.substr(0, 1) == "_") {
		if (Att.size() > 1) {
			Attx.first = Att.substr(2, Att.size() - 4);
			Attx.second = true;
		}
		else {
			Attx.first = Att;
			Attx.second = false;
		}
	}
	//Case "x+y", "x" or v, c
	else {
		if (Att.substr(0, 1) == "\"") {
			Attx.first = Att.substr(1, Att.size() - 2);
			Attx.second = false;
		}
		else {
			Attx.first = Att;
			Attx.second = false;
		}
	}
}


void QueryHandler::handleUses(string &firstAtt, string &secondAtt, vector<int> &useVec, vector<string> &uvarVec) {
	UseTable* useTab = PKB::getUseTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "variable") {
			useVec.push_back(-1);
		}
		else {
			useVec = useTab->getUser(secondAtt);
		}
	}
	else {
		if (isInt(firstAtt)) {
			uvarVec = useTab->getUsed(stoi(firstAtt));
		}
	}
}

void QueryHandler::handleParent(string &firstAtt, string &secondAtt, vector<int> &parVec)
{
	ParentTable* parTab = PKB::getParentTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
			parVec.push_back(-1);
		}
		if (isInt(secondAtt)) {
			parVec.push_back(parTab->getParent(stoi(secondAtt)));
		}
	}
	else {
		if (isInt(firstAtt)) {
			parVec = parTab->getChild(stoi(secondAtt));
		}
	}
}

void QueryHandler::handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &mvarVec)
{
	ModifyTable* modTab = PKB::getModifyTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "variable") {
			modVec.push_back(-1);
		}
		else {
			modVec = modTab->getModifier(secondAtt);
		}
	}
	else {
		if (isInt(firstAtt)) {
			mvarVec.push_back(modTab->getModified(stoi(firstAtt)));
		}
	}
}

int QueryHandler::handleFollows(string &firstAtt, string &secondAtt) {
	FollowTable* folTab = PKB::getFollowTable();
	int ans;
	//Case 1st: n/a
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		//Case 2nd: n/a
		if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
			ans = -1;
		}
		//Case 2nd: 1, 2...
		if (isInt(secondAtt)) {
			ans = folTab->getPrev(stoi(secondAtt));
		}
	}
	//Case 1st: 1, 2
	else {
		if (isInt(firstAtt)) {
			ans = folTab->getNext(stoi(firstAtt));
		}
	}
	return ans;
}

string QueryHandler::handleSelect(QueryTree * query, PreResultNode * &result)
{
	if (query->getResult() != NULL) {
		result = query->getResult();
		string rs = result->getResult();
		return getSymMean(rs);
	}
}


bool QueryHandler::isInt(string &secondAtt)
{
	try {
		int number = stoi(secondAtt);
		return true;
	}
	catch (exception e) {
		return false;
	}
}


string QueryHandler::getSymMean(string sym) {
	for (vector<int>::size_type i = 0; i != symTable.size(); i++) {
		vector<string> current = symTable[i];
		if (find(current.begin(), current.end(), sym) != current.end()) {
			return current[0];
		}
	}
	return "";
}
//Implement intersection method (case pair (n1, v1) and select v or n
vector<int> QueryHandler::intersection(vector<int> vec1, vector<int> vec2) {
	vector<int> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		int current = vec1[i];
		if (find(vec2.begin(), vec2.end(), current) != vec2.end()) {
			ansVec.push_back(current);
		}
	}
	return ansVec;
}

vector<string> QueryHandler::intersection(vector<string> vec1, vector<string> vec2) {
	vector<string> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		string current = vec1[i];
		if (find(vec2.begin(), vec2.end(), current) != vec2.end()) {
			ansVec.push_back(current);
		}
	}
	return ansVec;
}

vector<string> QueryHandler::intersection(vector<string> vec1, vector<pair<int, string>> vec2) {
	vector<string> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		string current = vec1[i];
		for (size_t j = 0; j != (sizeof vec2); j++) {
			if (vec2[j].second == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}

vector<int> QueryHandler::intersection(vector<int> vec1, vector<pair<int, string>> vec2) {
	vector<int> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		int current = vec1[i];
		for (size_t j = 0; j != (sizeof vec2); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}

vector<pair<int, string>> QueryHandler::intersection(vector<pair<int, string>> vec1, vector<pair<int, string>> vec2) {
	vector<pair<int, string>> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		pair<int, string> current = vec1[i];
		for (size_t j = 0; j != (sizeof vec2); j++) {
			if (vec2[j] == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}
