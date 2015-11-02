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
	HandleRS handleRS = HandleRS();
	HandleST handleST = HandleST();
	HandlePT handlePT = HandlePT();
	HUtility utility = HUtility();
	ProgLine* progLine = PKB::getProgLine();

	//Return check
	vector<int> PTCheck;
	vector<int> STCheck;
	vector<string> final;


	//check validity
	if (queryTree->getValidity() == false) {
		return final;
	}
	/*final.push_back("valid");
	return final;*/

	//Initialize symtable for HUtility
	if (queryTree->getSymbolTable().empty()) {
		return final;
	}
	else {
		utility.setSymTable(queryTree->getSymbolTable());
	}

	//Handle select
	string selType = HandleRS().handleSelect(queryTree, result);
	if (selType == "") {
		return final;
	}
	string rs = result->getResult().getSynonym();

	//Handle suchThat

	queryST = queryTree->getSuchThat();
	while (true) {
		vector<string> folVec;
		vector<pair<string, string>>  folTable;

		vector<string> mvarVec;
		vector<string> modVec;
		vector<pair<string, vector<string>>>  modTable;

		vector<string> parVec;
		vector<pair<string, vector<string>>>  parTable;


		vector<string> useVec;
		vector<pair<string, vector<string>>> useTable;

		vector<string> nextVec;
		vector<pair<string, vector<string>>>  nextTable;

		vector<string> callVec;
		vector <pair<string, vector<string>>> callTable;


		if (queryST->getSynonym() != "") {
			ST = queryST->getSynonym();
			stFirst = queryST->getFirstAttr();
			stSecond = queryST->getSecondAttr();

			//Handle follows
			if (ST == "Follows") {
				folVec.push_back(handleST.handleFollows(stFirst, stSecond));
				if (folVec.size() > 0 && folVec.front() == "all") {
					utility.getFollowTable(folTable);
					handleRS.checkPSS(folTable, stFirst, stSecond);
				}
				if (folVec.size() > 0 && folVec.front() != "all"&& folVec.front() != "na"&& folVec.front() != "true") {
					handleRS.checkSS(folVec, stFirst, stSecond);
				}
			}

			//Handle follows*
			if (ST == "Follows*") {
				FollowSTable* folS = PKB::getFollowSTable();
				//Check if case Follows*(1, 2)
				if (utility.isInt(stFirst) && utility.isInt(stSecond)) {
					if (folS->isFollowS(stFirst, stSecond)) {
						folVec.push_back("true");
					}
				}
				else {
					folVec.push_back(handleST.handleFollows(stFirst, stSecond));
					if (folVec.size() > 0 && folVec.front() == "all") {
						utility.getFollowTable(folTable);
						handleRS.checkPSS(folTable, stFirst, stSecond);
					}
					else {
						//Case Follows*(1,s)
						if (utility.isInt(stFirst)) {
							folVec = folS->getNextS(stFirst);
						}
						if (utility.isInt(stSecond)) {
							folVec = folS->getPrevS(stSecond);
						}
						handleRS.checkSS(folVec, stFirst, stSecond);
					}
				}
			}

			//Handle modifies
			if (ST == "Modifies") {
				handleST.handleModifies(stFirst, stSecond, modVec, mvarVec);
				if (modVec.size() > 0 && modVec.front() == "all") {
					utility.getModifyTable(modTable);
					handleRS.checkPSV(modTable, stFirst, stSecond);
				}
				handleRS.checkSS(modVec, stFirst, stSecond);
			}

			//Handle parent
			if (ST == "Parent") {
				handleST.handleParent(stFirst, stSecond, parVec);
				if (parVec.size() > 0 && parVec.front() == "all") {
					utility.getParentTable(parTable);
					handleRS.checkPSV(parTable, stFirst, stSecond);
				}
				else {
					handleRS.checkSS(parVec, stFirst, stSecond);
				}
				/*final.push_back(to_string(parTable.size()));
				return final;*/
			}

			//Handle parent*
			if (ST == "Parent*") {
				//Check case Parent*(1, 2)
				if (utility.isInt(stFirst) && utility.isInt(stSecond)) {
					if (handleST.isParentS(stFirst, stSecond)) {
						parVec.push_back("true");
					}
				}
				else {
					handleST.handleParent(stFirst, stSecond, parVec);
					if (!parVec.empty()) {
						if (parVec.size() > 0 && parVec.front() == "all") {
							utility.getParentTable(parTable);
							handleRS.checkPSV(parTable, stFirst, stSecond);
						}
						else {
							ParentSTable* parTab = PKB::getParentSTable();
							//Case Parent*(1, s)
							if (utility.isInt(stFirst)) {
								parVec = parTab->getChildS(stFirst);
							}
							if (utility.isInt(stSecond)) {
								parVec = parTab->getParentS(stSecond);
							}
							//return parVec;
							handleRS.checkSS(parVec, stFirst, stSecond);
						}
					}
				}
			}

			//Handle uses
			vector<UseEntry_t> useTab;
			if (ST == "Uses") {
				handleST.handleUses(stFirst, stSecond, useVec);
				if (useVec.size() > 0 && useVec.front() == "all") {
					utility.getUseTable(useTable);
					handleRS.checkPSV(useTable, stFirst, "");
				}
				else {
					handleRS.checkSS(useVec, stFirst, "");
				}
				/*for (size_t i = 0; i < useTable.size(); i++) {
					final.push_back(useTable[i].first);
				}
				return final;*/
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
				nextVec = handleST.handleNext(stFirst, stSecond);
				if (nextVec.size() > 0 && nextVec.front() == "all") {
					utility.getNextTable(nextTable);
				}
			}

			//Handle Calls
			if (ST == "Calls") {
				callVec = handleST.handleCalls(stFirst, stSecond);
				if (callVec.size() > 0 && callVec.front() == "all") {
					utility.getCallTable(callTable);
				}
			}
			for (int i = 0; i < 13; i++) {
				STCheck.push_back(0);
			}
			RSEntry_t currentRS;
			currentRS.synCount = 0;
			if (utility.getSymMean(stFirst) != "") {
				currentRS.firstAtt = stFirst;
				currentRS.synCount++;
			}
			else {
				currentRS.firstAtt = "";
			}
			if (utility.getSymMean(stSecond) != "") {
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
				currentRS.ssTable = folTable;
			}
			if (!mvarVec.empty()) {
				STCheck[2] = 1;
				currentRS.vec = mvarVec;
			}
			if (!modVec.empty() && modVec.front() != "all") {
				STCheck[3] = 1;
				currentRS.vec = modVec;
			}
			if (!modTable.empty()) {
				STCheck[4] = 1;
				currentRS.table = modTable;
			}
			if (!parVec.empty() && parVec.front() != "all") {
				STCheck[5] = 1;
				currentRS.vec = parVec;
			}
			if (!parTable.empty()) {
				STCheck[6] = 1;
				currentRS.table = parTable;
			}
			if (!useVec.empty() && useVec.front() != "all") {
				STCheck[7] = 1;
				currentRS.vec = useVec;
			}
			if (!useTable.empty()) {
				STCheck[8] = 1;
				currentRS.table = useTable;
			}
			if (!nextVec.empty() && nextVec.front() != "all") {
				STCheck[9] = 1;
				currentRS.vec = nextVec;
			}
			if (!nextTable.empty()) {
				STCheck[10] = 1;
				currentRS.table = nextTable;
			}
			if (!callVec.empty() && callVec.front() != "all") {
				STCheck[11] = 1;
				currentRS.vec = callVec;
			}
			if (!callTable.empty()) {
				STCheck[12] = 1;
				currentRS.table = callTable;
			}
			if (utility.getPos(STCheck) == -1) {
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
	queryPT = queryTree->getPattern();
	while (true) {
		vector<string> patVec;
		vector <pair<string, vector<string>>> patAsgTable;
		vector<pair<string, string>> patIfTable;
		vector<pair<string, string>> patWhileTable;

		vector<asgPat_t> asgPat;

		if (queryPT->getSynonym() != "") {
			PT = queryPT->getSynonym();
			string pType = utility.getSymMean(PT);
			ptFirst = queryPT->getFirstAttr();
			ptSecond = queryPT->getSecondAttr();

			if (pType == "assign") {
				handlePT.handleAssign(ptFirst, ptSecond, patVec, patAsgTable, asgPat);
				/*final = patVec;
				final.push_back("wtf " + to_string(patVec.size()));
				return final;*/
			}
			if (pType == "if") {
				handlePT.handleIf(ptFirst, ptSecond, patVec, patIfTable);
				/*final.push_back("wtf " + to_string(patIfTable.size()));
				return final;*/
				/*for (size_t i = 0; i < patIfTable.size(); i++) {
					final.push_back(patIfTable[i].first);
				}
				return final;*/
			}
			if (pType == "while") {
				handlePT.handleWhile(ptFirst, ptSecond, patVec, patWhileTable);
				/*	for (size_t i = 0; i < patWhileTable.size(); i++) {
					final.push_back(patWhileTable[i].first);
					}
					return final;*/
			}
			for (int i = 0; i < 4; i++) {
				PTCheck.push_back(0);
			}

			RSEntry_t currentRS;
			currentRS.synCount = 0;
			if (utility.getSymMean(ptFirst) != "") {
				currentRS.secondAtt = ptFirst;
				currentRS.synCount++;
			}
			else {
				currentRS.firstAtt = "";
			}
			/*if (utility.getSymMean(stSecond) != "") {
				currentRS.secondAtt = stSecond;
				currentRS.synCount++;
			}
			else {
				currentRS.secondAtt = "";
			}*/
			currentRS.firstAtt = PT;
			currentRS.synCount++;

			if (!patVec.empty()) {
				PTCheck[0] = 1;
				currentRS.vec = patVec;
			}
			if (!patAsgTable.empty()) {
				PTCheck[1] = 1;
				currentRS.table = patAsgTable;
			}
			if (!patIfTable.empty()) {
				PTCheck[2] = 1;
				currentRS.ssTable = patIfTable;
			}
			if (!patWhileTable.empty()) {
				PTCheck[3] = 1;
				currentRS.ssTable = patWhileTable;
			}
			if (utility.getPos(PTCheck) == -1) {
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
		if (utility.contain(attList, queryRS[i].firstAtt) == -1) {
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
		if (utility.contain(attList, queryRS[i].secondAtt) == -1) {
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
		int changed = 0;
		for (size_t j = 0; j < pos.size(); j++) {
			//Case 1 syn
			if (queryRS[pos[j]].synCount == 1) {
				for (size_t k = 0; k < pos.size(); k++) {
					if (queryRS[pos[k]].synCount == 1) {
						int oldJSize = queryRS[pos[j]].vec.size();
						int oldKSize = queryRS[pos[k]].vec.size();
						changed += utility.intersectionSS(queryRS[pos[j]].vec, queryRS[pos[k]].vec, 1);
					}
					if (queryRS[pos[k]].synCount == 2) {
						if (queryRS[pos[k]].table.size()>0 && var == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPSV(queryRS[pos[j]].vec, queryRS[pos[k]].table, 1);
						}
						if (queryRS[pos[k]].table.size()>0 && var == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPSV(queryRS[pos[j]].vec, queryRS[pos[k]].table, 2);
						}
						if (queryRS[pos[k]].ssTable.size() > 0 && var == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPSS(queryRS[pos[j]].vec, queryRS[pos[k]].ssTable, 1);
						}
						if (queryRS[pos[k]].ssTable.size() > 0 && var == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPSS(queryRS[pos[j]].vec, queryRS[pos[k]].ssTable, 2);
						}
					}
				}
			}

			//Case 2 syns
			if (queryRS[pos[j]].synCount == 2) {
				for (size_t k = 0; k < pos.size(); k++) {
					if (queryRS[pos[k]].synCount == 1) {
						if (queryRS[pos[j]].table.size()>0 && var == queryRS[pos[j]].firstAtt) {
							changed += utility.intersectionPSV(queryRS[pos[k]].vec, queryRS[pos[j]].table, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && var == queryRS[pos[j]].secondAtt) {
							changed += utility.intersectionPSV(queryRS[pos[k]].vec, queryRS[pos[j]].table, 2);
						}
						if (queryRS[pos[j]].ssTable.size() > 0 && var == queryRS[pos[j]].firstAtt) {
							changed += utility.intersectionPSS(queryRS[pos[k]].vec, queryRS[pos[j]].ssTable, 1);
						}
						if (queryRS[pos[j]].ssTable.size() > 0 && var == queryRS[pos[j]].secondAtt) {
							changed += utility.intersectionPSS(queryRS[pos[k]].vec, queryRS[pos[j]].ssTable, 2);
						}
					}

					if (queryRS[pos[k]].synCount == 2) {
						//case table vs table
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 2);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							//final.push_back("old size " + to_string(queryRS[pos[j]].table.size()));
							changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 2);
							/*int temp = utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 2);
							final.push_back("new size " + to_string(queryRS[pos[j]].table.size()));
							final.push_back("temp " + to_string(temp));
							return final;*/
						}

						//Case foltable vs foltable
						if (queryRS[pos[j]].ssTable.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 1, 1);
						}
						if (queryRS[pos[j]].ssTable.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 1, 2);
						}
						if (queryRS[pos[j]].ssTable.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 2, 1);
						}
						if (queryRS[pos[j]].ssTable.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 2, 2);
						}

						//Case foltable vs table
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 1, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 2, 1);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 1, 2);
						}
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].ssTable.size() > 0 &&
							queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 2, 2);
						}

						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].ssTable.size() > 0 &&
							queryRS[pos[k]].firstAtt == queryRS[pos[j]].firstAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 1, 1);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].ssTable.size() > 0 &&
							queryRS[pos[k]].firstAtt == queryRS[pos[j]].secondAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 2, 1);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].ssTable.size() > 0 &&
							queryRS[pos[k]].secondAtt == queryRS[pos[j]].firstAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 1, 2);
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].ssTable.size() > 0 &&
							queryRS[pos[k]].secondAtt == queryRS[pos[j]].secondAtt) {
							changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 2, 2);
						}
					}
				}
			}
		}
		loopList.erase(loopList.begin());
		if (changed > 0) {
			for (size_t j = 0; j < attList[i].reAtt.size(); j++) {
				if (!utility.contain(loopList, attList[i].reAtt[j])) {
					loopList.push_back(attList[i].reAtt[j]);
				}
			}
		}
	}
	//Check if any intersection results in empty vector
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (queryRS[i].vec.empty() && queryRS[i].table.empty() && queryRS[i].ssTable.empty()) {
			return final;
		}
	}

	//Check case select not equal to each attribute
	bool checkRS = false;
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (queryRS[i].firstAtt == rs || queryRS[i].secondAtt == rs) {
			checkRS = true;
			break;
		}
		//final.push_back(rs + " " + queryRS[i].firstAtt + " " + queryRS[i].secondAtt);
	}
	/*final.push_back(rs);
	return final;*/

	if (!checkRS) {
		if (selType == "prog_line" || selType == "stmt") {
			final = progLine->getLinesOfType("prog_line");
		}
		if (selType == "variable") {
			final = PKB::getVarTable()->getTable();
		}
		if (selType == "constant") {
			vector<ConstEntry_t> constTable = PKB::getConstTable()->getTable();
			for (size_t i = 0; i < constTable.size(); i++) {
				for (size_t j = 0; j < constTable[i].constants.size(); j++) {
					if (!utility.contain(final, constTable[i].constants[j])) {
						final.push_back(constTable[i].constants[j]);
					}
				}
			}
		}
		if (selType == "procedure") {
			final = PKB::getProcTable()->getTable();
		}
		if (selType == "assign") {
			final = progLine->getLinesOfType("assign");
		}
		if (selType == "while") {
			final = progLine->getLinesOfType("while");
		}
		if (selType == "if") {
			final = progLine->getLinesOfType("if");
		}
		handleRS.rmEString(final);
		return final;
	}
	//if (queryRS.size() > 0) {
	//	//final = queryRS[0].vec;
	//	final.push_back("after " + to_string(queryRS[0].ssTable.size()));
	//}
	//return final;

	//Case select within any clause (at this point no more empty vector)
	for (size_t i = 0; i < attList.size(); i++) {
		if (rs == attList[i].att) {
			//Case at least 1 1syn clause is the rs
			bool found = false;
			for (size_t j = 0; j < attList[i].reClause.size(); j++) {
				if (queryRS[attList[i].reClause[j]].synCount == 1) {
					final = queryRS[attList[i].reClause[j]].vec;
					found = true;
				}
				break;
			}

			//Case select from 2 syn

			if (!found) {
				for (size_t j = 0; j < attList[i].reClause.size(); j++) {
					if (queryRS[attList[i].reClause[j]].synCount == 2) {
						if (rs == queryRS[attList[i].reClause[j]].firstAtt && queryRS[attList[i].reClause[j]].table.size()>0) {

							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].table.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].table[k].first);
							}
						}
						/*if (queryRS[attList[i].reClause[j]].table.size()>0) {
							final.push_back(queryRS[attList[i].reClause[j]].firstAtt);
							return final;
						}*/
						if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].table.size()>0) {
							/*final.push_back("wtf "+to_string(queryRS[attList[i].reClause[j]].table.size()));
							return final;*/
							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].table.size(); k++) {
								for (size_t h = 0; h < queryRS[attList[i].reClause[j]].table[k].second.size(); h++) {
									if (!utility.contain(final, queryRS[attList[i].reClause[j]].table[k].second[h])) {
										final.push_back(queryRS[attList[i].reClause[j]].table[k].second[h]);
									}
								}
							}
							//return final;
						}
						if (rs == queryRS[attList[i].reClause[j]].firstAtt && queryRS[attList[i].reClause[j]].ssTable.size()>0) {
							/*	final.push_back(queryRS[attList[i].reClause[j]].firstAtt + " " + rs);
								return final;*/
							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].ssTable.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].ssTable[k].first);
							}
							return final;
						}
						if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].ssTable.size()>0) {
							for (size_t k = 0; k < queryRS[attList[i].reClause[j]].ssTable.size(); k++) {
								final.push_back(queryRS[attList[i].reClause[j]].ssTable[k].second);
							}
						}
					}
				}
			}
		}
	}

	/*if (final.size() > 0 && selType == "assign") {
		final = utility.intersection(final, utility.getAssignTable());
	}
	if (selType == "prog_line" || selType == "stmt") {
		final = utility.intersection(final, progLine->getLinesOfType("prog_line"));
	}
	if (selType == "while") {
		final = utility.intersection(final, progLine->getLinesOfType("while"));
	}
	if (selType == "if") {
		final = utility.intersection(final, progLine->getLinesOfType("if"));
	}
	if (selType == "procedure") {
		final = utility.intersection(final, progLine->getLinesOfType("procedure"));
	}*/
	//handleRS.rmEString(final);
	return final;
}