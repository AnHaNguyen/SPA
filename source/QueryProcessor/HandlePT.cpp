#include "../QueryProcessor/HandlePT.h"
HandlePT::HandlePT() {}
HandlePT::~HandlePT() {}

void HandlePT::handleAssign(string ptFirst, string ptSecond,
	vector<string> &patVec, vector<pair<string, vector<string>>> &patAsgTable) {
	pair<string, bool> ptFirstU;
	pair<string, bool> ptSecondU;
	HandlePT().checkUnderscore(ptFirstU, ptFirst);
	HandlePT().checkUnderscore(ptSecondU, ptSecond);

	vector<string> asgTable = HUtility().getAssignTable();
	UseTable* useTable = PKB::getUseTable();
	ModifyTable* modTable = PKB::getModifyTable();


	//Case 1st att = _
	if (ptFirstU.first == "_") {
		//Case 2nd att = _
		if (ptSecondU.first == "_") {
			patVec = HUtility().getAssignTable();
		}
		//Case 2nd att = "x123" or _"x+y+123"_
		if (HUtility().getSymMean(ptSecond) == "" && ptSecondU.first != "_") {
			patVec = PKB::checkAssign(ptSecondU.first, ptSecondU.second);
		}
		//Case 2nd att = v
		/*if (HUtility().getSymMean(ptSecond) == "variable") {
			for (size_t i = 0; i < asgTable.size(); i++) {
				pair<string, vector<string>> temp;
				if (useTable->getUsed(asgTable[i]).size()>0) {
					temp.first = asgTable[i];
					temp.second = useTable->getUsed(asgTable[i]);
				}
				patTable.push_back(temp);
			}
		}*/
		//Case 2nd att = c
		//if (HUtility().getSymMean(ptSecond) == "constant") {
		//
		//}
	}

	//Case 1st att = "x123"
	if (HUtility().getSymMean(ptFirst) == "" &&ptFirst != "_") {
		//Case 2nd att = _
		if (ptSecondU.first == "_") {
			patVec = PKB::getModifyTable()->getModifier(ptFirstU.first);
			HUtility().intersectionSS(HUtility().getAssignTable(), patVec, 1);
		}
		//Case 2nd att = "x123" or _"x+123"_
		if (HUtility().getSymMean(ptSecond) == "" && ptSecondU.first != "_") {
			vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstU.first);
			vector<string> temp2 = PKB::checkAssign(ptSecondU.first, ptSecondU.second);
			if (HUtility().intersection(temp1, temp2).size() > 0) {
				patVec = HUtility().intersection(temp1, temp2);
				//patVec = temp2;
			}
		}
		//Case 2nd att = v
		/*if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "variable") {
			vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstU.first);
			vector<UseEntry_t> useTable = PKB::getUseTable()->getTable();
			vector<string> temp2 = HUtility().getUseRelated(useTable, 1);
			if (selType == "variable") {
				vector<string> temp3 = HUtility().intersection(temp1, temp2);
				for (size_t i = 0; i < temp3.size(); i++) {
					for (size_t j = 0; j < useTable.size(); j++) {
						if (temp3[i] == useTable[j].userLine) {
							for (int k = 0; k < useTable[j].usedVar.size(); k++) {
								pvarVec.push_back(useTable[j].usedVar[k]);
							}
						}
					}
				}
			}
			else {
				patVec = HUtility().intersection(temp1, temp2);
			}
		}*/
		//Case 2nd att = c
		//if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "constant") {
		//	//Seltype = constant
		//	if (selType == "constant") {
		//		vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstU.first);
		//		vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
		//		vector < pair<string, vector<string>>> temp3 = HUtility().intersection(temp2, temp1);
		//		for (size_t i = 0; i < temp3.size(); i++) {
		//			pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
		//		}
		//	}
		//	else {
		//		vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstU.first);
		//		vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
		//		patVec = HUtility().intersection(temp2, temp1, true);
		//	}
		//}
	}
	//Case 1st att = v 
	if (HUtility().getSymMean(ptFirst) == "variable") {
		//Case 2nd att = _
		if (ptSecondU.first == "_") {
			for (size_t i = 0; i < asgTable.size(); i++) {
				pair<string, vector<string>> temp;
				if (modTable->getModified(asgTable[i]).size()>0) {
					temp.first = asgTable[i];
					temp.second = modTable->getModified(asgTable[i]);
				}
				patAsgTable.push_back(temp);
			}
		}
		//Case 2nd att = "x123"
		if (ptSecondU.first != "_" && HUtility().getSymMean(ptSecond) == "") {
			vector<string> useVec = PKB::checkAssign(ptSecondU.first, ptSecondU.second);
			vector<string> tempVec = HUtility().intersection(asgTable, useVec);
			//patVec = useVec;
			//patVec.push_back(ptSecondU.first);*/
			for (size_t i = 0; i < tempVec.size(); i++) {
				pair<string, vector<string>> temp;
				if (modTable->getModified(tempVec[i]).size()>0) {
					temp.first = tempVec[i];
					temp.second = modTable->getModified(tempVec[i]);
				}
				patAsgTable.push_back(temp);
			}
		}
		//Case 2nd att = v
	/*	if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "variable") {
			if (selType == "variable") {
				vector<string> temp1 = HUtility().getAssignTable();
				if (rs == ptFirst) {
					for (size_t i = 0; i < temp1.size(); i++) {
						pvarVec = PKB::getModifyTable()->getModified(temp1[i]);
					}
				}
				else {
					for (size_t i = 0; i < temp1.size(); i++) {
						for (size_t j = 0; j < PKB::getUseTable()->getUsed(temp1[i]).size(); j++) {
							pvarVec.push_back(PKB::getUseTable()->getUsed(temp1[i])[j]);
						}
					}
				}
			}
		}*/
		//Case 2nd att = c
		//if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "constant") {
		//	//Seltype = constant
		//	if (selType == "constant") {
		//		vector<string> temp2 = HUtility().getAssignTable();
		//		vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
		//		vector < pair<string, vector<string>>> temp3 = HUtility().intersection(temp2, temp1);
		//		for (size_t i = 0; i < temp3.size(); i++) {
		//			pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
		//		}
		//	}
		//	else {
		//		vector<string> temp2 = HUtility().getAssignTable();
		//		vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
		//		patVec = HUtility().intersection(temp2, temp1, true);
		//	}
		//}
	}
}

void HandlePT::handleIf(string ptFirst, string ptSecond, 
	vector<string> &patVec, vector<pair<string, string>> &ifTable) {
	pair<string, bool> ptFirstQ;
	pair<string, bool> ptSecondQ;
	HUtility().checkQuotation(ptFirstQ, ptFirst);
	HUtility().checkQuotation(ptSecondQ, ptSecond);
	if (HUtility().getSymMean(ptFirst) == "") {
		patVec = PKB::patternIf(ptFirstQ.first, ptFirstQ.second);
	}
	else if (HUtility().getSymMean(ptFirst) == "variable") {
		ifTable = PKB::patternIf();
	}
	//patVec.push_back(ptFirst /*ptFirstQ.first/* + to_string(ptFirstQ.second)*/);
}

void HandlePT::handleWhile(string ptFirst, string ptSecond, 
	vector<string> &patVec, vector<pair<string, string>> &whileTable) {
	pair<string, bool> ptFirstQ;
	pair<string, bool> ptSecondQ;
	HUtility().checkQuotation(ptFirstQ, ptFirst);
	HUtility().checkQuotation(ptSecondQ, ptSecond);
	if (HUtility().getSymMean(ptFirst) == "") {
		patVec = PKB::patternWhile(ptFirstQ.first, ptFirstQ.second);
	}
	else if (HUtility().getSymMean(ptFirst) == "variable") {
		whileTable = PKB::patternWhile();
	}
}

bool HandlePT::containSign(string str) {
	if (str.find("+") != string::npos || str.find("-") != string::npos || str.find("*") != string::npos) {
		return true;
	}
	return false;
}
void HandlePT::checkUnderscore(pair<string, bool> &AttU, string &Att) {
	//Case _"x+y"_ or _
	if (Att.substr(0, 1) == "_") {
		if (Att.size() > 4) {
			AttU.first = Att.substr(2, Att.size() - 4);
			AttU.second = true;
		}
		else {
			AttU.first = Att;
			AttU.second = false;
		}
	}
	//Case "x+y", "x" or v, c
	else {
		if (Att.substr(0, 1) == "\"") {
			AttU.first = Att.substr(1, Att.size() - 2);
			AttU.second = false;
		}
		else {
			AttU.first = Att;
			AttU.second = false;
		}
	}
}
