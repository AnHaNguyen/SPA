#include "../QueryProcessor/HandlePT.h"
HandlePT::HandlePT() {}
HandlePT::~HandlePT() {}

void HandlePT::handleAssign(string ptFirst, string ptSecond, string selType, string rs,
	vector<string> patVec, vector<string> pvarVec, vector<string> pconVec) {
	pair<string, bool> ptFirstU;
	pair<string, bool> ptSecondU;
	HandlePT().checkUnderscore(ptFirstU, ptFirst);
	HandlePT().checkUnderscore(ptSecondU, ptSecond);

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
		if (HUtility().getSymMean(ptSecond) == "variable") {
			//Select type = variable
			if (selType == "variable") {
				vector<string> tempVec;
				tempVec = HUtility().getAssignTable();
				for (int i = 0; i < tempVec.size(); i++) {
					vector<string> current = PKB::getUseTable()->getUsed(tempVec[i]);
					for (int j = 0; j < current.size(); j++) {
						pvarVec.push_back(current[j]);
					}
				}
			}
			//Select type = assign or not found
			else {
				vector<string> tempVec;
				tempVec = HUtility().getAssignTable();
				for (int i = 0; i < tempVec.size(); i++) {
					vector<string> current = PKB::getUseTable()->getUsed(tempVec[i]);
					if (current.size() > 0) {
						patVec.push_back("true");
						break;
					}
				}
			}
		}
		//Case 2nd att = c
		if (HUtility().getSymMean(ptSecond) == "constant") {
			//Seltype = constant
			if (selType == "constant") {
				vector<string> temp1 = HUtility().getAssignTable();
				vector<pair<string, vector<string>>> temp2 = HUtility().getConstTable();
				vector<pair<string, vector<string>>> ansVec = HUtility().intersection(temp1, temp2);
				for (int i = 0; i < ansVec.size(); i++) {
					pconVec.insert(pconVec.end(), temp2[i].second.begin(), temp2[i].second.end());
				}
			}
			else {
				vector<string> temp2 = HUtility().getAssignTable();
				vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
				patVec = HUtility().intersection(temp2, temp1, true);
			}
		}
	}

	//Case 1st att = "x123"
	if (HUtility().getSymMean(ptFirst) == "" &&ptFirst != "_") {
		//Case 2nd att = _
		if (ptSecondU.first == "_") {
			patVec = PKB::getModifyTable()->getModifier(ptFirstU.first);
		}
		//Case 2nd att = "x123" or _"x+123"_
		if (HUtility().getSymMean(ptSecond) == "" && ptSecondU.first != "_") {
			vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstU.first);
			vector<string> temp2 = PKB::checkAssign(ptSecondU.first, ptSecondU.second);
			if (HUtility().intersection(temp1, temp2).size() > 0) {
				patVec = HUtility().intersection(temp1, temp2);
			}

		}
		//Case 2nd att = v
		if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "variable") {
			vector<string> temp1 = PKB::getModifyTable()->getModifier(ptFirstU.first);
			vector<UseEntry_t> useTable = PKB::getUseTable()->getTable();
			vector<string> temp2 = HUtility().toConvention(useTable, true);
			if (selType == "variable") {
				vector<string> temp3 = HUtility().intersection(temp1, temp2);
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
			else {
				patVec = HUtility().intersection(temp1, temp2);
			}
		}
		//Case 2nd att = c
		if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "constant") {
			//Seltype = constant
			if (selType == "constant") {
				vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstU.first);
				vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
				vector < pair<string, vector<string>>> temp3 = HUtility().intersection(temp2, temp1);
				for (int i = 0; i < temp3.size(); i++) {
					pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
				}
			}
			else {
				vector<string> temp2 = PKB::getModifyTable()->getModifier(ptFirstU.first);
				vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
				patVec = HUtility().intersection(temp2, temp1, true);
			}
		}
	}
	//Case 1st att = v 
	if (HUtility().getSymMean(ptFirst) == "variable") {
		//Case 2nd att = _
		if (ptSecondU.first == "_") {
			if (selType == "variable") {
				vector<string> temp1 = HUtility().getAssignTable();
				for (int i = 0; i < temp1.size(); i++) {
					pvarVec = PKB::getModifyTable()->getModified(temp1[i]);
				}
			}
			else {
				patVec = HUtility().getAssignTable();
			}
		}
		//Case 2nd att = "x123"
		if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "") {
			if (selType == "variable") {
				vector<string> temp1 = HUtility().getAssignTable();
				vector<string> temp2 = PKB::getUseTable()->getUser(ptSecondU.first);
				vector<string> temp3 = HUtility().intersection(temp1, temp2);
				for (int i = 0; i < temp3.size(); i++) {
					pvarVec = PKB::getModifyTable()->getModified(temp3[i]);
				}
			}
			else {
				vector<string> temp1 = HUtility().getAssignTable();
				vector<string> temp2 = PKB::getUseTable()->getUser(ptSecondU.first);
				patVec = HUtility().intersection(temp1, temp2);
			}
		}
		//Case 2nd att = v
		if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "variable") {
			if (selType == "variable") {
				vector<string> temp1 = HUtility().getAssignTable();
				if (rs == ptFirst) {
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
		if (HandlePT().containSign(ptSecondU.first) == false && HUtility().getSymMean(ptSecond) == "constant") {
			//Seltype = constant
			if (selType == "constant") {
				vector<string> temp2 = HUtility().getAssignTable();
				vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
				vector < pair<string, vector<string>>> temp3 = HUtility().intersection(temp2, temp1);
				for (int i = 0; i < temp3.size(); i++) {
					pconVec.insert(pconVec.end(), temp1[i].second.begin(), temp1[i].second.end());
				}
			}
			else {
				vector<string> temp2 = HUtility().getAssignTable();
				vector<pair<string, vector<string>>> temp1 = HUtility().getConstTable();
				patVec = HUtility().intersection(temp2, temp1, true);
			}
		}
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

void HandlePT::checkQuotation(pair<string, bool> &AttQ, string &Att) {

	//Case "x"
	if (Att.substr(0, 1) == "\"") {
		AttQ.first = Att.substr(1, Att.size() - 2);
		AttQ.second = true;
	}
	//Case v or _
	else {
		AttQ.first = Att;
		AttQ.second = false;
	}
}