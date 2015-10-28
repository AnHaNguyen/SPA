#include "../QueryProcessor/QueryHandler.h"

QueryHandler::QueryHandler() {}
QueryHandler::~QueryHandler() {}


vector<string> QueryHandler::queryRec(QueryTree* queryTree) {

	//Initiate global values
	PreResultNode* result;
	PreSuchThatNode* queryST;
	PrePatternNode* queryPT;
	string stFirst;
	string stSecond;
	string ptFirst;
	string ptSecond;
	string ST;
	string PT;
	vector<RSEntry_t> queryRS;

	//Return check
	vector<int> PTCheck;
	vector<int> STCheck;
	vector<string> final;
	//check validity
	if (queryTree->getValidity() == false) {
		return final;
	}
	//Initialize symtable for HUtility
	if (queryTree->getSymbolTable().empty()) {
		return final;
	}
	else {
		HUtility().setSymTable(queryTree->getSymbolTable());
	}

	//Handle select
	string selType = HandleRS().handleSelect(queryTree, result);
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

	vector<string> callVec;
	vector <pair<string, vector<string>>> callTable;

	queryST = queryTree->getSuchThat();
	while (true) {
		if (queryST->getSynonym() != "") {
			ST = queryST->getSynonym();
			stFirst = queryST->getFirstAttr();
			stSecond = queryST->getSecondAttr();

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
							for (size_t i = 0; i < parVec.size(); i++) {
								parQ.push(parVec[i]);
							}
							while (!parQ.empty()) {
								string temp = parQ.front();
								parQ.pop();
								size_t oldSize = parVec.size();
								//Case Parent*(1, s)
								if (HUtility().isInt(stFirst)) {
									HandleST().handleParent(temp, stSecond, parVec);
									if (oldSize < parVec.size()) {
										for (size_t i = oldSize; i < parVec.size(); i++) {
											parQ.push(parVec[i]);
										}
									}
								}
								//Case Parent*(s, 1)
								if (HUtility().isInt(stSecond)) {
									HandleST().handleParent(stFirst, temp, parVec);
									if (oldSize < parVec.size()) {
										for (size_t i = oldSize; i < parVec.size(); i++) {
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

			/*Handle affects - next (next iteration)
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

			//Handle Calls
			if (ST == "Calls") {
				callVec = HandleST().handleCalls(stFirst, stSecond);
				if (callVec.size() > 0 && callVec.front() == "all") {
					HUtility().getCallTable(callTable);
				}
			}

			for (int i = 0; i < 16; i++) {
				STCheck.push_back(0);
			}
			RSEntry_t currentRS;
			currentRS.synCount = 0;
			if (HUtility().getSymMean(stFirst) != "") {
				currentRS.firstAtt = stFirst;
				currentRS.synCount++;
			}
			else {
				currentRS.firstAtt = "";
			}
			if (HUtility().getSymMean(stSecond) != "") {
				currentRS.secondAtt = stSecond;
				currentRS.synCount++;
			}
			else {
				currentRS.secondAtt = "";
			}

			if (!folVec.empty() && folVec.front() != "na" && folVec.front() != "all") {
				STCheck[0] = 1;
				currentRS.vec = folVec;
			}
			if (!folTable.empty()) {
				STCheck[1] = 1;
				currentRS.folTable = folTable;
			}
			if (!mvarVec.empty()) {
				STCheck[2] = 1;
				currentRS.vec = mvarVec;
			}
			if (!modVec.empty() && modVec.front() != "na" && modVec.front() != "all") {
				STCheck[3] = 1;
				currentRS.vec = modVec;
			}
			if (!modTable.empty()) {
				STCheck[4] = 1;
				currentRS.table = modTable;
			}
			if (!parVec.empty() && parVec.front() != "na" && parVec.front() != "all") {
				STCheck[5] = 1;
				currentRS.vec = parVec;
			}
			if (!parTable.empty()) {
				STCheck[6] = 1;
				currentRS.vec = parTable;
			}
			if (!nestTable.empty()) {
				STCheck[7] = 1;
				currentRS.vec = nestTable;
			}
			if (!uvarVec.empty()) {
				STCheck[8] = 1;
				currentRS.vec = uvarVec;
			}
			if (!useVec.empty() && useVec.front() != "na" && useVec.front() != "all") {
				STCheck[9] = 1;
				currentRS.vec = useVec;
			}
			if (!userTable.empty()) {
				STCheck[10] = 1;
				currentRS.vec = userTable;
			}
			if (!usedTable.empty()) {
				STCheck[11] = 1;
				currentRS.vec = usedTable;
			}
			if (!nextVec.empty() && nextVec.front() != "na" && nextVec.front() != "all") {
				STCheck[12] = 1;
				currentRS.vec = nextVec;
			}
			if (!nextTable.empty()) {
				STCheck[13] = 1;
				currentRS.table = nextTable;
			}
			if (!callVec.empty() && callVec.front() != "na" && callVec.front() != "all") {
				STCheck[14] = 1;
				currentRS.vec = callVec;
			}
			if (!callTable.empty()) {
				STCheck[15] = 1;
				currentRS.table = callTable;
			}
			if (HUtility().getPos(STCheck) == -1) {
				return final;
			}
			if (currentRS.synCount > 0) {
				queryRS.push_back(currentRS);
			}
		}
		if (queryST->getNext() != NULL) {
			queryST = queryST->getNext();
		}
		else {
			break;
		}
	}


	//Handle pattern
	vector<string> patVec;
	vector<string> pvarVec;
	vector<string> pconVec;

	queryPT = queryTree->getPattern();
	while (true) {
		if (queryPT->getSynonym() != "") {
			PT = queryPT->getSynonym();
			string pType = HUtility().getSymMean(PT);
			ptFirst = queryPT->getFirstAttr();
			ptSecond = queryPT->getSecondAttr();

			pair<string, bool> ptFirstQ;
			pair<string, bool> ptSecondQ;
			HUtility().checkQuotation(ptFirstQ, ptFirst);
			HUtility().checkQuotation(ptSecondQ, ptSecond);

			if (pType == "assign") {
				HandlePT().handleAssign(ptFirst, ptSecond, selType, rs, patVec, pvarVec, pconVec);
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

			RSEntry_t currentRS;
			currentRS.firstAtt = ptFirst;
			currentRS.secondAtt = ptSecond;

			if (!patVec.empty() && patVec.front() != "na") {
				PTCheck[0] = 1;
				currentRS.vec = patVec;
			}
			if (!pvarVec.empty()) {
				PTCheck[1] = 1;
				currentRS.vec = pvarVec;
			}
			if (!pconVec.empty()) {
				PTCheck[2] = 1;
				currentRS.vec = pconVec;
			}
			if (HUtility().getPos(PTCheck) == -1) {
				return final;
			}
			if (currentRS.synCount > 0) {
				queryRS.push_back(currentRS);
			}
		}
		if (queryPT->getNext() != NULL) {
			queryPT = queryPT->getNext();
		}
		else {
			break;
		}
	}


	//Return function normal case
	//Find all dependency of syn and clauses
	//Populate attList's atts and reClause
	vector <attEntry_t> attList;
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (HUtility().contain(attList, queryRS[i].firstAtt) == -1) {
			attEntry_t temp;
			temp.att = queryRS[i].firstAtt;
			for (size_t j = 0; j < queryRS.size(); j++) {
				if (queryRS[i].firstAtt == queryRS[j].firstAtt || queryRS[i].firstAtt == queryRS[j].secondAtt) {
					temp.reClause.push_back(j);
				}
			}
			if (temp.reClause.size()>0) {
				attList.push_back(temp);
			}
		}
		if (HUtility().contain(attList, queryRS[i].secondAtt) == -1) {
			attEntry_t temp;
			temp.att = queryRS[i].secondAtt;
			for (size_t j = 0; j < queryRS.size(); j++) {
				if (queryRS[i].secondAtt == queryRS[j].firstAtt || queryRS[i].secondAtt == queryRS[j].secondAtt) {
					temp.reClause.push_back(j);
				}
			}
			if (temp.reClause.size()>0) {
				attList.push_back(temp);
			}
		}
	}
	//Populate attList's reAtt
	for (size_t i = 0; i < attList.size(); i++) {
		vector<int> temp;
		for (size_t j = i + 1; j < attList.size(); j++) {
			for (size_t k = 0; k < attList[i].reClause.size(); k++) {
				if (find(attList[j].reClause.begin(), attList[j].reClause.end(), attList[i].reClause[k]) != attList[j].reClause.end()) {
					//attList[i].reAtt.push_back[j];
					temp.push_back(j);
					break;
				}
			}
		}
		attList[i].reAtt = temp;
	}
 
	//Initialize loop list
	vector<int> loopList;
	for (size_t i = 0; i < attList.size(); i++) {
		loopList.push_back(i);
	}
	while (loopList.size() > 0) {
		int i = loopList.front();
		string var = attList[i].att;
		vector<int> pos = attList[i].reClause;
		bool changed = 0;
		for (size_t j = 0; j < pos.size(); j++) {
			//Case 1 syn
			if (queryRS[pos[j]].synCount == 1) {
				for (size_t k = 0; k < pos.size(); k++) {
					if (queryRS[pos[k]].synCount == 1) {
						int oldJSize = queryRS[pos[j]].vec.size();
						int oldKSize = queryRS[pos[k]].vec.size();
						queryRS[pos[j]].vec = HUtility().intersection(queryRS[pos[j]].vec, queryRS[pos[k]].vec);
						queryRS[pos[k]].vec = queryRS[pos[j]].vec;
						if (oldJSize != queryRS[pos[j]].vec.size() || oldKSize != queryRS[pos[k]].vec.size()) {
							changed += true;
						}
					}
					if (queryRS[pos[k]].synCount == 2) {
						if (queryRS[pos[k]].table.size()>0 && var == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPSV(queryRS[pos[j]].vec, queryRS[pos[k]].table, 1);
						}
						if (queryRS[pos[k]].table.size()>0 && var == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPSV(queryRS[pos[j]].vec, queryRS[pos[k]].table, 2);
						}
						if (queryRS[pos[k]].folTable.size() > 0 && var == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPSS(queryRS[pos[j]].vec, queryRS[pos[k]].folTable, 1);
						}
						if (queryRS[pos[k]].folTable.size() > 0 && var == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPSS(queryRS[pos[j]].vec, queryRS[pos[k]].folTable, 2);
						}
					}
				}
			}
			//Case 2 syns
			if (queryRS[pos[j]].synCount == 2) {
				for (size_t k = 0; k < pos.size(); k++) {
					if (queryRS[pos[k]].synCount == 1) {
						if (queryRS[pos[j]].table.size()>0 && var == queryRS[pos[j]].firstAtt) {
							changed += HUtility().intersectionPSV(queryRS[pos[k]].vec, queryRS[pos[j]].table, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && var == queryRS[pos[j]].secondAtt) {
							changed += HUtility().intersectionPSV(queryRS[pos[k]].vec, queryRS[pos[j]].table, 2);
						}
						if (queryRS[pos[j]].folTable.size() > 0 && var == queryRS[pos[j]].firstAtt) {
							changed += HUtility().intersectionPSS(queryRS[pos[k]].vec, queryRS[pos[j]].folTable, 1);
						}
						if (queryRS[pos[j]].folTable.size() > 0 && var == queryRS[pos[j]].secondAtt) {
							changed += HUtility().intersectionPSS(queryRS[pos[k]].vec, queryRS[pos[j]].folTable, 2);
						}
					}

					if (queryRS[pos[k]].synCount == 2) {
						//case table vs table
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 2);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 2);
						}

						//Case foltable vs foltable
						if (queryRS[pos[j]].folTable.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSS(queryRS[pos[j]].folTable, queryRS[pos[k]].folTable, 1, 1);
						}
						if (queryRS[pos[j]].folTable.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSS(queryRS[pos[j]].folTable, queryRS[pos[k]].folTable, 1, 2);
						}
						if (queryRS[pos[j]].folTable.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSS(queryRS[pos[j]].folTable, queryRS[pos[k]].folTable, 2, 1);
						}
						if (queryRS[pos[j]].folTable.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSS(queryRS[pos[j]].folTable, queryRS[pos[k]].folTable, 2, 2);
						}

						//Case foltable vs table
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[k]].folTable, queryRS[pos[j]].table, 1, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[k]].folTable, queryRS[pos[j]].table, 2, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[k]].folTable, queryRS[pos[j]].table, 1, 2);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].folTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[k]].folTable, queryRS[pos[j]].table, 2, 2);
						}

						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].folTable.size() > 0 &&
							queryRS[pos[k]].firstAtt == queryRS[pos[j]].firstAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[j]].folTable, queryRS[pos[k]].table, 1, 1);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].folTable.size() > 0 &&
							queryRS[pos[k]].firstAtt == queryRS[pos[j]].secondAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[j]].folTable, queryRS[pos[k]].table, 2, 1);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].folTable.size() > 0 &&
							queryRS[pos[k]].secondAtt == queryRS[pos[j]].firstAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[j]].folTable, queryRS[pos[k]].table, 1, 2);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].folTable.size() > 0 &&
							queryRS[pos[k]].secondAtt == queryRS[pos[j]].secondAtt) {
							changed += HUtility().intersectionPPSM(queryRS[pos[j]].folTable, queryRS[pos[k]].table, 2, 2);
						}
					}
				}
			}
		}
		loopList.erase(loopList.begin());
		if (changed > 0) {
			for (size_t j = 0; j < attList[i].reAtt.size(); j++) {
				if (!HUtility().contain(loopList, attList[i].reAtt[j])) {
					loopList.push_back(attList[i].reAtt[j]);
				}
			}
		}
	}

	//Check if any interstion results in empty vector
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (queryRS[i].vec.empty() && queryRS[i].table.empty() && queryRS[i].folTable.empty()) {
			return final;
		}
	}

	//Check case select not equal to each attribute
	bool checkRS = false;
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (queryRS[i].firstAtt == rs || queryRS[i].secondAtt == rs) {
			checkRS = true;
		}
	}
	if (!checkRS) {
		if (selType == "prog_line" || selType == "stmt") {
			final = PKB::getProgLine()->getLinesOfType("prog_line");
		}
		if (selType == "variable") {
			final = PKB::getVarTable()->getTable();
		}

		if (selType == "constant") {
			vector<ConstEntry_t> constTable = PKB::getConstTable()->getTable();
			for (size_t i = 0; i < constTable.size(); i++) {
				for (size_t j = 0; j < constTable[i].constants.size(); j++) {
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
		HandleRS().rmEString(final);
		return final;
	}

	//Case select within any clause (at this point no more empty vector
	for (size_t i = 0; i < attList.size(); i++) {
		if (rs == attList[i].att) {
			//Case at least 1 1syn clause is the rs
			bool found = false;
			for (size_t j = 0; j < attList[i].reClause.size(); j++) {
				if (queryRS[attList[i].reClause[j]].synCount == 1) {
					final == queryRS[attList[i].reClause[j]].vec;
					found = true;
				}
				break;
			}

			//Case select from 2 syn
			if (!found) {
				for (size_t j = 0; j < attList[i].reClause.size(); j++) {
					if (queryRS[attList[i].reClause[j]].synCount == 2) {
						if (rs == queryRS[attList[i].reClause[j]].firstAtt && queryRS[attList[i].reClause[j]].table.size()>0) {
							for (size_t k = 0; j < queryRS[attList[i].reClause[j]].table.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].table[k].first);
							}
						}
						if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].table.size()>0) {
							for (size_t k = 0; j < queryRS[attList[i].reClause[j]].table.size(); k++) {
								for (size_t h = 0; h < queryRS[attList[i].reClause[j]].table[k].second.size(); h++) {
									if (!HUtility().contain(final, queryRS[attList[i].reClause[j]].table[k].second[j])) {
										final.push_back(queryRS[attList[i].reClause[j]].table[k].second[h]);
									}
								}
							}
						}
						if (rs == queryRS[attList[i].reClause[j]].firstAtt && queryRS[attList[i].reClause[j]].folTable.size()>0) {
							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].folTable.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].folTable[k].first);
							}
						}
						if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].folTable.size()>0) {
							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].folTable.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].folTable[k].second);
							}
						}
					}
				}
			}
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
	HandleRS().rmEString(final);
	//final.push_back(to_string(HUtility().getPos(STCheck))+" " +to_string(HUtility().getPos(PTCheck)));
	return final;
}