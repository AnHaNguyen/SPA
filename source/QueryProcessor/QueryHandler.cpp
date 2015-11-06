#include "../QueryProcessor/QueryHandler.h"

QueryHandler::QueryHandler() {}
QueryHandler::~QueryHandler() {}


vector<string> QueryHandler::queryRec(QueryTree* queryTree) {

	//Initiate global values
	PreResultNode* result;
	PreSuchThatNode* queryST;
	PrePatternNode* queryPT;
	PreWithNode* queryWith;
	string stFirst;
	string stSecond;
	string ptFirst;
	string ptSecond;
	string withFirst;
	string withSecond;
	string ST;
	string PT;
	vector<RSEntry_t> queryRS;
	HandleRS handleRS = HandleRS();
	HandleST handleST = HandleST();
	HandlePT handlePT = HandlePT();
	HandleWith handleWith = HandleWith();
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
	//final.push_back("valid");
	//return final;

	//Initialize symtable for HUtility
	if (queryTree->getSymbolTable().empty()) {
		return final;
	}
	else {
		utility.setSymTable(queryTree->getSymbolTable());
	}


	//Handle suchThat
	queryST = queryTree->getSuchThat();
	while (true) {
		vector<string> folVec;
		vector<pair<string, string>>  folTable;

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

		vector<string> affVec;
		vector <pair<string, vector<string>>> affTable;


		if (queryST->getSynonym() != "") {
			ST = queryST->getSynonym();
			stFirst = queryST->getFirstAttr();
			stSecond = queryST->getSecondAttr();

			//Handle follows
			if (ST == "Follows") {
				handleST.handleFollows(stFirst, stSecond, folVec);
				if (folVec.size() > 0 && folVec.front() == "all") {
					utility.getFollowTable(folTable);
					handleRS.checkPSS(folTable, stFirst, stSecond);
				}
				if (folVec.size() > 0 && folVec.front() != "all" && folVec.front() != "true" && folVec.front() != "") {
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
					handleST.handleFollows(stFirst, stSecond, folVec);
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
				handleST.handleModifies(stFirst, stSecond, modVec);
				if (modVec.size() > 0 && modVec.front() == "all") {
					utility.getModifyTable(modTable);
					handleRS.checkPSV(modTable, stFirst, stSecond);
				}
				else if (modVec.size() > 0 && modVec.front() != "true" && modVec.front() != "") {
					handleRS.checkSS(modVec, stFirst, stSecond);
				}
			}

			//Handle parent
			if (ST == "Parent") {
				handleST.handleParent(stFirst, stSecond, parVec);
				if (parVec.size() > 0 && parVec.front() == "all") {
					utility.getParentTable(parTable);
					handleRS.checkPSV(parTable, stFirst, stSecond);
				}
				else if (parVec.size() > 0 && parVec.front() != "true" && parVec.front() != "") {
					handleRS.checkSS(parVec, stFirst, stSecond);
				}
			}

			//Handle parent*
			if (ST == "Parent*") {
				ParentSTable* parTab = PKB::getParentSTable();
				//Check case Parent*(1, 2)
				if (utility.isInt(stFirst) && utility.isInt(stSecond)) {
					if (parTab->isParentS(stFirst, stSecond)) {
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
				else if (useVec.size() > 0 && useVec.front() != "true" && useVec.front() != "") {
					handleRS.checkSS(useVec, stFirst, "");
				}
			}

			//Handle Next
			if (ST == "Next") {
				handleST.handleNext(stFirst, stSecond, nextVec);
				if (nextVec.size() > 0 && nextVec.front() == "all") {
					utility.getNextTable(nextTable);
					handleRS.checkPSV(nextTable, stFirst, stSecond);
					/*final = nextVec;
					return final;*/
				}
				else if (nextVec.size() > 0 && nextVec.front() != "true" && nextVec.front() != "") {
					handleRS.checkSS(nextVec, stFirst, stSecond);
				}
			}

			//Handle Next*
			if (ST == "Next*") {
				NextTable* nextTab = PKB::getNextTable();
				//Check case Next*(1, 2)
				if (utility.isInt(stFirst) && utility.isInt(stSecond)) {
					if (nextTab->isNextS(stFirst, stSecond)) {
						nextVec.push_back("true");
					}
				}
				else {
					handleST.handleNext(stFirst, stSecond, nextVec);
					if (!nextVec.empty()) {
						if (nextVec.size() > 0 && nextVec.front() == "all") {
							utility.getNextTable(nextTable);
							handleRS.checkPSV(nextTable, stFirst, stSecond);
						}
						else {
							//Case Next*(1, s)
							if (utility.isInt(stFirst)) {
								nextVec = nextTab->getNextS(stFirst);
							}
							if (utility.isInt(stSecond)) {
								nextVec = nextTab->getPrevS(stSecond);
							}
							handleRS.checkSS(nextVec, stFirst, stSecond);
						}
					}
				}
			}

			//Handle Calls
			if (ST == "Calls") {
				handleST.handleCalls(stFirst, stSecond, callVec);
				if (callVec.size() > 0 && callVec.front() == "all") {
					utility.getCallTable(callTable);
				}
			}

			//Handle Calls*
			if (ST == "Calls*") {
				CallSTable* callSTab = PKB::getCallSTable();
				pair<string, bool> firstAttQ;
				pair<string, bool> secondAttQ;
				HUtility().checkQuotation(firstAttQ, stFirst);
				HUtility().checkQuotation(secondAttQ, stSecond);

				//Check case Calls*("ABC", "DEF")
				if (utility.getSymMean(stFirst) == "" && utility.getSymMean(stSecond) == "") {
					if (callSTab->isCallS(firstAttQ.first, secondAttQ.first)) {
						callVec.push_back("true");
					}
				}
				else {
					handleST.handleCalls(stFirst, stSecond, callVec);
					if (!callVec.empty()) {
						if (callVec.size() > 0 && callVec.front() == "all") {
							utility.getCallTable(callTable);
							handleRS.checkPSV(callTable, stFirst, stSecond);
						}
						else {
							//Case Call*(1, s)
							if (utility.isInt(stFirst)) {
								callVec = callSTab->getCalleeS(stFirst);
							}
							if (utility.isInt(stSecond)) {
								callVec = callSTab->getCallerS(stSecond);
							}
						}
					}
				}
			}

			//Handle affects
			if (ST == "Affects") {
				handleST.handleAffect(stFirst, stSecond, affVec);
				if (affVec.size() > 0 && affVec.front() == "all") {
					affTable = PKB::affect();
				}
			}

			//Handle affects*
			if (ST == "Affects*") {
				//Check case Affect*(1, 2)
				if (HUtility().isInt(stFirst) && HUtility().isInt(stSecond)) {
					affVec = PKB::affectS(stFirst, stSecond);
				}
				else {
					handleST.handleAffect(stFirst, stSecond, affVec);
					if (affVec.size() > 0) {
						if (affVec.size() > 0 && affVec.front() == "all") {
							affTable = PKB::affect();
						}
					}
					else {
						//Case Affect*(1, n)
						if (HUtility().isInt(stFirst)) {
							affVec = PKB::affectS(stFirst, "_");
						}
						if (HUtility().isInt(stSecond)) {
							affVec = PKB::affectS("_", stSecond);
						}
					}
				}
			}

			//Populate queryRS
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

			for (int i = 0; i < 14; i++) {
				STCheck.push_back(0);
			}
			if (!folVec.empty() && folVec.front() != "all" && folVec.front() != "") {
				STCheck[0] = 1;
				currentRS.vec = folVec;
			}
			if (!folTable.empty()) {
				STCheck[1] = 1;
				currentRS.ssTable = folTable;
			}
			if (!modVec.empty() && modVec.front() != "all" && modVec.front() != "") {
				STCheck[2] = 1;
				currentRS.vec = modVec;
			}
			if (!modTable.empty()) {
				STCheck[3] = 1;
				currentRS.table = modTable;
			}
			if (!parVec.empty() && parVec.front() != "all" && parVec.front() != "") {
				STCheck[4] = 1;
				currentRS.vec = parVec;
			}
			if (!parTable.empty()) {
				STCheck[5] = 1;
				currentRS.table = parTable;
			}
			if (!useVec.empty() && useVec.front() != "all" && useVec.front() != "") {
				STCheck[6] = 1;
				currentRS.vec = useVec;
			}
			if (!useTable.empty()) {
				STCheck[7] = 1;
				currentRS.table = useTable;
			}
			if (!nextVec.empty() && nextVec.front() != "all" && nextVec.front() != "") {
				STCheck[8] = 1;
				currentRS.vec = nextVec;
			}
			if (!nextTable.empty()) {
				STCheck[9] = 1;
				currentRS.table = nextTable;
			}
			if (!callVec.empty() && callVec.front() != "all" && callVec.front() != "") {
				STCheck[10] = 1;
				currentRS.vec = callVec;
			}
			if (!callTable.empty()) {
				STCheck[11] = 1;
				currentRS.table = callTable;
			}
			if (!affVec.empty() && affVec.front() != "all" && affVec.front() != "") {
				STCheck[12] = 1;
				currentRS.vec = affVec;
			}
			if (!affTable.empty()) {
				STCheck[13] = 1;
				currentRS.table = affTable;
			}

			//Check if all empty
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


		if (queryPT->getSynonym() != "") {
			PT = queryPT->getSynonym();
			string pType = utility.getSymMean(PT);
			ptFirst = queryPT->getFirstAttr();
			ptSecond = queryPT->getSecondAttr();

			if (pType == "assign") {
				handlePT.handleAssign(ptFirst, ptSecond, patVec, patAsgTable);
			}
			if (pType == "if") {
				handlePT.handleIf(ptFirst, ptSecond, patVec, patIfTable);
			}
			if (pType == "while") {
				handlePT.handleWhile(ptFirst, ptSecond, patVec, patWhileTable);
			}

			//Populate QueryRS
			RSEntry_t currentRS;
			currentRS.synCount = 0;
			if (utility.getSymMean(ptFirst) != "") {
				currentRS.secondAtt = ptFirst;
				currentRS.synCount++;
			}
			else {
				currentRS.firstAtt = "";
			}
			currentRS.firstAtt = PT;
			currentRS.synCount++;

			for (int i = 0; i < 4; i++) {
				PTCheck.push_back(0);
			}
			if (!patVec.empty() && patVec.front() != "all" && patVec.front() != "") {
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


	//Handle with
	queryWith = queryTree->getWith();
	while (true) {
		vector<string> withVec;

		if (queryWith->getLeftType() != "") {
			final.push_back("wtf1");
			withFirst = queryWith->getLeftType();
		}
		else {
			final.push_back("wtf2");
			withFirst = queryWith->getLeftAttrRef().getSynonym();
		}
		if (queryWith->getRightType() == "2") {
			final.push_back("wtf3");
			withSecond = queryWith->getRightType();
		}
		else {
			final.push_back("wtf4");
			withSecond = queryWith->getRightAttrRef().getSynonym();
		}
		final.push_back(queryWith->getRightType());
		return final;
		handleWith.handleWith(withVec, withFirst, withSecond);
		if (withVec.empty()) {
			return final;
		}
		
		//Populate queryRS
		RSEntry_t currentRS;
		currentRS.synCount = 0;
		if (utility.getSymMean(withFirst) != "") {
			currentRS.firstAtt = withFirst;
			currentRS.synCount = 1;
		}
		else {
			currentRS.firstAtt = "";
		}
		if (utility.getSymMean(withSecond) != "") {
			currentRS.secondAtt = withSecond;
			currentRS.synCount = 1;
		}
		else {
			currentRS.secondAtt = "";
		}
		currentRS.vec = withVec;
		if (queryWith->getNext() != NULL) {
			queryWith = queryWith->getNext();
		}
		else {
			break;
		}
	}


	//Find all dependency of syn and clauses
	//Populate attList's atts and reClause
	vector <attEntry_t> attList;
	for (size_t i = 0; i < queryRS.size(); i++) {
		//Case att not found, add att and all clauses where att is found
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
		//Find all attEntry that this att is related with
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
	//Each iteration processes the 1st item in the queue
	while (loopList.size() > 0) {
		int i = loopList.front();
		string var = attList[i].att;
		vector<int> pos = attList[i].reClause;
		int changed = 0;
		for (size_t j = 0; j < pos.size(); j++) {
			//Case 1 syn
			if (queryRS[pos[j]].synCount == 1) {
				for (size_t k = 0; k < pos.size(); k++) {
					if (j != k && queryRS[pos[k]].synCount == 1) {
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

					if (j != k && queryRS[pos[k]].synCount == 2) {
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
							changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 2);
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
		//If some the item's reClauses change then enqueue all reAtt that are not inside the list already (some of these will change too)
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


	//Handle select
	string selType = HandleRS().handleSelect(queryTree, result);
	if (selType == "") {
		return final;
	}

	vector<vector<string>> finalVec;
	//Check case both control parameters are in the same clause
	bool checkSC = true;
	if (result->getNext() != NULL) {
		string var1 = result->getResult().getSynonym();
		string var2 = result->getNext()->getResult().getSynonym();
		vector<string> finalPart1;
		vector<string> finalPart2;

		for (size_t i = 0; i < queryRS.size(); i++) {
			if (queryRS[i].firstAtt == var1 && queryRS[i].secondAtt == var2) {
				if (!queryRS[i].vec.empty()) {
					for (size_t j = 0; j < queryRS[i].vec.size(); j++) {
						final.push_back(queryRS[i].vec[j]+" " + queryRS[i].vec[j]);
					}
				}
				if (!queryRS[i].ssTable.empty()) {
					for (size_t j = 0; j < queryRS[i].ssTable.size(); j++) {
						final.push_back(queryRS[i].ssTable[j].first + " " + queryRS[i].ssTable[j].second);
					}
				}
				if (!queryRS[i].table.empty()) {
					for (size_t j = 0; j < queryRS[i].table.size(); j++) {
						for (size_t k = 0; k < queryRS[i].table[j].second.size(); k++) {
							final.push_back(queryRS[i].table[j].first + " " + queryRS[i].table[j].second[k]);
						}
					}
				}
				checkSC = false;
				break;
			}
			if (queryRS[i].firstAtt == var2 && queryRS[i].secondAtt == var1) {
				if (!queryRS[i].vec.empty()) {
					for (size_t j = 0; j < queryRS[i].vec.size(); j++) {
						final.push_back(queryRS[i].vec[j] + " " + queryRS[i].vec[j]);
					}
				}
				if (!queryRS[i].ssTable.empty()) {
					for (size_t j = 0; j < queryRS[i].ssTable.size(); j++) {
						final.push_back(queryRS[i].ssTable[j].second + " " + queryRS[i].ssTable[j].first);
					}
				}
				if (!queryRS[i].table.empty()) {
					for (size_t j = 0; j < queryRS[i].table.size(); j++) {
						for (size_t k = 0; k < queryRS[i].table[j].second.size(); k++) {
							final.push_back(queryRS[i].table[j].second[k]+ " " + queryRS[i].table[j].first);
						}
					}
				}
				checkSC = false;
				break;
			}
		}
		if (!checkSC) {
			return final;
		}
	}

	//Other cases
	while (checkSC) {
		string rs = result->getResult().getSynonym();
		vector<string> finalPart;
		//Check case select not equal to each attribute
		bool checkRS = false;
		for (size_t i = 0; i < queryRS.size(); i++) {
			if (queryRS[i].firstAtt == rs || queryRS[i].secondAtt == rs) {
				checkRS = true;
				break;
			}
		}

		if (!checkRS) {
			if (selType == "prog_line" || selType == "stmt") {
				finalPart = progLine->getLinesOfType("prog_line");
			}
			if (selType == "variable") {
				finalPart = PKB::getVarTable()->getTable();
			}
			if (selType == "constant") {
				vector<ConstEntry_t> constTable = PKB::getConstTable()->getTable();
				for (size_t i = 0; i < constTable.size(); i++) {
					for (size_t j = 0; j < constTable[i].constants.size(); j++) {
						if (!utility.contain(finalPart, constTable[i].constants[j])) {
							finalPart.push_back(constTable[i].constants[j]);
						}
					}
				}
			}
			if (selType == "procedure") {
				finalPart = PKB::getProcTable()->getTable();
			}
			if (selType == "assign") {
				finalPart = progLine->getLinesOfType("assign");
			}
			if (selType == "while") {
				finalPart = progLine->getLinesOfType("while");
			}
			if (selType == "if") {
				finalPart = progLine->getLinesOfType("if");
			}
		}
		else {
			//Return function normal case
			//Case select within any clause (at this point no more empty vector)
			for (size_t i = 0; i < attList.size(); i++) {
				if (rs == attList[i].att) {
					//Case at least 1 1syn clause is the rs
					bool found = false;
					for (size_t j = 0; j < attList[i].reClause.size(); j++) {
						if (queryRS[attList[i].reClause[j]].synCount == 1) {
							finalPart = queryRS[attList[i].reClause[j]].vec;
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
										finalPart.push_back(queryRS[attList[i].reClause[j]].table[k].first);
									}
								}
								if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].table.size()>0) {
									for (size_t k = 0; k < queryRS[attList[i].reClause[j]].table.size(); k++) {
										for (size_t h = 0; h < queryRS[attList[i].reClause[j]].table[k].second.size(); h++) {
											if (!utility.contain(finalPart, queryRS[attList[i].reClause[j]].table[k].second[h])) {
												finalPart.push_back(queryRS[attList[i].reClause[j]].table[k].second[h]);
											}
										}
									}
								}
								if (rs == queryRS[attList[i].reClause[j]].firstAtt && queryRS[attList[i].reClause[j]].ssTable.size()>0) {
									for (size_t k = 0; k < queryRS[attList[i].reClause[j]].ssTable.size(); k++) {
										finalPart.push_back(queryRS[attList[i].reClause[j]].ssTable[k].first);
									}
								}
								if (rs == queryRS[attList[i].reClause[j]].secondAtt && queryRS[attList[i].reClause[j]].ssTable.size()>0) {
									for (size_t k = 0; k < queryRS[attList[i].reClause[j]].ssTable.size(); k++) {
										finalPart.push_back(queryRS[attList[i].reClause[j]].ssTable[k].second);
									}
								}
							}
						}
					}
				}
			}
		}
		finalVec.push_back(finalPart);
		if (result->getNext() != NULL) {
			result = result->getNext();
		}
		else {
			break;
		}
	}
	if (finalVec.size() == 1) {
		final = finalVec[0];
	}
	if (finalVec.size() == 2) {
		for (size_t i = 0; i < finalVec[0].size(); i++) {
			for (size_t j = 0; j < finalVec[1].size(); j++) {
				final.push_back(finalVec[0][i] + " " + finalVec[1][j]);
			}
		}
	}

	return final;
}