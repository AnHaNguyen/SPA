#include "../QueryProcessor/QueryHandler.h"

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
	string ST;
	string PT;

	//Return check
	vector<int> PTCheck;
	vector<int> STCheck;
	vector<string> final;
	//check validity
	if (query->getValidity() == false) {
		return final;
	}
	//Initialize symtable for HUtility
	if (query->getSymbolTable().empty()) {
		return final;
	}
	else {
		HUtility().setSymTable(query->getSymbolTable());
	}

	//Handle select
	string selType = HandleRS().handleSelect(query, result);
	if (selType == "") {
		return final;
	}
	string rs = result->getResult().getSynonym();

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

	vector<string> nextVec;
	vector<pair<string, vector<string>>>  nextTable;

	if (query->getSuchThat()->getSynonym() != "") {
		suchThat = query->getSuchThat();
		ST = suchThat->getSynonym();
		stFirst = suchThat->getFirstAttr();
		stSecond = suchThat->getSecondAttr();

		//Handle follows
		if (ST == "Follows") {
			folVec.push_back(HandleST().handleFollows(stFirst, stSecond));
			if (folVec.size() > 0 && folVec.front() == "all") {
				HUtility().getFollowTable(folTable);
			}
		}

		//Handle follows*
		if (ST == "Follows*") {
			//Check if case Follows*(1, 2)
			if (HUtility().isInt(stFirst) && HUtility().isInt(stSecond)) {
				if (HandleST().isFollowsS(stFirst, stSecond)) {
					folVec.push_back("true");
				}
			}
			else {
				folVec.push_back(HandleST().handleFollows(stFirst, stSecond));
				queue<string> folQ;
				folQ.push(folVec[0]);
				if (folVec.size() > 0 && folVec.front() == "all") {
					HUtility().getFollowTable(folTable);
				}
				else {
					while (!folQ.empty()) {
						string temp = folQ.front();
						folQ.pop();
						string nextFol;
						if (HUtility().isInt(stFirst)) {
							nextFol = HandleST().handleFollows(temp, stSecond);
							if (nextFol != "na") {
								folQ.push(nextFol);
								folVec.push_back(nextFol);
							}
						}
						if (HUtility().isInt(stSecond)) {
							nextFol = HandleST().handleFollows(stFirst, temp);
							if (nextFol != "na") {
								folQ.push(nextFol);
								folVec.push_back(nextFol);
							}
						}
					}
				}
			}
		}
	
		//Handle modifies
		if (ST == "Modifies") {
			HandleST().handleModifies(stFirst, stSecond, modVec, mvarVec);
			if (modVec.size() > 0 && modVec.front() == "all") {
				HUtility().getModifyTable(modTable);
			}
		}

		//Handle parent
		if (ST == "Parent") {
			HandleST().handleParent(stFirst, stSecond, parVec);
			if (parVec.size() > 0 && parVec.front() == "all") {
				HUtility().getParentTable(result, stFirst, parTable, stSecond, nestTable);
			}
		}

		//Handle parent*
		if (ST == "Parent*") {
			//Check case Parent*(1, 2)
			if (HUtility().isInt(stFirst) && HUtility().isInt(stSecond)) {
				if (HandleST().isParentS(stFirst, stSecond)) {
					parVec.push_back("true");
				}
			}
			else {
				HandleST().handleParent(stFirst, stSecond, parVec);
				if (!parVec.empty()) {
					queue<string> parQ;
					if (parVec.size() > 0 && parVec.front() == "all") {
						HUtility().getParentTable(result, stFirst, parTable, stSecond, nestTable);
					}
					else {
						for (int i = 0; i < parVec.size(); i++) {
							parQ.push(parVec[i]);
						}
						while (!parQ.empty()) {
							string temp = parQ.front();
							parQ.pop();
							int oldSize = parVec.size();
							//Case Parent*(1, s)
							if (HUtility().isInt(stFirst)) {
								HandleST().handleParent(temp, stSecond, parVec);
								if (oldSize < parVec.size()) {
									for (int i = oldSize; i < parVec.size(); i++) {
										parQ.push(parVec[i]);
									}
								}
							}
							//Case Parent*(s, 1)
							if (HUtility().isInt(stSecond)) {
								HandleST().handleParent(stFirst, temp, parVec);
								if (oldSize < parVec.size()) {
									for (int i = oldSize; i < parVec.size(); i++) {
										parQ.push(parVec[i]);
									}
								}
							}
						}
					}
				}
			}
		}

		//Handle uses
		vector<UseEntry_t> useTab;
		if (ST == "Uses") {
			HandleST().handleUses(stFirst, stSecond, useVec, uvarVec);
			if (useVec.size() > 0 && useVec.front() == "all") {
				HUtility().getUseTable(useTab, userTable, usedTable);
			}
		}

		/*Handle affecs - next (next iteration)
		if (ST == "affects") {
		ansVec = getAffect(firstAtt, secondAtt);
		}
		if (ST == "affects*") {
		ansVec = getAffectS(firstAtt, secondAtt);
		}
		}*/
		//Handle Next
		if (ST == "Next") {
			nextVec = HandleST().handleNext(stFirst, stSecond);
			if (nextVec.size() > 0 && nextVec.front() == "all") {
				HUtility().getNextTable(nextTable);
			}
		}

		for (int i = 0; i < 14; i++) {
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
		if (!modVec.empty() && modVec.front() != "na" && modVec.front() != "all") {
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
		if (!nestTable.empty()) {
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
		if (!nextVec.empty() && nextVec.front() != "na" && nextVec.front() != "all") {
			STCheck[12] = 1;
		}
		if (!nextTable.empty()) {
			STCheck[13] = 1;
		}
	}
	//Handle pattern (this iteration only assign pattern)
	vector<string> patVec;
	vector<string> pvarVec;
	vector<string> pconVec;


	if (query->getPattern()->getSynonym() != "") {
		pattern = query->getPattern();
		PT = pattern->getSynonym();
		string pType = HUtility().getSymMean(PT);
		ptFirst = pattern->getFirstAttr();
		ptSecond = pattern->getSecondAttr();

		pair<string, bool> ptFirstU;
		pair<string, bool> ptSecondU;
		HandlePT().checkUnderscore(ptFirstU, ptFirst);
		HandlePT().checkUnderscore(ptSecondU, ptSecond);

		pair<string, bool> ptFirstQ;
		pair<string, bool> ptSecondQ;
		HandlePT().checkQuotation(ptFirstQ, ptFirst);
		HandlePT().checkQuotation(ptSecondQ, ptSecond);

		if (pType == "assign") {
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
						if (result->getResult().getSynonym() == ptFirst) {
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
		if (PT == "if") {
			patVec = PKB::patternIf(ptFirstQ.first, ptFirstQ.second);
		}
		if (PT == "while") {
			patVec = PKB::patternWhile(ptFirstQ.first, ptFirstQ.second);
		}
		for (int i = 0; i < 3; i++) {
			PTCheck.push_back(0);
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
	
	//Check if pattern or such that is false
	if (query->getSuchThat()->getSynonym() != "" && HUtility().getPos(STCheck) == -1) {
		HUtility().rmEString(final);
		return final;
	}
	if (query->getPattern()->getSynonym() != "" && HUtility().getPos(PTCheck) == -1) {
		HUtility().rmEString(final);
		return final;
	}

	//Check case select not equal to each attribute
	if (rs != stFirst && rs != stSecond && rs != ptFirst && rs != ptSecond && rs != PT) {
		if (selType == "prog_line" || selType == "stmt") {
			final = PKB::getProgLine()->getLinesOfType("prog_line");
		}
		if (selType == "variable") {
			final = PKB::getVarTable()->getTable();
		}

		if (selType == "constant") {
			vector<ConstEntry_t> constTable = PKB::getConstTable()->getTable();
			for (int i = 0; i < constTable.size(); i++) {
				for (int j = 0; j < constTable[i].constants.size(); j++) {
					if (!HUtility().contain(final, constTable[i].constants[j])) {
						final.push_back(constTable[i].constants[j]);
					}
				}
			}
		}
		if (selType == "procedure") {
			final = PKB::getProcTable()->getTable();
		}
		if (selType == "assign") {
			final = PKB::getProgLine()->getLinesOfType("assign");
		}
		if (selType == "while") {
			final = PKB::getProgLine()->getLinesOfType("while");
		}
		if (selType == "if") {
			final = PKB::getProgLine()->getLinesOfType("if");
		}
		HUtility().rmEString(final);
		return final;
	}

	//Return function normal case
	if (HUtility().getPos(STCheck) != -1) {
		switch (HUtility().getPos(STCheck)) {
		case 0:
			final = folVec;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(folVec, patVec);
			}
			break;
		case 1:
			for (int i = 0; i < folTable.size(); i++) {
				if (rs == stFirst) {
					//Check if stSecond is in FolTable
					if (HandleRS().folAss(stSecond, stFirst, folTable, i)) {
						final.push_back(folTable[i].first);
					}
					else if (HUtility().getSymMean(stSecond) != "assign") {
						final.push_back(folTable[i].first);
					}
				}

				if (rs == stSecond) {
					//Check if stFirst is in FolTable
					if (stFirst != stFirst)
						final.push_back(stFirst);
					if (HandleRS().folAss(stFirst, stFirst, folTable, i)) {
						final.push_back(folTable[i].second);
					}
					else if (HUtility().getSymMean(stFirst) != "assign") {
						final.push_back(folTable[i].second);
					}
				}
				if (rs != stFirst && rs != stSecond && folTable.size() > 0) {
					final.push_back("true");
				}
			}
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(patVec, folTable);
			}
			break;
		case 2:
			final = mvarVec;
			if (HUtility().getPos(PTCheck) == 1) {
				final = HUtility().intersection(mvarVec, pvarVec);
			}
			break;
		case 3:
			final = modVec;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(modVec, patVec);
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
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(patVec, modTable, true);
			}
			if (HUtility().getPos(PTCheck) == 1) {
				final = HUtility().intersection(pvarVec, modTable, true);
			}
			if (HUtility().getPos(PTCheck) == 2) {
				final = HUtility().intersection(pconVec, modTable, true);
			}
			break;
		case 5:
			final = parVec;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(parVec, patVec);
			}
			break;
		case 6:
			final = parTable;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(parTable, patVec);
			}
			break;
		case 7:
			final = nestTable;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(nestTable, patVec);
			}
			break;
		case 8:
			final = uvarVec;
			if (HUtility().getPos(PTCheck) == 1) {
				final = HUtility().intersection(uvarVec, pvarVec);
			}
			break;
		case 9:
			final = useVec;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(useVec, patVec);
			}
			break;
		case 10:
			final = userTable;
			if (HUtility().getPos(PTCheck) == 0) {
				final = HUtility().intersection(userTable, patVec);
			}
			break;
		case 11:
			final = usedTable;
			if (HUtility().getPos(PTCheck) == 1) {
				final = HUtility().intersection(usedTable, pvarVec);
			}
			if (HUtility().getPos(PTCheck) == 2) {
				final = HUtility().intersection(usedTable, pconVec);
			}
			break;
		}
	}
	else if (HUtility().getPos(PTCheck) != -1) {
		switch (HUtility().getPos(PTCheck)) {
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
	if (final.size() > 0 && selType == "assign") {
		final = HUtility().intersection(final, HUtility().getAssignTable());
	}
	if (selType == "prog_line" || selType == "stmt") {
		final = HUtility().intersection(final, PKB::getProgLine()->getLinesOfType("prog_line"));
	}
	if (selType == "while") {
		final = HUtility().intersection(final, PKB::getProgLine()->getLinesOfType("while"));
	}
	if (selType == "if") {
		final = HUtility().intersection(final, PKB::getProgLine()->getLinesOfType("if"));
	}
	if (selType == "procedure") {
		final = HUtility().intersection(final, PKB::getProgLine()->getLinesOfType("procedure"));
	}
	HUtility().rmEString(final);
	//final.push_back(to_string(HUtility().getPos(STCheck))+" " +to_string(HUtility().getPos(PTCheck)));
	return final;
}