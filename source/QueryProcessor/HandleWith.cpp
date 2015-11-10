#include "../QueryProcessor/HandleWith.h"
HandleWith::HandleWith() {}
HandleWith::~HandleWith() {}

void HandleWith::handleWith(vector<string> &withVec, vector<pair<string, vector<string>>> &withCall, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	pair<string, bool> secondAttQ;
	HUtility().checkQuotation(secondAttQ, secondAtt);
	if (HUtility().getSymMean(secondAtt) == "" && HUtility().getSymMean(firstAtt) != "call") {
		if (HUtility().isInt(secondAtt)) {
			withVec.push_back(secondAtt);
		}
		else {
			withVec.push_back(secondAttQ.first);
		}
	}
	else {
		//Case stmt
		if (HUtility().getSymMean(firstAtt) == "stmt" || HUtility().getSymMean(firstAtt) == "prog_line") {
			if (HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "prog_line") {
				withVec = progLine->getLinesOfType("prog_line");
			}
			if (HUtility().getSymMean(secondAtt) == "assign") {
				withVec = progLine->getLinesOfType("assign");
			}
			if (HUtility().getSymMean(secondAtt) == "if") {
				withVec = progLine->getLinesOfType("if");
			}
			if (HUtility().getSymMean(secondAtt) == "while") {
				withVec = progLine->getLinesOfType("while");
			}
			if (HUtility().getSymMean(secondAtt) == "constant") {
				withVec = progLine->getLinesOfType("stmt");
				vector<string> total;
				vector<pair<string, vector<string>>> constTable = HUtility().getConstTable();
				for (size_t i = 0; i < constTable.size(); i++) {
					for (size_t j = 0; j < constTable[i].second.size(); j++) {
						if (!HUtility().contain(total, constTable[i].second[j])) {
							total.push_back(constTable[i].second[j]);
						}
					}
				}
				HUtility().intersectionSS(withVec, total, 1);
			}
		}
		//Case assign
		if (HUtility().getSymMean(firstAtt) == "assign") {
			withVec = progLine->getLinesOfType("assign");
			if (HUtility().getSymMean(secondAtt) == "if") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "while") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "constant") {
				withVec = progLine->getLinesOfType("stmt");
				vector<string> total;
				vector<pair<string, vector<string>>> constTable = HUtility().getConstTable();
				for (size_t i = 0; i < constTable.size(); i++) {
					for (size_t j = 0; j < constTable[i].second.size(); j++) {
						if (!HUtility().contain(total, constTable[i].second[j])) {
							total.push_back(constTable[i].second[j]);
						}
					}
				}
				HUtility().intersectionSS(withVec, total, 1);
			}
		}

		//Case if
		if (HUtility().getSymMean(firstAtt) == "if") {
			withVec = progLine->getLinesOfType("if");
			if (HUtility().getSymMean(secondAtt) == "assign") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "while") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "constant") {
				withVec = progLine->getLinesOfType("stmt");
				vector<string> total;
				vector<pair<string, vector<string>>> constTable = HUtility().getConstTable();
				for (size_t i = 0; i < constTable.size(); i++) {
					for (size_t j = 0; j < constTable[i].second.size(); j++) {
						if (!HUtility().contain(total, constTable[i].second[j])) {
							total.push_back(constTable[i].second[j]);
						}
					}
				}
				HUtility().intersectionSS(withVec, total, 1);
			}
		}

		//Case while
		if (HUtility().getSymMean(firstAtt) == "while") {
			withVec = progLine->getLinesOfType("while");
			if (HUtility().getSymMean(secondAtt) == "assign") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "if") {
				withVec.clear();
			}
			if (HUtility().getSymMean(secondAtt) == "constant") {
				withVec = progLine->getLinesOfType("stmt");
				vector<string> total;
				vector<pair<string, vector<string>>> constTable = HUtility().getConstTable();
				for (size_t i = 0; i < constTable.size(); i++) {
					for (size_t j = 0; j < constTable[i].second.size(); j++) {
						if (!HUtility().contain(total, constTable[i].second[j])) {
							total.push_back(constTable[i].second[j]);
						}
					}
				}
				HUtility().intersectionSS(withVec, total, 1);
			}
		}

		//Case procedure
		if (HUtility().getSymMean(firstAtt) == "procedure") {
			if (HUtility().getSymMean(secondAtt) == "procedure") {
				withVec = PKB::getProcTable()->getTable();
			}
		}

		//Case variable
		if (HUtility().getSymMean(firstAtt) == "variable") {
			if (HUtility().getSymMean(secondAtt) == "variable") {
				withVec = PKB::getVarTable()->getTable();
			}
		}

		//Case constant
		if (HUtility().getSymMean(firstAtt) == "constant") {
			vector<string> total;
			vector<pair<string, vector<string>>> constTable = HUtility().getConstTable();
			for (size_t i = 0; i < constTable.size(); i++) {
				for (size_t j = 0; j < constTable[i].second.size(); j++) {
					if (!HUtility().contain(total, constTable[i].second[j])) {
						total.push_back(constTable[i].second[j]);
					}
				}
			}
			withVec = total;
			if (HUtility().getSymMean(secondAtt) == "constant") {
				withVec = total;
			}
			if (HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "prog_line") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("prog_line"), 1);
			}
			if (HUtility().getSymMean(secondAtt) == "assign") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("assign"), 1);
			}
			if (HUtility().getSymMean(secondAtt) == "if") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("if"), 1);
			}
			if (HUtility().getSymMean(secondAtt) == "while") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("while"), 1);
			}
		}

		//Case call
		if (HUtility().getSymMean(firstAtt) == "call") {
			pair<string, vector<string>> temp;
			CallLine* callLine = PKB::getCallLine();
			vector<string> callers;
		

			//Case secondAtt is not a syn
			if (HUtility().getSymMean(secondAtt) == "") {
				if (HUtility().isInt(secondAtt)) {
					temp.first = callLine->getCallee(secondAtt);
					temp.second.push_back(secondAtt);
					withCall.push_back(temp);
				}
				else {
					temp.first = secondAtt;
					callers = callLine->getCallers(secondAttQ.first);
					for (size_t i = 0; i < callers.size(); i++) {
						temp.second.push_back(callers[i]);
					}
					if (temp.second.size()>0) {
						withCall.push_back(temp);
					}
				}
			}

			//Other cases
			if (HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "prog_line") {
				vector<pair<string, string>> callTab = callLine->getTable();
				vector<int> visited;
				for (size_t i = 0; i < callTab.size(); i++) {
					if (!HUtility().contain(visited, i)) {
						temp.first = callTab[i].second;
						for (size_t j = 0; j < callTab.size(); j++) {
							if (callTab[i].second == callTab[j].second) {
								temp.second.push_back(callTab[j].first);
							}
						}
						visited.push_back(i);
					}
				}
			}
			if (HUtility().getSymMean(secondAtt) == "variable") {
				callers = progLine->getLinesOfType("call");
				vector<string> varTab = PKB::getVarTable()->getTable();
				string callee;
				for (size_t i = 0; i < callers.size(); i++) {
					callee = callLine->getCallee(callers[i]);
					temp.second = callers;
					if (HUtility().contain(varTab, callee)) {
						temp.first = callee;
						withCall.push_back(temp);
					}
				}
			}
			if (HUtility().getSymMean(secondAtt) == "procedure") {
				vector<string> procTable = PKB::getProcTable()->getTable();
				for (size_t i = 0; i < procTable.size(); i++) {
					temp.first = procTable[i];
					temp.second = callLine->getCallers(procTable[i]);
					if (temp.second.size()>0) {
						withCall.push_back(temp);
					}
				}
			}
		}
	}
}