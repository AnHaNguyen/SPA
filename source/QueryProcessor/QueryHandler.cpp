#include <string>
#include <vector>
#include <algorithm>

#include "../QueryProcessor/QueryHandler.h"
#include "../Preprocessor/QueryTree.h"
#include "../FollowTable.h"

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
	vector<int> STVec = handleSuchThat(query, suchThat);

	//Handle pattern
	vector <int> PatVec = handlePattern(query, pattern);
}

string QueryHandler::handleSelect(QueryTree * query, PreResultNode * &result)
{
	if (query->getResult != NULL) {
		result = query->getResult();
		string rs = result->getResult;
		return getSymMean(rs);
	}
}


vector<int> QueryHandler::handleSuchThat(QueryTree * query, PreSuchThatNode * &suchThat, string selType) {
	if (query->getSuchThat != NULL) {
		suchThat = query->getSuchThat();
		string syn = suchThat->getSynonym();
		string firstAtt = suchThat->getFirstAttr();
		string secondAtt = suchThat->getSecondAttr();
		vector<int> ansVec;

		//Handle follows
		if (syn == "follows") {
			FollowTable folTab = FollowTable();
			if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
				if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
					vector<FollowEntry_t> folVec = folTab.getTable();
				}
				if (isInt(secondAtt)) {

				}
			}
		}
		if (syn == "follows*") {
			ansVec = getFollowsS(firstAtt, secondAtt);
		}

		//Handle modifies
		if (syn == "modifies") {
			ansVec = getModifies(firstAtt, secondAtt);
		}

		//Handle parent
		if (syn == "parent") {
			ansVec = getParent(firstAtt, secondAtt);
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