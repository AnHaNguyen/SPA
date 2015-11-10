#include "../QueryProcessor/HandleST.h"
HandleST::HandleST() {}
HandleST::~HandleST() {}

void HandleST::handleFollows(string &firstAtt, string &secondAtt, vector<string> &folVec) {
	FollowTable* folTab = PKB::getFollowTable();
	//Case 1st: n/s
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt"
		|| HUtility().getSymMean(firstAtt) == "assign" || HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "while") {
		//Case 2nd: n/s
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "assign" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			if (firstAtt != secondAtt || firstAtt == "_") {
				folVec.push_back("all");
			}
		}
		//Case 2nd: 1, 2...
		if (HUtility().isInt(secondAtt)) {
			if (folTab->getPrev(secondAtt) != "") {
				folVec.push_back(folTab->getPrev(secondAtt));
			}
		}
	}
	//Case 1st: 1, 2
	else if (HUtility().isInt(firstAtt)) {
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "assign" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			if (folTab->getNext(firstAtt) != "") {
				folVec.push_back(folTab->getNext(firstAtt));
			}
		}
		else if (folTab->isFollows(firstAtt, secondAtt)) {
			folVec.push_back("true");
		}
	}
}

void HandleST::handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec) {
	ModifyTable* modTab = PKB::getModifyTable();
	//Case 1st: all possible syms
	if (HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "assign"
		|| HUtility().getSymMean(firstAtt) == "while" || HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "procedure"
		|| HUtility().getSymMean(firstAtt) == "call") {
		if (HUtility().getSymMean(secondAtt) == "variable" || secondAtt == "_") {
			modVec.push_back("all");
		}
		else {
			modVec = modTab->getModifier(secondAtt.substr(1, secondAtt.size() - 2));
		}
	}
	//Case 1st: 1, 2
	else {
		pair<string, bool> firstAttQ;
		HUtility().checkQuotation(firstAttQ, firstAtt);
		if (HUtility().getSymMean(secondAtt) == "variable" || secondAtt == "_") {
			modVec = modTab->getModified(firstAttQ.first);
		}
		else if (modTab->isModified(firstAtt, secondAtt.substr(1, secondAtt.size() - 2))) {
			modVec.push_back("true");
		}
	}
}

void HandleST::handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec)
{
	ParentTable* parTab = PKB::getParentTable();
	//Case 1st: n/a/if/while
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt"
		|| HUtility().getSymMean(firstAtt) == "while" || HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "assign") {
		//Case 2nd: n/a/if/while
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "while" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "assign") {
			if (firstAtt != secondAtt || firstAtt == "_") {
				parVec.push_back("all");
			}
		}
		//Case 2nd: 1,2
		if (HUtility().isInt(secondAtt)) {
			string temp = parTab->getParent(secondAtt);
			if (temp != "" && !HUtility().contain(parVec, temp)) {
				parVec.push_back(temp);
			}
		}
	}
	//Case 1st: 1/2
	else if (HUtility().isInt(firstAtt)) {
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "while" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "assign") {
			vector<string> temp = parTab->getChild(firstAtt);
			for (size_t i = 0; i < temp.size(); i++) {
				if (!HUtility().contain(parVec, temp[i])) {
					parVec.push_back(temp[i]);
				}
			}
		}
		else if (parTab->isParent(firstAtt, secondAtt)) {
			parVec.push_back("true");
		}
	}
}

void HandleST::handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec) {
	UseTable* useTab = PKB::getUseTable();
	//Case 1st: n 
	if (HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "assign"
		|| HUtility().getSymMean(firstAtt) == "while" || HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "procedure"
		|| HUtility().getSymMean(firstAtt) == "call") {
		if (HUtility().getSymMean(secondAtt) == "variable" || HUtility().getSymMean(secondAtt) == "_") {
			useVec.push_back("all");
		}
		else {
			useVec = useTab->getUser(secondAtt.substr(1, secondAtt.size() - 2));
		}
	}
	//Case 1st: 123
	else {
		pair<string, bool> firstAttQ;
		HUtility().checkQuotation(firstAttQ, firstAtt);
		if (HUtility().getSymMean(secondAtt) == "variable" || HUtility().getSymMean(secondAtt) == "_") {
			useVec = useTab->getUsed(firstAttQ.first);
		}
		else if (useTab->isUsed(firstAtt, secondAtt.substr(1, secondAtt.size() - 2))) {
			useVec.push_back("true");
		}
	}
}

void HandleST::handleNext(string &firstAtt, string &secondAtt, vector<string> &nextVec) {
	NextTable* nextTab = PKB::getNextTable();
	//Case 1st: n/s
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "assign"
		|| HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "while") {
		//Case 2nd: n/s
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "assign"
			|| HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			if (firstAtt != secondAtt || firstAtt == "_") {
				nextVec.push_back("all");
			}
			//Case 2 parameters are the same
			else {
				vector<NextEntry_t> nextTable = nextTab->getTable();
				for (size_t i = 0; i < nextTable.size(); i++) {
					if (HUtility().contain(nextTable[i].nextStmts, nextTable[i].lineNo)) {
						nextVec.push_back(nextTable[i].lineNo);
					}
				}
			}
		}
		//Case 2nd: 1, 2...
		if (HUtility().isInt(secondAtt)) {
			nextVec = nextTab->getPrev(secondAtt);
		}
	}
	//Case 1st: 1, 2
	else if (HUtility().isInt(firstAtt)) {
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "assign"
			|| HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			if (nextTab->getNext(firstAtt).size()) {
				/*if (HUtility().getSymMean(secondAtt) == "assign") {
					nextVec = HUtility().intersection(nextTab->getNext(firstAtt), HUtility().getAssignTable());
				}
				if (HUtility().getSymMean(secondAtt) == "if") {
					nextVec = HUtility().intersection(nextTab->getNext(firstAtt), PKB::getProgLine()->getLinesOfType("if"));
				}
				if (HUtility().getSymMean(secondAtt) == "while") {
					nextVec = HUtility().intersection(nextTab->getNext(firstAtt), PKB::getProgLine()->getLinesOfType("while"));
				}
				if (HUtility().getSymMean(secondAtt) != "assign" && HUtility().getSymMean(secondAtt) != "if" && HUtility().getSymMean(secondAtt) != "while") {
					nextVec = nextTab->getNext(firstAtt);
				}*/
				nextVec = nextTab->getNext(firstAtt);
			}
		}
		else if (nextTab->isNext(firstAtt, secondAtt)) {
			nextVec.push_back("true");
		}
	}
}

void HandleST::handleCalls(string firstAtt, string secondAtt, vector<string> &callVec) {
	CallTable* callTab = PKB::getCallTable();
	pair<string, bool> firstAttQ;
	pair<string, bool> secondAttQ;
	HUtility().checkQuotation(firstAttQ, firstAtt);
	HUtility().checkQuotation(secondAttQ, secondAtt);

	//Case 1st: _/p
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "procedure") {
		//Case 2nd: _/p
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "procedure") {
			if (firstAtt != secondAtt || firstAtt == "_") {
				callVec.push_back("all");
			}
		}
		//Case 2nd: ABC
		else {
			callVec = callTab->getCallers(secondAttQ.first);
		}
	}
	//Case 1st: ABC
	else {
		//Case 2nd: _/p
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "procedure") {
			callVec = callTab->getCallees(firstAttQ.first);
		}
		//Case ABC
		else if (callTab->isCall(firstAttQ.first, secondAttQ.first)) {
			callVec.push_back("true");
		}
	}
}

void HandleST::handleAffect(string firstAtt, string secondAtt, vector<string> &affVec) {
	//Case firstAtt: n1 or _ (note: affects return assignment only)
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "assign") {
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "assign") {
			if (firstAtt != secondAtt || firstAtt == "_") {
				affVec.push_back("all");
			}
			else {
				vector<pair<string, vector<string>>> affTable = PKB::affect();
				for (size_t i = 0; i < affTable.size(); i++) {
					if (HUtility().contain(affTable[i].second, affTable[i].first)) {
						affVec.push_back(affTable[i].first);
					}
				}
			}
		}
		else if (HUtility().isInt(secondAtt)) {
			affVec = PKB::affect("_", secondAtt);
		}
	}
	//Case firstAtt: 123
	else if (HUtility().isInt(firstAtt)) {
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "assign") {
			affVec = PKB::affect(firstAtt, "_");
		}
		else if (HUtility().isInt(secondAtt)) {
			affVec = PKB::affect(firstAtt, secondAtt);
		}
	}
}

void HandleST::handleContain(vector<string> &conVec, vector<pair<string, string>> &conTable, string firstAtt, string secondAtt) {
	if (HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		conVec = PKB::contains(firstAtt, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		conVec = PKB::contains(firstAtt, HUtility().isInt(firstAtt), sym2, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		conVec = PKB::contains(sym1, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		conTable = PKB::contains(sym1, sym2);
	}
}

void HandleST::handleContainS(vector<string> &conVec, vector<pair<string, string>> &conTable, string firstAtt, string secondAtt) {
	if (HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		conVec = PKB::containsS(firstAtt, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		conVec = PKB::containsS(firstAtt, HUtility().isInt(firstAtt), sym2, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		conVec = PKB::containsS(sym1, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		conTable = PKB::containsS(sym1, sym2);
	}
}

void HandleST::handleSibling(vector<string> &sibVec, vector<pair<string, string>> &sibTable, string firstAtt, string secondAtt) {
	if (HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		sibVec = PKB::sibling(firstAtt, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		sibVec = PKB::sibling(firstAtt, HUtility().isInt(firstAtt), sym2, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		sibVec = PKB::sibling(sym1, HUtility().isInt(firstAtt), secondAtt, HUtility().isInt(secondAtt));
	}
	if (!HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
		string sym1 = HUtility().getSymMean(firstAtt);
		if (sym1 == "prog_line") {
			sym1 = "stmt";
		}
		string sym2 = HUtility().getSymMean(secondAtt);
		if (sym2 == "prog_line") {
			sym2 = "stmt";
		}
		sibTable = PKB::sibling(sym1, sym2);
	}
}

bool HandleST::isFollowsS(string firstAtt, string secondAtt) {
	string nextFol = PKB::getFollowTable()->getNext(firstAtt);
	if (nextFol == "" || stoi(nextFol) > stoi(secondAtt)) {
		return false;
	}
	while (nextFol != "" && stoi(nextFol) <= stoi(secondAtt)) {
		if (stoi(nextFol) == stoi(secondAtt)) {
			return true;
		}
		nextFol = PKB::getFollowTable()->getNext(nextFol);
	}
	return false;
}

bool HandleST::isParentS(string firstAtt, string secondAtt) {
	string nextPar = PKB::getParentTable()->getParent(secondAtt);
	if (nextPar == "" || stoi(nextPar) < stoi(firstAtt)) {
		return false;
	}
	while (nextPar != "" && stoi(nextPar) >= stoi(firstAtt)) {
		if (stoi(nextPar) == stoi(firstAtt)) {
			return true;
		}
		nextPar = PKB::getParentTable()->getParent(nextPar);
	}
	return false;
}