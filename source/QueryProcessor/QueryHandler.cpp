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
		//final.push_back("invalid1");
		return final;
	}


	//Initialize symtable for HUtility
	utility.setSymTable(queryTree->getSymbolTable());

	//Handle select
	string selType = HandleRS().handleSelect(queryTree, result);
	if (selType == "") {
		//final.push_back("invalid3");
		return final;
	}

	//Handle suchThat
	queryST = queryTree->getSuchThat();
	while (true) {
		vector<string> folVec;
		vector<pair<string, vector<string>>>  folTable;

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

		vector<string> conVec;
		vector<pair<string, string>> conTable;

		vector<string> sibVec;
		vector<pair<string, string>> sibTable;

		if (queryST->getSynonym() != "") {
			ST = queryST->getSynonym();
			stFirst = queryST->getFirstAttr();
			stSecond = queryST->getSecondAttr();

			//Handle follows
			if (ST == "Follows") {
				handleST.handleFollows(stFirst, stSecond, folVec);
				if (folVec.size() > 0 && folVec.front() == "all") {
					utility.getFollowTable(folTable);
					handleRS.checkPSV(folTable, stFirst, stSecond);
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
					if (folVec.size() > 0) {
						if (folVec.front() == "all") {
							utility.getFollowSTable(folTable);
							handleRS.checkPSV(folTable, stFirst, stSecond);
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
					/*final.push_back(to_string(parTable.size()));
					return final;*/
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
					if (parVec.size() > 0) {
						if (parVec.front() == "all") {
							utility.getParentSTable(parTable);
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
						if (nextVec.front() == "all") {
							utility.getNextSTable(nextTable);
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
					//Case Next*(s, s)
					else if (stFirst == stSecond) {
						nextVec.clear();
						vector<NextEntry_t> nextSTable = PKB::getNextTable()->getNextSTable();
						for (size_t i = 0; i < nextSTable.size(); i++) {
							if (HUtility().contain(nextSTable[i].nextStmts, nextSTable[i].lineNo)) {
								nextVec.push_back(nextSTable[i].lineNo);
							}
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
					if (callVec.size() > 0) {
						if (callVec.front() == "all") {
							utility.getCallSTable(callTable);
							handleRS.checkPSV(callTable, stFirst, stSecond);
						}
						else {
							//Case Calls*(1, s)
							if (utility.getSymMean(stFirst) == "") {
								callVec = callSTab->getCalleeS(firstAttQ.first);
							}
							if (utility.getSymMean(stSecond) == "") {
								callVec = callSTab->getCallerS(secondAttQ.first);
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
						if (affVec.front() == "all") {
							affTable = PKB::affectS();
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
					//Case Affects*(s, s)
					else if (stFirst == stSecond) {
						affVec.clear();
						vector<pair<string, vector<string>>> affTable = PKB::affectS();
						for (size_t i = 0; i < affTable.size(); i++) {
							if (HUtility().contain(affTable[i].second, affTable[i].first)) {
								affVec.push_back(affTable[i].first);
							}
						}
					}
				}
			}
			//Handle contains
			if (ST == "Contains") {
				handleST.handleContain(conVec, conTable, stFirst, stSecond);
			}
			//Handle contains*
			if (ST == "Contains*") {
				handleST.handleContainS(conVec, conTable, stFirst, stSecond);
			}

			//Handle sibling
			if (ST == "Sibling") {
				handleST.handleSibling(sibVec, sibTable, stFirst, stSecond);
			}

			//Populate queryRS
			RSEntry_t currentRS;
			currentRS.synCount = 0;
			if (utility.getSymMean(stFirst) != "" || stFirst == "_") {
				currentRS.firstAtt = stFirst;
				currentRS.synCount++;
			}
			else {
				currentRS.firstAtt = "";
			}
			if (utility.getSymMean(stSecond) != "" || stSecond == "_") {
				currentRS.secondAtt = stSecond;
				currentRS.synCount++;
			}
			else {
				currentRS.secondAtt = "";
			}
			//Check case (n, n)
			if (stFirst == stSecond && stFirst != "_") {
				currentRS.synCount = 1;
			}
			for (int i = 0; i < 18; i++) {
				STCheck.push_back(0);
			}
			if (!folVec.empty() && folVec.front() != "all" && folVec.front() != "") {
				STCheck[0] = 1;
				currentRS.vec = folVec;
			}
			if (!folTable.empty()) {
				STCheck[1] = 1;
				currentRS.table = folTable;
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
			if (!affVec.empty() && affVec.front() != "all" && affVec.front() != ""&& affVec.front() != "false") {
				STCheck[12] = 1;
				currentRS.vec = affVec;
			}
			if (!affTable.empty()) {
				STCheck[13] = 1;
				currentRS.table = affTable;
			}
			if (!conVec.empty() && conVec.front() != "all" && conVec.front() != ""&& conVec.front() != "false") {
				STCheck[14] = 1;
				currentRS.vec = conVec;
			}
			if (!conTable.empty()) {
				STCheck[15] = 1;
				currentRS.ssTable = conTable;
			}
			if (!sibVec.empty() && sibVec.front() != "all" && sibVec.front() != ""&& sibVec.front() != "false") {
				STCheck[16] = 1;
				currentRS.vec = sibVec;
			}
			if (!sibTable.empty()) {
				STCheck[17] = 1;
				currentRS.ssTable = sibTable;
			}

			//Check if all empty
			if (utility.getPos(STCheck) == -1) {
				if (selType == "BOOLEAN") {
					final.push_back("false");
				}
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
				if (selType == "BOOLEAN") {
					final.push_back("false");
				}
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
		vector<pair<string, vector<string>>> withCall;
		if (queryWith->getLeftType() != "") {
			withFirst = queryWith->getLeftType();
		}
		else {
			withFirst = queryWith->getLeftAttrRef().getSynonym();
		}
		if (queryWith->getRightType() != "") {
			withSecond = queryWith->getRightType();
		}
		else {
			withSecond = queryWith->getRightAttrRef().getSynonym();
		}

		handleWith.handleWith(withVec, withCall, withFirst, withSecond);
		if (withVec.empty() && withCall.empty()) {
			if (selType == "BOOLEAN") {
				final.push_back("false");
			}
			return final;
		}


		//Populate queryRS
		RSEntry_t currentRS;
		currentRS.synCount = 0;
		if (utility.getSymMean(withFirst) != "") {
			//Case withFirst is call
			if (utility.getSymMean(withFirst) == "call") {
				//Case c.procName = p.procName
				if (utility.getSymMean(withSecond) == "variable" || utility.getSymMean(withSecond) == "procedure") {
					if (queryWith->getLeftAttrRef().getAttr() == "procName") {
						currentRS.firstAtt = withSecond;
						currentRS.secondAtt = withFirst;
					}
					currentRS.synCount = 2;
				}
				//Case c.stmt# = ...
				if (utility.getSymMean(withSecond) != "" &&
					utility.getSymMean(withSecond) != "variable" && utility.getSymMean(withSecond) != "procedure") {
					if (queryWith->getLeftAttrRef().getAttr() == "stmt#") {
						currentRS.firstAtt = withFirst;
						currentRS.secondAtt = withSecond;
					}
					currentRS.synCount = 2;
				}
				//Case withSecond is a string
				if (utility.getSymMean(withSecond) == "") {
					currentRS.firstAtt = withFirst;
					currentRS.synCount = 2;
				}
			}
			//Case withFirst are others
			if (utility.getSymMean(withFirst) != "call" && utility.getSymMean(withFirst) != "") {
				//Case both synonyms
				if (utility.getSymMean(withSecond) != "") {
					currentRS.firstAtt = withFirst;
					currentRS.secondAtt = withSecond;
					currentRS.synCount = 1;
				}
				//Case withSecond is string
				if (utility.getSymMean(withSecond) == "") {
					currentRS.firstAtt = withFirst;
					currentRS.synCount = 1;
				}
			}
		}
		currentRS.vec = withVec;
		currentRS.table = withCall;
		queryRS.push_back(currentRS);

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
		for (size_t j = 0; j < attList.size(); j++) {
			for (size_t k = 0; k < attList[i].reClause.size(); k++) {
				if (utility.contain(attList[j].reClause, attList[i].reClause[k])) {
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

	int w = 0;
	//Each iteration processes the 1st item in the queue
	while (loopList.size() > 0) {
		int i = loopList.front();
		string var = attList[i].att;
		vector<int> pos = attList[i].reClause;
		int changed = 0;
		/*string temp;
		temp += to_string(w) + " " + "a: ";
		for (size_t w1 = 0; w1 < queryRS[0].ssTable.size(); w1++) {
			temp += " " + queryRS[0].ssTable[w1].first;
		}
		final.push_back(temp);
		w++;*/
		for (size_t j = 0; j < pos.size(); j++) {
			//Case 1 syn
			if (queryRS[pos[j]].synCount == 1) {
				for (size_t k = j + 1; k < pos.size(); k++) {
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
				for (size_t k = j + 1; k < pos.size(); k++) {
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
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].table.size() > 0) {
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 1, 2);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSV(queryRS[pos[j]].table, queryRS[pos[k]].table, 2, 2);
							}
						}

						//Case foltable vs foltable
						if (queryRS[pos[j]].ssTable.size() > 0 && queryRS[pos[k]].ssTable.size() > 0) {
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 1, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 2, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 1, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 1, 2);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 2, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSS(queryRS[pos[j]].ssTable, queryRS[pos[k]].ssTable, 2, 2);
							}
						}
						//Case foltable vs table
						if (queryRS[pos[j]].table.size() > 0 && queryRS[pos[k]].ssTable.size() > 0) {
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 1, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 2, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 1, 1);
							}
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 2, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 1, 2);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[k]].ssTable, queryRS[pos[j]].table, 2, 2);
							}
						}
						if (queryRS[pos[k]].table.size() > 0 && queryRS[pos[j]].ssTable.size() > 0) {
							if (queryRS[pos[j]].firstAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].secondAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 1, 1);
							}
							if (queryRS[pos[j]].secondAtt == queryRS[pos[k]].firstAtt &&
								queryRS[pos[j]].firstAtt == queryRS[pos[k]].secondAtt) {
								changed += utility.specialPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 2, 1);
							}
							if (queryRS[pos[k]].firstAtt == queryRS[pos[j]].firstAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 1, 1);
							}
							if (queryRS[pos[k]].firstAtt == queryRS[pos[j]].secondAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 2, 1);
							}
							if (queryRS[pos[k]].secondAtt == queryRS[pos[j]].firstAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 1, 2);
							}
							if (queryRS[pos[k]].secondAtt == queryRS[pos[j]].secondAtt) {
								changed += utility.intersectionPPSM(queryRS[pos[j]].ssTable, queryRS[pos[k]].table, 2, 2);
							}
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
	//return final;

	//Check if any intersection results in empty vector
	for (size_t i = 0; i < queryRS.size(); i++) {
		if (queryRS[i].vec.empty() && queryRS[i].table.empty() && queryRS[i].ssTable.empty()) {
			if (selType == "BOOLEAN") {
				final.push_back("false");
			}
			return final;
		}
	}

	//At this point all clauses are not empty
	if (selType == "BOOLEAN") {
		final.push_back("true");
		return final;
	}



	//Case normal select
	vector<vector<string>> finalVec;
	//Case 2 control parameters
	bool checkSC = true;
	if (result->getNext() != NULL) {
		string var1 = result->getResult().getSynonym();
		string var2 = result->getNext()->getResult().getSynonym();
		vector<string> finalPart1;
		vector<string> finalPart2;

		//Check case both control parameters are in the same clause
		for (size_t i = 0; i < queryRS.size(); i++) {
			if (queryRS[i].firstAtt == var1 && queryRS[i].secondAtt == var2) {
				if (!queryRS[i].vec.empty()) {
					for (size_t j = 0; j < queryRS[i].vec.size(); j++) {
						final.push_back(queryRS[i].vec[j] + " " + queryRS[i].vec[j]);
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
							final.push_back(queryRS[i].table[j].second[k] + " " + queryRS[i].table[j].first);
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


		//Case 1 para is found in 2 syn clause and the other found in another (n1, n2) (n2, n3)
		vector<int> vertices;
		vector<int> controlV1;
		vector<int> controlV2;
		vector<int> SSSP;
		vector<int> curP;
		int curV;
		int begin;
		int end;


		//Find all clauses that have 2 syns and those that have begin and end nodes
		for (size_t i = 0; i < queryRS.size(); i++) {
			if (queryRS[i].synCount == 2 &&
				(!queryRS[i].table.empty() || !queryRS[i].ssTable.empty())) {
				if (queryRS[i].firstAtt == var1 || queryRS[i].secondAtt == var1) {
					controlV1.push_back(i);
				}
				else if (queryRS[i].firstAtt == var2 || queryRS[i].secondAtt == var2) {
					controlV2.push_back(i);
				}
				vertices.push_back(i);
			}
		}

		if (!controlV1.empty() && !controlV2.empty()) {
			size_t maxSize = queryRS.size();

			//Initializing adjList
			vector<vector<int>> adjList;
			for (size_t i = 0; i < maxSize; i++) {
				vector<int> temp;
				adjList.push_back(temp);
			}
			for (size_t i = 0; i < vertices.size(); i++) {
				for (size_t j = 0; j < vertices.size(); j++) {
					if (j != i && (queryRS[vertices[i]].firstAtt == queryRS[vertices[j]].firstAtt
						|| queryRS[vertices[i]].firstAtt == queryRS[vertices[j]].secondAtt
						|| queryRS[vertices[i]].secondAtt == queryRS[vertices[j]].firstAtt
						|| queryRS[vertices[i]].secondAtt == queryRS[vertices[j]].secondAtt)) {
						adjList[vertices[i]].push_back(vertices[j]);
					}
				}
			}

			//BFS
			vector<int> visited;
			vector<int> parent;
			for (size_t i = 0; i < maxSize; i++) {
				visited.push_back(0);
				parent.push_back(-1);
			}
			for (size_t i = 0; i < controlV1.size(); i++) {
				begin = controlV1[i];
				visited[begin] = 1;
				vector<int> qV;
				qV.push_back(begin);
				while (!qV.empty()) {
					curV = qV.front();
					qV.erase(qV.begin());
					for (size_t j = 0; j < adjList[curV].size(); j++) {
						if (visited[adjList[curV][j]] == 0) {
							visited[adjList[curV][j]] = 1;
							parent[adjList[curV][j]] = curV;
							if (HUtility().contain(controlV2, adjList[curV][j])) {
								end = adjList[curV][j];
								break;
							}
							else {
								qV.push_back(adjList[curV][j]);
							}
						}
					}
				}
				//Backtracking
				curV = end;
				while (curV != begin) {
					curP.push_back(curV);
					curV = parent[curV];
				}
				curP.push_back(begin);
				if (SSSP.empty() || curP.size() < SSSP.size()) {
					SSSP = curP;
				}
			}
			//Check if there exist a SSSP
			if (!SSSP.empty()) {
				checkSC = false;
			}
			//Processing from SSSP
			vector<pair<vector<string>, vector<string>>> finalVec;
			string comAtt;
			int startPos;
			vector<string> tempVec;

			//Find common att
			if (queryRS[SSSP[SSSP.size() - 1]].firstAtt == var1) {
				startPos = 1;
				comAtt = queryRS[SSSP[SSSP.size() - 1]].secondAtt;
			}
			else {
				startPos = 2;
				comAtt = queryRS[SSSP[SSSP.size() - 1]].firstAtt;
			}
			if (!queryRS[SSSP[SSSP.size() - 1]].table.empty()) {
				pair<vector<string>, vector<string>> temp;
				vector<pair<string, vector<string>>> table = queryRS[SSSP[SSSP.size() - 1]].table;
				for (size_t i = 0; i < table.size(); i++) {
					temp.first.push_back(table[i].first);
					temp.second = table[i].second;
					finalVec.push_back(temp);
					temp.first.clear();
					temp.second.clear();
				}
			}
			else {
				pair<vector<string>, vector<string>> temp;
				vector<pair<string, string>> ssTable = queryRS[SSSP[SSSP.size() - 1]].ssTable;
				for (size_t i = 0; i < ssTable.size(); i++) {
					temp.first.push_back(ssTable[i].first);
					temp.second.push_back(ssTable[i].second);
					finalVec.push_back(temp);
					temp.first.clear();
					temp.second.clear();
				}
			}

			//Bridging clauses
			for (size_t i = SSSP.size() - 2; i >= 0; i--) {
				if (queryRS[SSSP[i]].firstAtt == comAtt) {
					//Case (n1, n2) (n2, n3)
					if (startPos == 1) {
						//Replacing finalVec.second with new values
						if (!queryRS[SSSP[i]].table.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].table.size(); m++) {
									if (HUtility().contain(finalVec[j].second, queryRS[SSSP[i]].table[m].first)) {
										for (size_t n = 0; n < queryRS[SSSP[i]].table[m].second.size(); n++) {
											if (!HUtility().contain(tempVec, queryRS[SSSP[i]].table[m].second[n])) {
												tempVec.push_back(queryRS[SSSP[i]].table[m].second[n]);
											}
										}
									}
								}
								comAtt = queryRS[SSSP[i]].secondAtt;
								finalVec[j].second = tempVec;
								tempVec.clear();
							}
						}
						else if (!queryRS[SSSP[i]].ssTable.empty()) {

							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].ssTable.size(); m++) {
									if (HUtility().contain(finalVec[j].second, queryRS[SSSP[i]].ssTable[m].first)) {
										if (!HUtility().contain(tempVec, queryRS[SSSP[i]].ssTable[m].second)) {
											tempVec.push_back(queryRS[SSSP[i]].ssTable[m].second);
										}
									}
								}
								comAtt = queryRS[SSSP[i]].secondAtt;
								finalVec[j].second = tempVec;
								tempVec.clear();
							}
						}
					}
					//Case (n2, n1) (n2, n3)
					else if (startPos == 2) {
						//Replacing finalVec.first with new values
						if (!queryRS[SSSP[i]].table.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].table.size(); m++) {
									if (HUtility().contain(finalVec[j].first, queryRS[SSSP[i]].table[m].first)) {
										for (size_t n = 0; n < queryRS[SSSP[i]].table[m].second.size(); n++) {
											if (!HUtility().contain(tempVec, queryRS[SSSP[i]].table[m].second[n])) {
												tempVec.push_back(queryRS[SSSP[i]].table[m].second[n]);
											}
										}
									}
								}
								comAtt = queryRS[SSSP[i]].secondAtt;
								finalVec[j].first = tempVec;
								tempVec.clear();
							}
						}
						else if (!queryRS[SSSP[i]].ssTable.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].ssTable.size(); m++) {
									if (HUtility().contain(finalVec[j].second, queryRS[SSSP[i]].ssTable[m].first)) {
										if (!HUtility().contain(tempVec, queryRS[SSSP[i]].ssTable[m].second)) {
											tempVec.push_back(queryRS[SSSP[i]].ssTable[m].second);
										}
									}
								}
								comAtt = queryRS[SSSP[i]].secondAtt;
								finalVec[j].first = tempVec;
								tempVec.clear();
							}
						}
					}
				}
				else if (queryRS[SSSP[i]].secondAtt == comAtt) {
					//Case (n1, n2) (n3, n2)
					if (startPos == 1) {
						//Replacing finalVec.second with new values
						if (!queryRS[SSSP[i]].table.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].table.size(); m++) {
									for (size_t k = 0; k < queryRS[SSSP[i]].table[m].second.size(); k++) {
										if (HUtility().contain(finalVec[j].second, queryRS[SSSP[i]].table[m].second[k])) {
											if (!HUtility().contain(tempVec, queryRS[SSSP[i]].table[m].first)) {
												tempVec.push_back(queryRS[SSSP[i]].table[m].first);
											}
										}
									}
								}
								comAtt = queryRS[SSSP[i]].firstAtt;
								finalVec[j].second = tempVec;
								tempVec.clear();
							}
						}
						else if (!queryRS[SSSP[i]].ssTable.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].ssTable.size(); m++) {
									if (HUtility().contain(finalVec[j].second, queryRS[SSSP[i]].ssTable[m].second)) {
										if (!HUtility().contain(tempVec, queryRS[SSSP[i]].ssTable[m].first)) {
											tempVec.push_back(queryRS[SSSP[i]].ssTable[m].first);
										}
									}
								}
								comAtt = queryRS[SSSP[i]].firstAtt;
								finalVec[j].second = tempVec;
								tempVec.clear();
							}
						}
					}
					//Case (n2, n1) (n3, n2)
					else if (startPos == 2) {
						//Replacing finalVec.first with new values
						if (!queryRS[SSSP[i]].table.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].table.size(); m++) {
									for (size_t k = 0; k < queryRS[SSSP[i]].table[m].second.size(); k++) {
										if (HUtility().contain(finalVec[j].first, queryRS[SSSP[i]].table[m].second[k])) {
											if (!HUtility().contain(tempVec, queryRS[SSSP[i]].table[m].first)) {
												tempVec.push_back(queryRS[SSSP[i]].table[m].first);
											}
										}
									}
								}
								comAtt = queryRS[SSSP[i]].firstAtt;
								finalVec[j].first = tempVec;
								tempVec.clear();
							}
						}
						else if (!queryRS[SSSP[i]].ssTable.empty()) {
							for (size_t j = 0; j < finalVec.size(); j++) {
								for (size_t m = 0; m < queryRS[SSSP[i]].ssTable.size(); m++) {
									if (HUtility().contain(finalVec[j].first, queryRS[SSSP[i]].ssTable[m].second)) {
										if (!HUtility().contain(tempVec, queryRS[SSSP[i]].ssTable[m].first)) {
											tempVec.push_back(queryRS[SSSP[i]].ssTable[m].first);
										}
									}
								}
								comAtt = queryRS[SSSP[i]].firstAtt;
								finalVec[j].first = tempVec;
								tempVec.clear();
							}
						}
					}
				}
				if (i == 0) {
					break;
				}
			}
			if (startPos == 1) {
				for (size_t i = 0; i < finalVec.size(); i++) {
					for (size_t j = 0; j < finalVec[i].first.size(); j++) {
						for (size_t k = 0; k < finalVec[i].second.size(); k++) {
							final.push_back(finalVec[i].first[j] + " " + finalVec[i].second[k]);
						}
					}
				}
			}
			if (startPos == 2) {
				for (size_t i = 0; i < finalVec.size(); i++) {
					for (size_t j = 0; j < finalVec[i].second.size(); j++) {
						for (size_t k = 0; k < finalVec[i].first.size(); k++) {
							final.push_back(finalVec[i].second[j] + " " + finalVec[i].first[k]);
						}
					}
				}
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
			if (selType == "call") {
				if (result->getResult().getAttr() == "stmt#") {
					finalPart = progLine->getLinesOfType("call");
				}
				else {
					for (size_t i = 0; i < progLine->getLinesOfType("call").size(); i++) {
						if (!HUtility().contain(finalPart, PKB::getCallLine()->getCallee(progLine->getLinesOfType("call")[i]))) {
							finalPart.push_back(PKB::getCallLine()->getCallee(progLine->getLinesOfType("call")[i]));
						}
					}
				}
			}
		}
		else {
			//Return normal case
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

					//Case select from call
					if (!found && utility.getSymMean(rs) == "call") {
						if (result->getResult().getAttr() == "stmt#") {
							for (size_t j = 0; j < attList[i].reClause.size(); j++) {
								if (queryRS[attList[i].reClause[j]].synCount == 2) {
									if (queryRS[attList[i].reClause[j]].firstAtt == rs) {
										for (size_t k = 0; k < queryRS[attList[i].reClause[j]].table.size(); k++) {
											for (size_t m = 0; m < queryRS[attList[i].reClause[j]].table[k].second.size(); m++) {
												finalPart.push_back(queryRS[attList[i].reClause[j]].table[k].second[m]);
												
											}
										}
										found = true;
									}
								}
							}
						}
						if (result->getResult().getAttr() == "procName") {
							for (size_t j = 0; j < attList[i].reClause.size(); j++) {
								if (queryRS[attList[i].reClause[j]].synCount == 2) {
									if (queryRS[attList[i].reClause[j]].secondAtt == rs) {
										for (size_t k = 0; k < queryRS[attList[i].reClause[j]].table.size(); k++) {
											finalPart.push_back(queryRS[attList[i].reClause[j]].table[k].first);
										}
										found = true;
									}
								}
							}
						}
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