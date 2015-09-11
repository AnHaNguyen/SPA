#include <string>
#include <vector>
#include <algorithm>

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
		return false;

	//Initiate nodes
	PreResultNode* result;
	PreSuchThatNode* suchThat;
	PrePatternNode* pattern;
	if (query->getSymbolTable().size() != 0) {
		symTable = query->getSymbolTable();
	}
	else {
		return false;
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
			folVec = handleFollows(firstAtt, secondAtt);
			if (folVec.front() == -1) {
				FollowTable* FollowTable = PKB::getFollowTable();
				folTab = FollowTable->getTable();
			}
		}
		/*
		if (syn == "follows*") {
			folVec = getFollowsS(firstAtt, secondAtt);
		}
		*/

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

		/*if (syn == "parent*") {
			ansVec = getParentS(firstAtt, secondAtt);
		}*/

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

	//Handle pattern
	if (query->getPattern() != NULL) {
		pattern = query->getPattern();
		string syn = pattern->getSynonym();
		string pType = getSymMean(syn);
		string firstAtt = pattern->getFirstAttr();
		string secondAtt = pattern->getSecondAttr();

		pair<string, bool> secondAttx;
		atoPair(secondAttx, secondAtt);

		//All the cases 2nd is within _ _
		if (secondAttx.first.size() > 1 || secondAttx.second == true) {
			//Case 1st: _
			if (firstAtt.size() == 1) {
				getAssign(firstAtt, secondAttx);	//call PKB::checkAssign(string pattern, bool contains_) ->vector<int>
			}
			//Case 1st: "x"
			else {
				getAssign(firstAtt.substr(1, 1), secondAttx);
				//call PKB::checkAssign(string pattern, bool contains_) for RHS -> vector<int>
				//call PKB::getModifyTable()->getModifier(var)	for LHS ->vector<int>
			}
		}
		//2nd is _ or "x"
		else {
			//Case 2nd: _
			if (secondAttx.first == "") {
				//Case 1st: _
				if (firstAtt.size() == 1) {
					getAssignTable();	//PKB::getASTList() -> vector<AST*>
										//run loop ast->getAssign() -> vector<int> on each ast on list
				}
				//Case 1st: "x"
				else {
					getModify(firstAtt.substr(1, 1));	//PKB::getModifyTable()->getModifier(var) -> vector<int>
				}
			}
			//Case 2nd: "x"
			else {
				//Case 1st: _
				if (firstAtt.size() == 1) {
					getUsed(secondAttx.first);		//PKB::getUseTable()->getUser(var) -> vector<int>
				}
				//Case 1st: "x"
				else {
					//getModify(firstAtt.substr(1, 1)) intersect getUsed(secondAttx.first))
					//PKB::getUseTable()->getUser(var) -> vector<int>
					//PKB::getModifyTable()->getModifier(var) -> vector<int>
				}
			}
		}
	}

	vector<string> output = { "test1", "test2"};
	return output;
}
		void QueryHandler::atoPair(pair<string, bool> &Attx, string &Att) {
			Attx.first = "";
			Attx.second == false;

			if (Att.size() > 1) {
				if (Att.substr(0, 1) == "_") {
					Attx.first = Att.substr(2, Att.size() - 4);
					Attx.second = true;
				}
				else {
					Attx.first = Att.substr(1, Att.size() - 2);
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