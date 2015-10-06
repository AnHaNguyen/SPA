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
#include "../ProgLine.h"



QueryHandler::QueryHandler() {}
QueryHandler::~QueryHandler() {}


vector<string> QueryHandler::queryRec(QueryTree* query) {

	//Initiate global values
	PreResultNode* result;
	PreSuchThatNode* suchThat;
	PrePatternNode* pattern;
	string stFirst;
	string stSecond;
	string ptFirst;
	string ptSecond;

	//Return check
	vector<int> PTCheck;
	vector<int> STCheck;
	vector<string> final;

	//check validity
	if (query->getValidity() == false)
		return final;
	if (query->getSymbolTable().size() != 0) {
		symTable = query->getSymbolTable();
	}
	else {
		return final;
	}

	//Handle select
	string selType = handleSelect(query, result);

	//Handle suchThat
	vector<string> folVec;
	vector<pair<string, string>>  folTable;

	vector<string> mvarVec;
	vector<string> modVec;
	vector<pair<string, vector<string>>>  modTable;

	vector<string> parVec;
	vector<string>  parTable;
	vector<string> nestTable;

	vector<string> uvarVec;
	vector<string> useVec;
	vector<string> userTable;
	vector<string> usedTable;


	if (query->getSuchThat()->getSynonym() != "") {
		suchThat = query->getSuchThat();
		string syn = suchThat->getSynonym();
		stFirst = suchThat->getFirstAttr();
		stSecond = suchThat->getSecondAttr();
		if (stFirst == stSecond&&stFirst != "_") {
			return final;
		}

		//Handle follows
		if (syn == "Follows") {
			folVec.push_back(handleFollows(stFirst, stSecond));
			if (folVec.size() > 0 && folVec.front() == "all") {
				getFollowTable(folTable);
			}
		}

		//Handle follows*
		if (syn == "Follows*") {
			folVec.push_back(handleFollows(stFirst, stSecond));
			queue<string> folQ;
			folQ.push(folVec[0]);
			if (folVec.size() > 0 && folVec.front() == "all") {
				getFollowTable(folTable);
			}
			else {
				while (!folQ.empty()) {
					string temp = folQ.front();
					folQ.pop();
					string nextFol;
					if (isInt(stFirst)) {
						nextFol = handleFollows(temp, stSecond);
						if (nextFol != "na") {
							folQ.push(nextFol);
							folVec.push_back(nextFol);
						}
					}
					else {
						nextFol = handleFollows(stFirst, temp);
						if (nextFol != "na") {
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
			handleModifies(stFirst, stSecond, modVec, mvarVec);
			if (modVec.size() > 0 && modVec.front() == "all") {
				getModifyTable(modTab, modTable);
			}
		}

		//Handle parent
		if (syn == "Parent") {
			handleParent(stFirst, stSecond, parVec);
			if (parVec.size() > 0 && parVec.front() == "all") {
				getParentTable(result, stFirst, parTable, stSecond, nestTable);
			}
		}

		//Handle parent*
		if (syn == "Parent*") {
			handleParent(stFirst, stSecond, parVec);
			queue<string> parQ;
			if (parVec.size() > 0 && parVec.front() == "all") {
				getParentTable(result, stFirst, parTable, stSecond, nestTable);
			}
			else {
				parQ.push(parVec.front());
				while (!parQ.empty()) {
					string temp = parQ.front();
					int oldSize = parVec.size();
					parQ.pop();
					if (isInt(stFirst)) {
						handleParent(temp, stSecond, parVec);
						if (oldSize < parVec.size()) {
							for (int i = oldSize; i < parVec.size(); i++) {
								parQ.push(parVec[i]);
							}
						}
					}
					else {
						handleParent(stFirst, temp, parVec);
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
			handleUses(stFirst, stSecond, useVec, uvarVec);
			if (useVec.size() > 0 && useVec.front() == "all") {
				getUseTable(useTab, userTable, usedTable);
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
		if (!folVec.empty() && folVec.front() != "na" && folVec.front() != "all") {
			STCheck[0] = 1;
		}
		if (!folTable.empty()) {
			STCheck[1] = 1;
		}
		if (!mvarVec.empty()) {
			STCheck[2] = 1;
		}
		if (!modVec.empty() && modVec.front() != "na") {
			STCheck[3] = 1;
		}
		if (!modTable.empty()) {
			STCheck[4] = 1;
		}
		if (!parVec.empty() && parVec.front() != "na" && parVec.front() != "all") {
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
		if (!useVec.empty() && useVec.front() != "na" && useVec.front() != "all") {
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
	vector<string> patVec;
	vector<string> pvarVec;
	vector<string> pconVec;


	if (query->getPattern()->getSynonym() != "") {
		pattern = query->getPattern();
		string syn = pattern->getSynonym();
		string pType = getSymMean(syn);
		ptFirst = pattern->getFirstAttr();
		ptSecond = pattern->getSecondAttr();
		if (ptFirst == ptSecond&&ptFirst != "_") {
			return final;
		}

		pair<string, bool> ptFirstX;
		pair<string, bool> ptSecondX;
		atoPair(ptFirstX, ptFirst);
		atoPair(ptSecondX, ptSecond);


		//Case 1st att = _
		if (ptFirstX.first == "") {
			//Case 2nd att = _
			if (ptSecondX.first == "_") {
				patVec = getAssignTable();
			}
			//Case 2nd att = "x123"
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "") {
				patVec = PKB::getUseTable()->getUser(ptSecondX.first);
			}
			//Case 2nd att = v
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "variable") {
				//Select type = variable
				if (selType == "variable") {
					vector<string> tempVec;
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
					vector<string> tempVec;
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
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<string> temp2 = getAssignTable();
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					vector < pair<string, vector<string>>> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
					}
				}
				else if (selType == "assign") {
					vector<string> temp2 = getAssignTable();
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1, true);
				}
			}
		}
		//Case 1st att = "x123"
		if (getSymMean(ptFirstX.first) == "") {
			//Case 2nd att = _
			if (ptSecondX.first == "_") {
				patVec = PKB::getModifyTable()->getModifier(ptFirstX.first);
			}
			//Case 2nd att = "x123"
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "") {
				vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstX.first);
				vector<string> temp2 = PKB::getUseTable()->getUser(ptSecondX.first);
				patVec = intersection(temp1, temp2);
			}
			//Case 2nd att = v
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "variable") {
				vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstX.first);
				vector<UseEntry_t> useTable = PKB::getUseTable()->getTable();
				vector<string> temp2 = toConvention(useTable, true);
				if (selType == "variable") {
					vector<string> temp3 = intersection(temp1, temp2);
					for (int i = 0; i < temp3.size(); i++) {
						for (int j = 0; j < useTable.size(); j++) {
							if (temp3[i] == useTable[j].userLine) {
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
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstX.first);
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					vector < pair<string, vector<string>>> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
					}
				}
				else if (selType == "assign") {
					vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstX.first);
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1, true);
				}
			}
		}
		//Case 1st att = v
		if (getSymMean(ptFirstX.first) == "variable") {
			//Case 2nd att = _
			if (ptSecondX.first == "_") {
				if (selType == "variable") {
					vector<string> temp1 = getAssignTable();
					for (int i = 0; i < temp1.size(); i++) {
						pvarVec = PKB::getModifyTable()->getModified(temp1[i]);
					}
				}
				else {
					patVec = getAssignTable();
				}
			}
			//Case 2nd att = "x123"
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "") {
				if (selType == "variable") {
					vector<string> temp1 = getAssignTable();
					vector<string> temp2 = PKB::getUseTable()->getUser(ptSecondX.first);
					vector<string> temp3 = intersection(temp1, temp2);
					for (int i = 0; i < temp3.size(); i++) {
						pvarVec = PKB::getModifyTable()->getModified(temp3[i]);
					}
				}
				else {
					vector<string> temp1 = getAssignTable();
					vector<string> temp2 = PKB::getUseTable()->getUser(ptSecondX.first);
					patVec = intersection(temp1, temp2);
				}
			}
			//Case 2nd att = v
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "variable") {
				if (selType == "variable") {
					vector<string> temp1 = getAssignTable();
					if (result->getResult() == ptFirst) {
						for (int i = 0; i < temp1.size(); i++) {
							pvarVec = PKB::getModifyTable()->getModified(temp1[i]);
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
			if (containSign(ptSecondX.first) == false && getSymMean(ptSecondX.first) == "constant") {
				//Seltype = constant
				if (selType == "constant") {
					vector<string> temp2 = getAssignTable();
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					vector < pair<string, vector<string>>> temp3 = intersection(temp2, temp1);
					for (int i = 0; i < temp3.size(); i++) {
						pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
					}
				}
				else if (selType == "assign") {
					vector<string> temp2 = getAssignTable();
					vector<pair<string, vector<string>>> temp1 = getConstTable();
					patVec = intersection(temp2, temp1, true);
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			PTCheck[i] = 0;
		}
		if (!patVec.empty() && patVec.front() != "na") {
			PTCheck[0] = 1;
		}
		if (!pvarVec.empty()) {
			PTCheck[1] = 1;
		}
		if (!pconVec.empty()) {
			PTCheck[2] = 1;
		}
	}
	//Check case select not belongs to each attribute
	//string rs = result->getResult();
	//if (rs != stFirst && rs != stSecond && rs != ptFirst && rs != ptSecond) {
	//	if (query->getSuchThat()->getSynonym() == "" && getPos(STCheck)==-1) {
	//		return final;
	//	}
	//	if (query->getPattern()->getSynonym() == "" && getPos(PTCheck) == -1) {
	//		return final;
	//	}
	//	if (getSymMean(rs) == "prog_line"||"stmt") {
	//		//final.push_back(PKB::)
	//	}
	//	if (getSymMean(rs) == "variable") {
	//		final = PKB::getVarTable();
	//	}
	//	if (getSymMean(rs) == "assign") {
	//		final = AST::getAssign();
	//	}
	//	if (getSymMean(rs) == "procedure") {
	//		final = PKB::getProcTable();
	//	}
	//	if (getSymMean(rs) == "while") {
	//		final = PKB::
	//	}
	//	if (getSymMean(rs) == "if") {
	//		final = PKB::
	//	}
	//	if (getSymMean(rs) == "constant") {
	//		final = PKB::getConstTable();
	//	}
	//	if (getSymMean(rs) == "while") {
	//		final = PKB::
	//	}
	//}

	//Return function
	if (getPos(STCheck) != -1) {
		switch (getPos(STCheck)) {
		case 0:
			final = folVec;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(folVec, patVec);
				break;
			}
			break;
		case 1:
			for (int i = 0; i < folTable.size(); i++) {
				final.push_back(folTable[i].first);
			}
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(patVec, folTable);
				break;
			}
			break;
		case 2:
			final = mvarVec;
			switch (getPos(PTCheck)) {
			case 1:
				final = intersection(mvarVec, pvarVec);
				break;
			}
			break;
		case 3:
			final = modVec;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(modVec, patVec);
				break;
			}
			break;
		case 4:
			if (selType == "variable") {
				for (int i = 0; i < modTable.size(); i++) {
					final.insert(final.end(), modTable[i].second.begin(), modTable[i].second.end());
				}
			}
			else {
				for (int i = 0; i < modTable.size(); i++) {
					final.push_back(modTable[i].first);
				}
			}
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(patVec, modTable, true);
				break;
			case 1:
				final = intersection(pvarVec, modTable, true);
				break;
			case 2:
				final = intersection(pconVec, modTable, true);
				break;
			}
			break;
		case 5:
			final = parVec;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(parVec, patVec);
				break;
			}
			break;
		case 6:
			final = parTable;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(parVec, patVec);
				break;
			}
			break;
		case 7:
			final = nestTable;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(nestTable, patVec);
				break;
			}
			break;
		case 8:
			final = uvarVec;
			switch (getPos(PTCheck)) {
			case 1:
				final = intersection(uvarVec, pvarVec);
				break;
			}
			break;
		case 9:
			final = nestTable;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(nestTable, patVec);
				break;
			}
			break;
		case 10:
			final = userTable;
			switch (getPos(PTCheck)) {
			case 0:
				final = intersection(userTable, patVec);
				break;
			}
			break;
		case 11:
			final = usedTable;
			switch (getPos(PTCheck)) {
			case 1:
				final = intersection(usedTable, pvarVec);
				break;
			case 2:
				final = intersection(usedTable, pconVec);
				break;
			}
			break;
		}
	}
	else {
		switch (getPos(PTCheck)) {
		case 0:
			final = patVec;
			break;
		case 1:
			final = pvarVec;
			break;
		case 2:
			final = pconVec;
			break;
		}
	}
	return final;
}

void QueryHandler::getUseTable(vector<UseEntry_t> &useTab, vector<string> &userTable, vector<string> &usedTable) {
	UseTable* UseTable = PKB::getUseTable();
	useTab = UseTable->getTable();
	userTable = toConvention(useTab, true);
	usedTable = toConvention(useTab, 2);
}

bool QueryHandler::getParentTable(PreResultNode * result, string &firstAtt, vector<string> &parTable, string &secondAtt, vector<string> &nestTable) {
	vector<ParentEntry_t> parTab;
	ParentTable* ParentTable = PKB::getParentTable();
	parTab = ParentTable->getTable();
	if (result->getResult() == firstAtt) {
		parTable = toConvention(parTab, 1);
	}
	else if (result->getResult() == secondAtt) {
		nestTable = toConvention(parTab, 2);
	}
	if (parTab.empty()) {
		return false;
	}
	return true;
}

void QueryHandler::getModifyTable(vector<ModifyEntry_t> &modTab, vector<pair<string, vector<string>>> &modTable) {
	ModifyTable* ModifyTable = PKB::getModifyTable();
	modTab = ModifyTable->getTable();
	modTable = toConvention(modTab);
}

void QueryHandler::getFollowTable(vector<pair<string, string>> &folTable) {
	vector<FollowEntry_t> folTab;
	FollowTable* FollowTable = PKB::getFollowTable();
	folTab = FollowTable->getTable();
	folTable = toConvention(folTab);
}

vector<pair<string, vector<string>>> QueryHandler::getConstTable() {
	vector<ConstEntry_t> temp1 = PKB::getConstTable()->getTable();
	vector<pair<string, vector<string>>> temp3;
	for (int i = 0; i < temp1.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = temp1[i].line;
		temp.second = temp1[i].constants;
		temp3.push_back(temp);
	}
	return temp3;
}


vector<string> QueryHandler::getAssignTable() {
	vector<string> ansVec;
	ProgLine* progLine = PKB::getProgLine();
	ansVec = progLine->getLinesOfType("assign");
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


void QueryHandler::handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec, vector<string> &uvarVec) {
	UseTable* useTab = PKB::getUseTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "variable") {
			useVec.push_back("all");
		}
		else {
			useVec = useTab->getUser(secondAtt.substr(1, secondAtt.size() - 2));
		}
	}
	else {
		if (isInt(firstAtt)) {
			uvarVec = useTab->getUsed(firstAtt);
		}
	}
}

void QueryHandler::handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec)
{
	ParentTable* parTab = PKB::getParentTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt") {
			parVec.push_back("all");
		}
		if (isInt(secondAtt)) {
			string temp = parTab->getParent(secondAtt);
			if (temp != "na") {
				parVec.push_back(temp);
			}
		}
	}
	else {
		if (isInt(firstAtt)) {
			parVec = parTab->getChild(firstAtt);
		}
	}
}

void QueryHandler::handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec, vector<string> &mvarVec)
{
	ModifyTable* modTab = PKB::getModifyTable();
	if (getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt") {
		if (getSymMean(secondAtt) == "variable" || secondAtt == "_") {
			modVec.push_back("all");
		}
		else {
			modVec = modTab->getModifier(secondAtt.substr(1, secondAtt.size() - 2));
		}
	}
	else {
		if (isInt(firstAtt)) {
			mvarVec = modTab->getModified(firstAtt);
		}
	}
}

string QueryHandler::handleFollows(string &firstAtt, string &secondAtt) {
	FollowTable* folTab = PKB::getFollowTable();
	string ans = "na";
	//Case 1st: n/a
	if (firstAtt == "_" || getSymMean(firstAtt) == "prog_line" || getSymMean(firstAtt) == "stmt" || getSymMean(firstAtt) == "assign") {
		//Case 2nd: n/a
		if (secondAtt == "_" || getSymMean(secondAtt) == "prog_line" || getSymMean(secondAtt) == "stmt" || getSymMean(firstAtt) == "assign") {
			ans = "all";
		}
		//Case 2nd: 1, 2...
		if (isInt(secondAtt)) {
			ans = folTab->getPrev(secondAtt);
		}
	}
	//Case 1st: 1, 2
	else {
		if (isInt(firstAtt)) {
			ans = folTab->getNext(firstAtt);
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

//Get synonym
string QueryHandler::getSymMean(string sym) {
	for (vector<string>::size_type i = 0; i != symTable.size(); i++) {
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
vector<pair<string, string>> QueryHandler::toConvention(vector<FollowEntry_t> table) {
	vector<pair<string, string>> ansVec;
	for (int i = 0; i < table.size(); i++) {
		pair<string, string> temp;
		temp.first = table[i].prev;
		temp.second = table[i].next;
		ansVec.push_back(temp);
	}
	return ansVec;
}
vector<pair<string, vector<string>>> QueryHandler::toConvention(vector<ModifyEntry_t>  table) {
	vector<pair<string, vector<string>>> ansVec;
	for (int i = 0; i < table.size(); i++) {
		pair<string, vector<string>> temp;
		temp.first = table[i].modifier;
		temp.second = table[i].modifiedVar;
		ansVec.push_back(temp);
	}
	return ansVec;
}
vector<string> QueryHandler::toConvention(vector<ParentEntry_t>  table, int x) {
	vector<string> ansVec;
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
vector<string> QueryHandler::toConvention(vector<UseEntry_t> table, bool x) {
	vector<string> ansVec;
	for (int i = 0; i < table.size(); i++) {
		ansVec.push_back(table[i].userLine);
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

vector<string> QueryHandler::intersection(vector<string> vec1, vector<pair<string, string>> vec2) {
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

vector < pair<string, vector<string>>> QueryHandler::intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2) {
	vector < pair<string, vector<string>>> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		string current = vec1[i];
		for (size_t j = 0; j != (sizeof vec2); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(vec2[j]);
			}
		}
	}
	return ansVec;
}

vector <string> QueryHandler::intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2, bool check) {
	vector <string> ansVec;
	for (size_t i = 0; i != (sizeof vec1); i++) {
		string current = vec1[i];
		for (size_t j = 0; j != (sizeof vec2); j++) {
			if (vec2[j].first == current) {
				ansVec.push_back(current);
			}
		}
	}
	return ansVec;
}