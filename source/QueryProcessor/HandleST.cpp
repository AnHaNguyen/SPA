#include "../QueryProcessor/HandleST.h"
HandleST::HandleST() {}
HandleST::~HandleST() {}

string HandleST::handleFollows(string &firstAtt, string &secondAtt) {
	FollowTable* folTab = PKB::getFollowTable();
	string ans = "na";
	//Case 1st: n/s
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt"
		|| HUtility().getSymMean(firstAtt) == "assign") {
		//Case 2nd: n/s
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "assign" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			ans = "all";
		}
		//Case 2nd: 1, 2...
		if (HUtility().isInt(secondAtt)) {
			if (folTab->getPrev(secondAtt) != "") {
				ans = folTab->getPrev(secondAtt);
			}
		}
	}
	//Case 1st: 1, 2
	else {
		if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
			if (folTab->getNext(firstAtt) != "") {
				ans = folTab->getNext(firstAtt);
			}
		}
		else if (folTab->isFollows(firstAtt, secondAtt)) {
			ans = "true";
		}
	}
	return ans;
}

void HandleST::handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec, vector<string> &mvarVec) {
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
		if (HUtility().isInt(firstAtt) && HUtility().getSymMean(secondAtt) != "") {
			modVec = modTab->getModified(firstAtt);
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
	if (HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt"
		|| HUtility().getSymMean(firstAtt) == "while" || HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "assign") {
		//Case 2nd: n/a/if/while
		if (HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt"
			|| HUtility().getSymMean(secondAtt) == "while" || HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "assign") {
			parVec.push_back("all");
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
	else {
		if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
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
	//Case 
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
	else {
		if (HUtility().isInt(firstAtt) && HUtility().getSymMean(secondAtt) != "") {
			useVec = useTab->getUsed(firstAtt);
		}
		else if (useTab->isUsed(firstAtt, secondAtt.substr(1, secondAtt.size() - 2))) {
			useVec.push_back("true");
		}
	}
}

vector<string> HandleST::handleNext(string &firstAtt, string &secondAtt) {
	NextTable* nextTab = PKB::getNextTable();
	vector<string> ansVec;
	//Case 1st: n/s
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "prog_line" || HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "assign"
		|| HUtility().getSymMean(firstAtt) == "if" || HUtility().getSymMean(firstAtt) == "while") {
		//Case 2nd: n/s
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "prog_line" || HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "assign"
			|| HUtility().getSymMean(secondAtt) == "if" || HUtility().getSymMean(secondAtt) == "while") {
			ansVec.push_back("all");
		}
		//Case 2nd: 1, 2...
		if (HUtility().isInt(secondAtt)) {
			ansVec = nextTab->getPrev(secondAtt);
		}
	}
	//Case 1st: 1, 2
	else {
		if (HUtility().isInt(firstAtt) && !HUtility().isInt(secondAtt)) {
			if (nextTab->getNext(firstAtt).size()) {
				if (HUtility().getSymMean(secondAtt) == "assign") {
					ansVec = HUtility().intersection(nextTab->getNext(firstAtt), HUtility().getAssignTable());
				}
				if (HUtility().getSymMean(secondAtt) == "if") {
					ansVec = HUtility().intersection(nextTab->getNext(firstAtt), PKB::getProgLine()->getLinesOfType("if"));
				}
				if (HUtility().getSymMean(secondAtt) == "while") {
					ansVec = HUtility().intersection(nextTab->getNext(firstAtt), PKB::getProgLine()->getLinesOfType("while"));
				}
				if (HUtility().getSymMean(secondAtt) != "assign" && HUtility().getSymMean(secondAtt) != "if" && HUtility().getSymMean(secondAtt) != "while") {
					ansVec = nextTab->getNext(firstAtt);
				}
			}
		}
		else if (nextTab->isNext(firstAtt, secondAtt)) {
			ansVec.push_back("true");
		}
	}
	return ansVec;
}

vector<string> HandleST::handleCalls(string firstAtt, string secondAtt) {
	CallTable* callTab = PKB::getCallTable();
	vector<string> ansVec;
	pair<string, bool> firstAttQ;
	pair<string, bool> secondAttQ;
	HUtility().checkQuotation(firstAttQ, firstAtt);
	HUtility().checkQuotation(secondAttQ, secondAtt);

	//Case 1st: _/p
	if (firstAtt == "_" || HUtility().getSymMean(firstAtt) == "procedure") {
		//Case 2nd: _/p
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "procedure") {
			ansVec.push_back("all");
		}
		//Case 2nd: ABC
		else {
			ansVec = callTab->getCallers(secondAttQ.first);
		}
	}
	//Case 1st: ABC
	else {
		//Case 2nd: _/p
		if (secondAtt == "_" || HUtility().getSymMean(secondAtt) == "procedure") {
			ansVec = callTab->getCallees(firstAttQ.first);
		}
		//Case ABC
		else if (callTab->isCall(firstAttQ.first, secondAttQ.first)) {
			ansVec.push_back("true");
		}
	}
	return ansVec;
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

