#include "../QueryProcessor/HandleWith.h"
HandleWith::HandleWith() {}
HandleWith::~HandleWith() {}

void HandleWith::handleWith(vector<string> &withVec, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	if (HUtility().getSymMean(secondAtt) == "") {
		withVec.push_back(secondAtt);
	}
	else {
		//Case stmt#
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
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("assign"),1);
			}
			if (HUtility().getSymMean(secondAtt) == "if") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("if"),1);
			}
			if (HUtility().getSymMean(secondAtt) == "while") {
				HUtility().intersectionSS(withVec, progLine->getLinesOfType("while"),1);
			}
		}
	}
}