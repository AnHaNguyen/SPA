#include "../QueryProcessor/HandleWith.h"
HandleWith::HandleWith() {}
HandleWith::~HandleWith() {}

void HandleWith::handleWith(vector<string> &withVec, vector<pair<string, string>> &withCall, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	if (HUtility().getSymMean(secondAtt) == "" || HUtility().getSymMean(firstAtt) == "call") {
		withVec.push_back(secondAtt);
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
			pair<string, string> temp;
			CallTable* callTable = PKB::getCallTable();
			vector<string> caller = progLine->getLinesOfType("call");

			//Case secondAtt is not a syn
			if (HUtility().getSymMean(secondAtt) == "") {
				if (HUtility().isInt(secondAtt)) {
				}
			}
			if (HUtility().getSymMean(secondAtt) == "stmt" || HUtility().getSymMean(secondAtt) == "prog_line") {
				withVec = progLine->getLinesOfType("call");
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("prog_line"), 1);
			}
			if (HUtility().getSymMean(secondAtt) == "variable") {
				for (size_t i = 0; i < caller.size(); i++) {
					for (size_t j = 0; j < callTable->getCallees(caller[i]).size(); j++) {
						if (!HUtility().contain(withVec, callTable->getCallees(caller[i])[j])) {
							withVec.push_back(callTable->getCallees(caller[i])[j]);
						}
					}
				}
			}
		}
	}
}