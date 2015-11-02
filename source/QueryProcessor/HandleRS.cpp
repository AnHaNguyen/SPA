#include "../QueryProcessor/HandleRS.h"
HandleRS::HandleRS() {}
HandleRS::~HandleRS() {}
HUtility utility = HUtility();

string HandleRS::handleSelect(QueryTree * query, PreResultNode * &result) {
	if (query->getResult() != NULL) {
		result = query->getResult();
		string rs = result->getResult().getSynonym();
		return utility.getSymMean(rs);
	}
	else return "";
}

//Check if getPrev(a) or getNext(a) has a = assign
bool HandleRS::folAss(string att, string firstAtt, vector<pair<string, string>> folTable, int i) {
	if (utility.getSymMean(att) == "assign" && att == firstAtt) {
		if (utility.contain(utility.getAssignTable(), folTable[i].first)) {
			return true;
		}
	}
	if (utility.getSymMean(att) == "assign" && att != firstAtt) {
		if (utility.contain(utility.getAssignTable(), folTable[i].second)) {
			return true;
		}
	}
	return false;
}

//Check conditions before result
void HandleRS::checkSS(vector<string> &vec, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	if (utility.getSymMean(firstAtt) == "prog_line" || utility.getSymMean(secondAtt) == "prog_line") {
		utility.intersectionSS(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "stmt" || utility.getSymMean(secondAtt) == "stmt") {
		utility.intersectionSS(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "assign" || utility.getSymMean(secondAtt) == "assign") {
		utility.intersectionSS(utility.getAssignTable(), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "if" || utility.getSymMean(secondAtt) == "if") {
		utility.intersectionSS(progLine->getLinesOfType("if"), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "while" || utility.getSymMean(secondAtt) == "while") {
		utility.intersectionSS(progLine->getLinesOfType("while"), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "procedure" || utility.getSymMean(secondAtt) == "procedure") {
		utility.intersectionSS(PKB::getProcTable()->getTable(), vec, 1);
	}
	if (utility.getSymMean(firstAtt) == "variable" || utility.getSymMean(secondAtt) == "variable") {
		utility.intersectionSS(PKB::getVarTable()->getTable(), vec, 1);
	}
	
}

void HandleRS::checkPSS(vector<pair<string, string>> &vec, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	if (utility.getSymMean(firstAtt) == "prog_line") {
		utility.intersectionPSS(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "prog_line") {
		utility.intersectionPSS(progLine->getLinesOfType("prog_line"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "stmt") {
		utility.intersectionPSS(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "stmt") {
		utility.intersectionPSS(progLine->getLinesOfType("prog_line"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "assign") {
		utility.intersectionPSS(utility.getAssignTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "assign") {
		utility.intersectionPSS(utility.getAssignTable(), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "if") {
		utility.intersectionPSS(progLine->getLinesOfType("if"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "if") {
		utility.intersectionPSS(progLine->getLinesOfType("if"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "while") {
		utility.intersectionPSS(progLine->getLinesOfType("while"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "while") {
		utility.intersectionPSS(progLine->getLinesOfType("while"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "procedure") {
		utility.intersectionPSS(PKB::getProcTable()->getTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "procedure") {
		utility.intersectionPSS(PKB::getProcTable()->getTable(), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "variable") {
		utility.intersectionPSS(PKB::getVarTable()->getTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "variable") {
		utility.intersectionPSS(PKB::getVarTable()->getTable(), vec, 2);
	}
}

void HandleRS::checkPSV(vector<pair<string, vector<string>>> &vec, string firstAtt, string secondAtt) {
	ProgLine* progLine = PKB::getProgLine();
	if (utility.getSymMean(firstAtt) == "prog_line") {
		utility.intersectionPSV(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "prog_line") {
		utility.intersectionPSV(progLine->getLinesOfType("prog_line"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "stmt") {
		utility.intersectionPSV(progLine->getLinesOfType("prog_line"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "stmt") {
		utility.intersectionPSV(progLine->getLinesOfType("prog_line"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "assign") {
		utility.intersectionPSV(utility.getAssignTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "assign") {
		utility.intersectionPSV(utility.getAssignTable(), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "if") {
		utility.intersectionPSV(progLine->getLinesOfType("if"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "if") {
		utility.intersectionPSV(progLine->getLinesOfType("if"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "while") {
		utility.intersectionPSV(progLine->getLinesOfType("while"), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "while") {
		utility.intersectionPSV(progLine->getLinesOfType("while"), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "procedure") {
		utility.intersectionPSV(PKB::getProcTable()->getTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "procedure") {
		utility.intersectionPSV(PKB::getProcTable()->getTable(), vec, 2);
	}
	if (utility.getSymMean(firstAtt) == "variable") {
		utility.intersectionPSV(PKB::getVarTable()->getTable(), vec, 1);
	}
	if (utility.getSymMean(secondAtt) == "variable") {
		utility.intersectionPSV(PKB::getVarTable()->getTable(), vec, 2);
	}
}

void HandleRS::rmEString(vector<string> vec) {
	if (vec.size() > 0) {
		for (size_t i = 0; i < vec.size(); i++) {
			if (vec[i] == "" || vec[i] == "na") {
				vec.erase(vec.begin() + i);
			}
		}
	}
}