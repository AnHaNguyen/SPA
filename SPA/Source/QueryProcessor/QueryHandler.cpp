#include <string>
#include <vector>
#include "../QueryProcessor/QueryHandler.h"
#include "../Preprocessor/QueryTree.h"

QueryHandler::QueryHandler() {};
QueryHandler::~QueryHandler() {};

bool QueryHandler::queryRec(QueryTree* query) {

	//check validity
	if (query->getValidity != false)
		return false;

	//Initiate nodes
	PreResultNode* result;
	PreSuchThatNode* suchThat;
	PrePatternNode* pattern;

	//Handle select
	if (query->getResult != NULL) {
		result = query->getResult();
		string rs = result->getResult;
		if (rs == "BOOLEAN") {

		}
		if (rs == "variable") {

		}
	}
	//Handle suchThat
	if (query->getSuchThat != NULL) {
		suchThat = query->getSuchThat();
		vector<int> STVec = handleST(suchThat);
	}

	//Handle pattern
	if (query->getPattern != NULL) {
		pattern = query->getPattern();
		vector<int> PTVec = handlePT(pattern);
	}
}

void QueryHandler::handlePT(PrePatternNode * pattern)
{
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

vector<int> QueryHandler::handlePType(std::string &type, std::string &firstAtt, std::string &secondAtt)
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

vector<int> QueryHandler::handleST(PreSuchThatNode * suchThat)
{
	string syn = suchThat->getSynonym();
	string firstAtt = suchThat->getFirstAttr();
	string secondAtt = suchThat->getSecondAttr();
	vector<int> ansVec;

	if (syn == "follows") {
		ansVec = getFollows(firstAtt, secondAtt);
	}
	if (syn == "follows*") {
		ansVec = getFollowsS(firstAtt, secondAtt);
	}
	if (syn == "modifies") {
		ansVec = getModifies(firstAtt, secondAtt);
	}
	if (syn == "parent") {
		ansVec = getParent(firstAtt, secondAtt);
	}
	if (syn == "parent*") {
		ansVec = getParentS(firstAtt, secondAtt);
	}
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
	}
	return ansVec;
}
