#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

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
	vector<int> folVec;
	vector<pair<int, int>>  folTable;

	vector<string> mvarVec;
	vector<int> modVec;
	vector<pair<int, string>>  modTable;

	vector<int> parVec;
	vector<int>  parTable;
	vector<int> nestTable;

	vector<string> uvarVec;
	vector<int> useVec;
	vector<int> userTable;
	vector<string> usedTable;


	vector<int> PTCheck;
	vector<int> STCheck;

	if (query->getSuchThat()->getSynonym() != "") {
		suchThat = query->getSuchThat();
		string syn = suchThat->getSynonym();
		string firstAtt = suchThat->getFirstAttr();
		string secondAtt = suchThat->getSecondAttr();

		//Handle follows
		vector<FollowEntry_t> folTab;
		if (syn == "Follows") {
			folVec.push_back(handleFollows(firstAtt, secondAtt));
			if (folVec.size() > 0 && folVec.front() == -2) {
				FollowTable* FollowTable = PKB::getFollowTable();
				folTab = FollowTable->getTable();
				folTable = toConvention(folTab);
			}
		}

		//Handle follows*
		if (syn == "Follows*") {
			folVec.push_back(handleFollows(firstAtt, secondAtt));
			queue<int> folQ;
			folQ.push(folVec[0]);
			if (folVec.size() > 0 && folVec.front() == -2) {
				FollowTable* FollowTable = PKB::getFollowTable();
				folTab = FollowTable->getTable();
				folTable = toConvention(folTab);
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
		vector<ModifyEntry_t> modTab;
		if (syn == "Modifies") {
			handleModifies(firstAtt, secondAtt, modVec, mvarVec);
			if (modVec.size() > 0 && modVec.front() == -2) {
				ModifyTable* ModifyTable = PKB::getModifyTable();
				modTab = ModifyTable->getTable();
				modTable = toConvention(modTab);
			}
		}

		//Handle parent
		vector<ParentEntry_t> parTab;
		if (syn == "Parent") {
			handleParent(firstAtt, secondAtt, parVec);
			if (parVec.size() > 0 && parVec.front() == -2) {
				ParentTable* ParentTable = PKB::getParentTable();
				parTab = ParentTable->getTable();
				if (result->getResult() == firstAtt) {
					parTable = toConvention(parTab, 1);
				}
				else if (result->getResult() == secondAtt) {
					nestTable = toConvention(parTab, 2);
				}
			}
		}

		//Handle parent*
		if (syn == "Parent*") {
			handleParent(firstAtt, secondAtt, parVec);
			queue<int> parQ;

			if (parVec.size() > 0 && parVec.front() == -2) {
				ParentTable* ParentTable = PKB::getParentTable();
				parTab = ParentTable->getTable();
				if (result->getResult() == firstAtt) {
					parTable = toConvention(parTab, 1);
				}
				else if (result->getResult() == secondAtt) {
					nestTable = toConvention(parTab, 2);
				}
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
		vector<UseEntry_t> useTab;
		if (syn == "Uses") {
			handleUses(firstAtt, secondAtt, useVec, uvarVec);
			if (useVec.size() > 0 && useVec.front() == -2) {
				UseTable* UseTable = PKB::getUseTable();
				useTab = UseTable->getTable();
				userTable = toConvention(useTab, true);
				usedTable = toConvention(useTab, 2);
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

		for (int i = 0; i < 12; i++) {
			STCheck.push_back(0);
		}
		if (!folVec.empty() && folVec.front() != -1 && folVec.front() != -2) {
			STCheck[0] = 1;
		}
		if (!folTable.empty()) {
			STCheck[1] = 1;
		}
		if (!mvarVec.empty()) {
			STCheck[2] = 1;
		}
		if (!modVec.empty() && modVec.front() != -1) {
			STCheck[3] = 1;
		}
		if (!modTable.empty()) {
			STCheck[4] = 1;
		}
		if (!parVec.empty() && parVec.front() != -1 && parVec.front() != -2) {
			STCheck[5] = 1;
		}
		if (!parTable.empty()) {
			STCheck[6] = 1;
		}
		if (!parTable.empty()) {
			STCheck[7] = 1;
		}
		if (!uvarVec.empty()) {
			STCheck[8] = 1;
		}
		if (!useVec.empty() && useVec.front() != -1 && useVec.front() != -2) {
			STCheck[9] = 1;
		}
		if (!userTable.empty()) {
			STCheck[10] = 1;
		}
		if (!usedTable.empty()) {
			STCheck[11] = 1;
		}
	}
	//Handle pattern (this iteration only assign pattern)
	vector<int> patVec;
	vector<string> pvarVec;
	vector<string> pconVec;


	if (query->getPattern()->getSynonym() != "") {
		pattern = query->getPattern();
		string syn = pattern->getSynonym();
		string pType = getSymMean(syn);
		string firstAtt = pattern->getFirstAttr();
		string secondAtt = pattern->getSecondAttr();

		pair<string, bool> firstAttx;
		pair<string, bool> secondAttx;
		atoPair(firstAttx, firstAtt);
		atoPair(secondAttx, secondAtt);


		//Case 1st att = _
		if (firstAttx.first == "") {
			//Case 2nd att = _
			if (secondAttx.first == "_") {
				patVec = getAssignTable();
			}
			//Case 2nd att = "x123"
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "") {
				patVec = PKB::getUseTable()->getUser(secondAttx.first);
			}
			//Case 2nd att = v
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "variable") {
				//Select type = variable
				if (selType == "variable") {
					vector<int> tempVec;
					tempVec = getAssignTable();

					for (int i = 0; i < tempVec.size(); i++) {
						vector<string> current = PKB::getUseTable()->getUsed(tempVec[i]);
						for (int j = 0; j < current.size(); j++) {
							pvarVec.push_back(current[j]);
						}
					}
				}
				//Select type = assign
				else if (selType == "assign") {
					vector<int> tempVec;
					tempVec = getAssignTable();
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
			//Case 2nd att = c
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<int> temp2 = getAssignTable();
					vector<pair<int, string>> temp1 = getConstTable();
					vector<int> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						for (int j = 0; j < temp3.size(); j++) {
							if (temp3[i] == temp1[j].first) {
								pconVec.push_back(temp1[j].second);
							}
						}
					}
				}
				else if (selType == "assign") {
					vector<int> temp2 = getAssignTable();
					vector<pair<int, string>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1);
				}
			}
		}
		//Case 1st att = "x123"
		if (getSymMean(firstAttx.first) == "") {
			//Case 2nd att = _
			if (secondAttx.first == "_") {
				patVec = PKB::getModifyTable()->getModifier(firstAttx.first);
			}
			//Case 2nd att = "x123"
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "") {
				vector<int> temp1 = PKB::getModifyTable()->getModifier(firstAttx.first);
				vector<int> temp2 = PKB::getUseTable()->getUser(secondAttx.first);
				patVec = intersection(temp1, temp2);
			}
			//Case 2nd att = v
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "variable") {
				vector<int> temp1 = PKB::getModifyTable()->getModifier(firstAttx.first);
				vector<UseEntry_t> useTable = PKB::getUseTable()->getTable();
				vector<int> temp2 = toConvention(useTable, true);
				if (selType == "variable") {
					vector<int> temp3 = intersection(temp1, temp2);
					for (int i = 0; i < temp3.size(); i++) {
						for (int j = 0; j < useTable.size(); j++) {
							if (temp3[i] == useTable[j].lineNo) {
								for (int k = 0; k < useTable[j].usedVar.size(); k++) {
									pvarVec.push_back(useTable[j].usedVar[k]);
								}
							}
						}
					}
				}
				else if (selType == "assign") {
					patVec = intersection(temp1, temp2);
				}
			}
			//Case 2nd att = c
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<int> temp2 = PKB::getModifyTable()->getModifier(firstAttx.first);
					vector<pair<int, string>> temp1 = getConstTable();
					vector<int> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						for (int j = 0; j < temp1.size(); j++) {
							if (temp1[j].first == temp3[i]) {
								pvarVec.push_back(temp1[j].second);
							}
						}
					}
				}
				else if (selType == "assign") {
					vector<int> temp2 = PKB::getModifyTable()->getModifier(firstAttx.first);
					vector<pair<int, string>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1);
				}
			}
		}
		//Case 1st att = v
		if (getSymMean(firstAttx.first) == "variable") {
			//Case 2nd att = _
			if (secondAttx.first == "_") {
				if (selType == "variable") {
					vector<int> temp1 = getAssignTable();
					for (int i = 0; i < temp1.size(); i++) {
						pvarVec.push_back(PKB::getModifyTable()->getModified(temp1[i]));
					}
				}
				else {
					patVec = getAssignTable();
				}
			}
			//Case 2nd att = "x123"
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "") {
				if (selType == "variable") {
					vector<int> temp1 = getAssignTable();
					vector<int> temp2 = PKB::getUseTable()->getUser(secondAttx.first);
					vector<int> temp3 = intersection(temp1, temp2);
					for (int i = 0; i < temp3.size(); i++) {
						pvarVec.push_back(PKB::getModifyTable()->getModified(temp3[i]));
					}
				}
				else {
					vector<int> temp1 = getAssignTable();
					vector<int> temp2 = PKB::getUseTable()->getUser(secondAttx.first);
					patVec = intersection(temp1, temp2);
				}
			}
			//Case 2nd att = v
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "variable") {
				if (selType == "variable") {
					vector<int> temp1 = getAssignTable();
					if (result->getResult() == firstAtt) {
						for (int i = 0; i < temp1.size(); i++) {
							pvarVec.push_back(PKB::getModifyTable()->getModified(temp1[i]));
						}
					}
					else {
						for (int i = 0; i < temp1.size(); i++) {
							for (int j = 0; j < PKB::getUseTable()->getUsed(temp1[i]).size(); j++) {
								pvarVec.push_back(PKB::getUseTable()->getUsed(temp1[i])[j]);
							}
						}
					}
				}
			}
			//Case 2nd att = c
			if (containSign(secondAttx.first) == false && getSymMean(secondAttx.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<int> temp2 = getAssignTable();
					vector<pair<int, string>> temp1 = getConstTable();
					vector<int> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						for (int j = 0; j < temp1.size(); j++) {
							if (temp1[j].first == temp3[i]) {
								pvarVec.push_back(temp1[j].second);
							}
						}
					}
				}
				else if (selType == "assign") {
					vector<int> temp2 = getAssignTable();
					vector<pair<int, string>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1);
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			PTCheck[i] = 0;
		}
		if (!patVec.empty() && patVec.front() != -1) {
			PTCheck[0] = 1;
		}
		if (!pvarVec.empty()) {
			PTCheck[1] = 1;
		}
		if (!pconVec.empty()) {
			PTCheck[2] = 1;
		}
	}

	vector<string> final1;
	vector<int> final2;
	if (getPos(STCheck) != -1) {
		switch (getPos(STCheck)) {
		case 0:
			final2 = folVec;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(folVec, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 1:
			for (int i = 0; i < folTable.size(); i++) {
				final2.push_back(folTable[i].first);
			}
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(patVec, folTable);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 2:
			final1 = mvarVec;
			switch (getPos(PTCheck)) {
			case 0:
				break;
			case 1:
				final1 = intersection(mvarVec, pvarVec);
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 3:
			final2 = modVec;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(modVec, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 4:
			if (selType == "variable") {
				for (int i = 0; i < modTable.size(); i++) {
					final1.push_back(modTable[i].second);
				}
			}
			else {
				for (int i = 0; i < modTable.size(); i++) {
					final2.push_back(modTable[i].first);
				}
			}
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(patVec, modTable);
				break;
			case 1:
				final1 = intersection(pvarVec, modTable);
				break;
			case 2:
				final1 = intersection(pconVec, modTable);
				break;
			default:
				break;
			}
			{
			default:
				break;
			}
			break;
		case 5:
			final2 = parVec;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(parVec, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 6:
			final2 = parTable;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(parVec, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 7:
			final2 = nestTable;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(nestTable, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 8:
			final1 = uvarVec;
			switch (getPos(PTCheck)) {
			case 0:
				break;
			case 1:
				final1 = intersection(uvarVec, pvarVec);
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 9:
			final2 = nestTable;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(nestTable, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 10:
			final2 = userTable;
			switch (getPos(PTCheck)) {
			case 0:
				final2 = intersection(userTable, patVec);
				break;
			case 1:
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 11:
			final1 = usedTable;
			switch (getPos(PTCheck)) {
			case 0:
				break;
			case 1:
				final1 = intersection(usedTable, pvarVec);
				break;
			case 2:
				final1 = intersection(usedTable, pconVec);
				break;
			default:
				break;
			}
			break;
		}

	}
	else {
		switch (getPos(PTCheck)) {
		case 0:
			final2 = patVec;
			break;
		case 1:
			final1 = pvarVec;
			break;
		case 2:
			final1 = pconVec;
			break;
		default:
			break;
		}
	}
	if (!final2.empty()) {
		for (int i = 0; i < final2.size(); i++) {
			final1.push_back(to_string(final2[i]));
		}
	}
	return final1;
}

vector<pair<int, string>> QueryHandler::getConstTable()
{
	vector<ConstEntry_t> temp1 = PKB::getConstTable()->getTable();
	vector<pair<int, string>> temp3;
	for (int i = 0; i < temp1.size(); i++) {
		pair<int, string> temp;
		temp.first = temp1[i].line;
		temp.second = temp1[i].constant;
		temp3.push_back(temp);
	}
	return temp3;
}

vector<int> QueryHandler::getAssignTable() {
	vector<int> ansVec;
	vector<AST*> ast = PKB::getASTList();
	for (int i = 0; i < ast.size(); i++) {
		vector<int> current = ast[i]->getAssign();
		for (int j = 0; j < current.size(); j++) {
			ansVec.push_back(current[j]);
		}
	}
	return ansVec;
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
			useVec.push_back(-2);
		}
		else {
			useVec = useTab->getUser(secondAtt.substr(1, secondAtt.size() - 2));
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
			parVec.push_back(-2);
		}
		if (isInt(secondAtt)) {
			int temp = parTab->getParent(stoi(secondAtt));
			if (temp != -1) {
				parVec.push_back(temp);
			}
		}
	}
	else {
		if (isInt(firstAtt)) {
			parVec = parTab->getChild(stoi(firstAtt));
		}
	}
}

void QueryHandler::handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &mvarVec)
{
	ModifyTable* modTab = PKB::getModifyTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "variable") {
			modVec.push_back(-2);
		}
		else {
			modVec = modTab->getModifier(secondAtt.substr(1, secondAtt.size() - 2));
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
	int ans = -1;
	//Case 1st: n/a
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt" || getSymMean(firstAtt) == "assign") {
		//Case 2nd: n/a
		if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt" || getSymMean(firstAtt) == "assign") {
			ans = -2;
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

int QueryHandler::getPos(vector<int> intVec) {
	for (int i = 0; i < intVec.size(); i++) {
		if (intVec[i] == 1) {
			return i;
		}
	}
	return -1;
}
//To convetion
vector<pair<int, int>> QueryHandler::toConvention(vector<FollowEntry_t> table) {
	vector<pair<int, int>> ansVec;
	for (int i = 0; i < table.size(); i++) {
		pair<int, int> temp;
		temp.first = table[i].prev;
		temp.second = table[i].next;
		ansVec.push_back(temp);
	}
	return ansVec;
}
vector<pair<int, string>> QueryHandler::toConvention(vector<ModifyEntry_t>  table) {
	vector<pair<int, string>> ansVec;
	for (int i = 0; i < table.size(); i++) {
		pair<int, string> temp;
		temp.first = table[i].lineNo;
		temp.second = table[i].modifiedVar;
		ansVec.push_back(temp);
	}
	return ansVec;
}
vector<int> QueryHandler::toConvention(vector<ParentEntry_t>  table, int x) {
	vector<int> ansVec;
	if (x == 1) {
		for (int i = 0; i < table.size(); i++) {
			ansVec.push_back(table[i].lineNo);
		}
	}
	else {
		for (int i = 0; i < table.size(); i++) {
			for (int j = 0; j < table[i].child.size(); j++) {
				ansVec.push_back(table[i].child[j]);
			}
		}
	}
	return ansVec;
}
vector<int> QueryHandler::toConvention(vector<UseEntry_t> table, bool x) {
	vector<int> ansVec;
	for (int i = 0; i < table.size(); i++) {
		ansVec.push_back(table[i].lineNo);
	}
	return ansVec;
}
vector<string> QueryHandler::toConvention(vector<UseEntry_t> table, int x) {
	vector<string> ansVec;
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].usedVar.size(); j++) {
			ansVec.push_back(table[i].usedVar[j]);
		}
	}
	return ansVec;
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
vector<int> QueryHandler::intersection(vector<int> vec1, vector<pair<int, int>> vec2) {
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
