#include <string>
#include <vector>
#include <algorithm>

#include "../PKB.h"
#include "../QueryProcessor/QueryHandler.h"
#include "../Preprocessor/QueryTree.h"
#include "../FollowTable.h"
#include "../ModifyTable.h"
#include "../ParentTable.h"


using namespace std;
QueryHandler::QueryHandler() {};
QueryHandler::~QueryHandler() {};

bool QueryHandler::queryRec(QueryTree* query) {

	//check validity
	if (query->getValidity == false)
		return false;

	//Initiate nodes
	PreResultNode* result;
	PreSuchThatNode* suchThat;
	PrePatternNode* pattern;
	if (query->getSymbolTable != NULL) {
		symTable = query->getSymbolTable;
	}
	else {
		return false;
	}

	//Handle select
	string selType = handleSelect(query, result);

	//Handle suchThat
	if (query->getSuchThat != NULL) {
		suchThat = query->getSuchThat();
		string syn = suchThat->getSynonym();
		string firstAtt = suchThat->getFirstAttr();
		string secondAtt = suchThat->getSecondAttr();

		//Handle follows
		vector<int> folVec;
		if (syn == "follows") {
			folVec = handleFollows(firstAtt, secondAtt);
			if (folVec.front() == -1) {
				FollowTable* FollowTable = PKB::getFollowTable();
				vector<FollowEntry_t> folTab = FollowTable->getTable();
			}
		}

		if (syn == "follows*") {
			folVec = getFollowsS(firstAtt, secondAtt);
		}

		//Handle modifies
		vector<string> varVec;
		vector<int> modVec;
		if (syn == "modifies") {
			handleModifies(firstAtt, secondAtt, modVec, varVec);
			if (modVec.front() == -1) {
				ModifyTable* ModifyTable = PKB::getModifyTable();
				vector<ModifyEntry_t> modTab = ModifyTable->getTable();
			}
		}

		//Handle parent
		if (syn == "parent") {
			ParentTable* folTab = PKB::getParentTable();
			if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
				if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
					ansVec.push_back(-1);
				}
				if (isInt(secondAtt)) {
					ansVec.push_back(folTab->getParent(stoi(secondAtt)));
				}
			}
			else {
				if (isInt(secondAtt)) {
					ansVec.push_back(folTab->getChild(stoi(secondAtt)));
				}
			}
		}

		if (syn == "parent*") {
			ansVec = getParentS(firstAtt, secondAtt);
		}

		//Handle uses


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
		return ansVec;
	}

	//Handle pattern
	vector <int> PatVec = handlePattern(query, pattern);
}

void QueryHandler::handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &varVec)
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
			varVec.push_back(modTab->getModified(stoi(firstAtt)));
		}
	}
}

vector<int> QueryHandler::handleFollows(string &firstAtt, string &secondAtt) {
	FollowTable* folTab = PKB::getFollowTable();
	vector<int> ansVec;
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
			ansVec.push_back(-1);
		}
		if (isInt(secondAtt)) {
			ansVec.push_back(folTab->getPrev(stoi(secondAtt)));
		}
	}
	else {
		if (isInt(secondAtt)) {
			ansVec.push_back(folTab->getPrev(stoi(secondAtt)));
		}
	}
	return ansVec;
}

string QueryHandler::handleSelect(QueryTree * query, PreResultNode * &result)
{
	if (query->getResult != NULL) {
		result = query->getResult();
		string rs = result->getResult;
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

vector<int> QueryHandler::handlePattern(QueryTree * query, PrePatternNode * &pattern)
{
	if (query->getPattern != NULL) {
		pattern = query->getPattern();
		vector<int> PTVec = handlePT(pattern);
		pair<string, string> pat = pattern->getSynonym();
		string type = pat.first;
		string syn = pat.second;
		string firstAtt = pattern->getFirstAttr();
		string secondAtt = pattern->getSecondAttr();

		handlePType(type, firstAtt, secondAtt);
		if (type == "BOOLEAN") {
			handlePType(type, firstAtt, secondAtt);
		}
	}
}

vector<int> QueryHandler::handlePType(string &type, string &firstAtt, string &secondAtt)
{
	vector<int> ansVec;
	if (type == "assign") {
		ansVec = getAssign(firstAtt, secondAtt);
	}
	if (type == "while") {
		ansVec = getWhile(firstAtt, secondAtt);
	}
	if (type == "if") {
		ansVec = getIf(firstAtt, secondAtt);
	}
	return ansVec;
}

string QueryHandler::getSymMean(string sym) {
	for (vector<int>::size_type i = 0; i != symTable.size(); i++) {
		vector<string> current = symTable[i];
		if (find(current.begin(), current.end(), sym) != current.end()) {
			return current[0];
		}
	}
}