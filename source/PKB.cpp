#include "PKB.h"
#include "AST.h"
#include "FollowTable.h"
#include "ModifyTable.h"
#include "ParentTable.h"
#include "UseTable.h"
#include "DesignExtractor.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstTable.h"
#include "ProgLine.h"

FollowTable* PKB::followTable;
UseTable* PKB::useTable;
ParentTable* PKB::parentTable;
ModifyTable* PKB::modifyTable;
vector<AST*> PKB::astList;
VarTable* PKB::varTable;
ProcTable* PKB::procTable;
ConstTable* PKB::constTable;
CallTable* PKB::callTable;
NextTable* PKB::nextTable;
vector<CFG* > PKB::cfgList;
ProgLine* PKB::progLine;
FollowSTable* PKB::followSTable;
CallSTable* PKB::callSTable;
ParentSTable* PKB::parentSTable;
CallLine* PKB::callLine;

PKB::PKB() {
}

FollowTable* PKB::getFollowTable() {
	return followTable;
}

void PKB::setFollowTable(FollowTable* fTable) {
	followTable = fTable;
}

UseTable* PKB::getUseTable() {
	return useTable;
}

void PKB::setUseTable(UseTable* useTable) {
	PKB::useTable = useTable;
}

ParentTable* PKB::getParentTable() {
	return parentTable;
}

void PKB::setParentTable(ParentTable* parTable) {
	PKB::parentTable = parTable;
}

ModifyTable* PKB::getModifyTable() {
	return modifyTable;
}

void PKB::setModifyTable(ModifyTable* modTable) {
	modifyTable = modTable;
}

vector<AST*> PKB::getASTList() {
	return astList;
}

void PKB::setASTList(vector<AST* > astList) {
	PKB::astList = astList;
}

VarTable* PKB::getVarTable() {
	return varTable;
}

void PKB::setVarTable(VarTable* varTable) {
	PKB::varTable = varTable;
}

ProcTable* PKB::getProcTable() {
	return procTable;
}

void PKB::setProcTable(ProcTable* procTable) {
	PKB::procTable = procTable;
}

ConstTable* PKB::getConstTable() {
	return constTable;
}

void PKB::setConstTable(ConstTable* constTable) {
	PKB::constTable = constTable;
}

CallTable* PKB::getCallTable() {
	return callTable;
}

void PKB::setCallTable(CallTable* callTable) {
	PKB::callTable = callTable;
}

NextTable* PKB::getNextTable() {
	return nextTable;
}

void PKB::setNextTable(NextTable* nextTable) {
	PKB::nextTable = nextTable;
}

vector<CFG*> PKB::getCFGList() {
	return cfgList;
}

void PKB::setCFGList(vector<CFG* > cfgList) {
	PKB::cfgList = cfgList;
}

ProgLine* PKB::getProgLine() {
	return progLine;
}

void PKB::setProgLine(ProgLine* progLine) {
	PKB::progLine = progLine;
}

vector<string> PKB::checkAssign(string pattern, bool contains_) {
	vector<string> returnList;
	DesignExtractor ext = DesignExtractor();
	AST* subtree = ext.buildSubtree(pattern);
	/*for (unsigned i = 0; i < subtree->getTree().size(); i++) {
		returnList.push_back(subtree->getTree().at(i)->getType());
	}*/
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<string> temp = astList.at(i)->findSubtree(subtree, contains_);
		returnList.insert(returnList.end(), temp.begin(), temp.end());
	}
	return returnList;
}

vector<string> PKB::patternIf(string controlVar, bool contains) {
	vector<string> returnList;
	if (contains) {
		if (controlVar == "_") {
			vector<string> result = progLine->getLinesOfType("if");
			for (unsigned i = 0; i < result.size(); i++) {
				returnList.push_back(result.at(i));
			}
		}
		else {
			vector<TNode*> ifNodes;
			for (unsigned i = 0; i < astList.size(); i++) {
				vector<TNode*>ifs = astList.at(i)->getType("if");
				for (unsigned j = 0; j < ifs.size(); j++) {
					ifNodes.push_back(ifs.at(j));
				}
			}
			for (unsigned i = 0; i < ifNodes.size(); i++) {
				if (ifNodes.at(i)->getChildList().at(0)->getValue() == controlVar) {
					returnList.push_back(to_string(ifNodes.at(i)->getLine()));
				}
			}
		}
	}
	else {
		returnList = progLine->getLinesOfType("if");
	}
	return returnList;
}

vector<string> PKB::patternWhile(string controlVar, bool contains) {
	vector<string> returnList;
	if (contains) {
		if (controlVar == "_") {
			vector<string> result = progLine->getLinesOfType("while");
			for (unsigned i = 0; i < result.size(); i++) {
				returnList.push_back(result.at(i));
			}
		}
		else {
			vector<TNode*> whileNodes;
			for (unsigned i = 0; i < astList.size(); i++) {
				vector<TNode*> whiles = astList.at(i)->getType("while");
				for (unsigned j = 0; j < whiles.size(); j++) {
					whileNodes.push_back(whiles.at(j));
				}
			}
			for (unsigned i = 0; i < whileNodes.size(); i++) {
				if (whileNodes.at(i)->getChildList().at(0)->getValue() == controlVar) {
					returnList.push_back(to_string(whileNodes.at(i)->getLine()));
				}
			}
		}
	}
	else {
		returnList = progLine->getLinesOfType("while");
	}
	return returnList;
}

FollowSTable* PKB::getFollowSTable() {
	return followSTable;
}

void PKB::setFollowSTable(FollowSTable* followSTable) {
	PKB::followSTable = followSTable;
}

CallSTable* PKB::getCallSTable() {
	return callSTable;
}

void PKB::setCallSTable(CallSTable* callSTable) {
	PKB::callSTable = callSTable;
}

ParentSTable* PKB::getParentSTable() {
	return parentSTable;
}

void PKB::setParentSTable(ParentSTable* parentSTable) {
	PKB::parentSTable = parentSTable;
}

bool PKB::checkExistMod(string start, string end, string modVar,vector<string> processed) {
	/*vector<string> modifier = modifyTable->getModifier(modVar);
	queue<string> q;
	q.push(start);
	while (!q.empty()) {
		vector<string> nextStmts = nextTable->getNext(q.front());
		q.pop();
		for (int i = 0; i < nextStmts.size(); i++) {
			if (nextStmts.at(i) == end) {
				return true;
			}
			int contain = 0;
			for (int j = 0; j < modifier.size(); j++) {
				if (nextStmts.at(i) == modifier.at(j)) {
					contain = 1;
				}
			}
			if (contain == 0) {
				q.push(nextStmts.at(i));
			}
		}
	}
	return false;*/
	vector<string> nextStmts = nextTable->getNext(start);
	if (nextStmts.size() == 0) {
		return false;
	}
	vector<bool> nextAns;
	for (unsigned i = 0; i < nextStmts.size(); i++) {
		string nextLine = nextStmts.at(i);
		if (nextLine == end) {
			return true;
		}
		else if (find(processed.begin(), processed.end(), nextLine) != processed.end()) {
			nextAns.push_back(false);
		}
		else if (progLine->getType(nextLine) == "assign" && 
						modifyTable->isModified(nextLine,modVar)) {
			nextAns.push_back(false);
		}
		else {
			processed.push_back(nextLine);
			nextAns.push_back(checkExistMod(nextLine, end, modVar, processed));
		}
	}
	bool finalAns = false;
	for (unsigned i = 0; i < nextAns.size(); i++) {
		finalAns = (finalAns || nextAns.at(i));
	}
	return finalAns;
}

vector<pair<string,string>> PKB::patternIf() {
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> ast = astList.at(i)->getTree();
		for (unsigned j = 0; j < ast.size(); j++) {
			if (ast.at(j)->getType() == "if") {
				pair<string, string> pr;
				pr.first = to_string(ast.at(j)->getLine());
				pr.second = ast.at(j)->getChildList().at(0)->getValue();
				returnList.push_back(pr);
			}
		}
	}
	return returnList;
}

vector<pair<string, string>> PKB::patternWhile() {
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> ast = astList.at(i)->getTree();
		for (unsigned j = 0; j < ast.size(); j++) {
			if (ast.at(j)->getType() == "while") {
				pair<string, string> pr;
				pr.first = to_string(ast.at(j)->getLine());
				pr.second = ast.at(j)->getChildList().at(0)->getValue();
				returnList.push_back(pr);
			}
		}
	}
	return returnList;
}

vector<string> PKB::affect(string n1, string n2) {
	vector<string> returnList;
	vector<string> processed;
	if (n1 != "_" && n2 != "_") {	//affect (1,2)
	//	return true or false;
		if (progLine->getType(n1) != "assign" || progLine->getType(n2) != "assign") {
			return returnList;
		}
		string modVar = modifyTable->getModified(n1).at(0);
		if (!useTable->isUsed(n2, modVar)) {
			returnList.push_back("false");
			return returnList;
		}

		if (checkExistMod(n1, n2, modVar, processed)) {
			returnList.push_back("true");
		}
		else {
			returnList.push_back("false");
		}
		return returnList;
	}
	else if (n1 != "_" && n2 == "_") {	//affect(1,n2)
		if (progLine->getType(n1) != "assign") {
			return returnList;
		}
		string modVar = modifyTable->getModified(n1).at(0);
		
		// for all assign in this procedure
		//		if (isUsed(assign, modVar) && exist...(n1,assign, var)
		//			add assign
		/*string curProc = progLine->getProcedure(n1);
		vector<string> assignList = progLine->getAssignsOfProc(curProc);
		for (unsigned i = 0; i < assignList.size(); i++) {
			if (useTable->isUsed(assignList.at(i), modVar) &&
				checkExistMod(n1, assignList.at(i), modVar,processed)) {
				returnList.push_back(assignList.at(i));
			}
		}*/
		returnList = getExistMod(n1, modVar, processed);
		return returnList;
		//find all assignments 1->as and no modify of modVar in that path and as uses modVar
	}
	else if (n2 != "_" && n1 == "_") {	//affect(n1,2)
		if (progLine->getType(n2) != "assign") {
			return returnList;
		}
		vector<string> usedVar = useTable->getUsed(n2);
		string curProc = progLine->getProcedure(n2);
		vector<string> assignList = progLine->getAssignsOfProc(curProc);
		for (unsigned i = 0; i < assignList.size(); i++) {
			for (unsigned j = 0; j < usedVar.size(); j++) {
				if (modifyTable->isModified(assignList.at(i),usedVar.at(j)) &&
					checkExistMod(assignList.at(i), n2, usedVar.at(j),processed)) {
					returnList.push_back(assignList.at(i));
				}
			}
		}
		return returnList;
		// for all assign in this procedure
		//		for each var in usedVar
		//			if (isMod(assign, var) && exist...(assign,n2,var)
		//				add assign
		
		//find all assignments as->1 and as mod var and no modify of var in that path
	}
	return returnList;
}

vector<pair<string, vector<string>>> PKB::affect() {
	vector<pair<string, vector<string>>> returnList;
	vector<string> assignList = progLine->getLinesOfType("assign");
	for (unsigned i = 0; i < assignList.size(); i++) {
		vector<string> affectList = affect(assignList.at(i), "_");
		pair<string, vector<string>> pr;
		pr.first = assignList.at(i);
		pr.second = affectList;
		returnList.push_back(pr);
	}
	return returnList;
}

vector<string> PKB::affectS(string n1, string n2) {
	vector<string> returnList;
	if (n1 != "_" && n2 != "_") {		//affect*(1,2)
		if (progLine->getType(n1) != "assign" || progLine->getType(n2) != "assign") {
			return returnList;
		}
		else {
			vector<string> list = getAffectS(n1);
			for (unsigned i = 0; i < list.size(); i++) {
				if (list.at(i) == n2) {
					returnList.push_back("true");
					return returnList;
				}
			}
			returnList.push_back("false");
			return returnList;
		}

	}
	else if (n1 != "_" && n2 == "_") {		//affect*(1,n2)
		if (progLine->getType(n1) != "assign") {
			return returnList;
		}
		else {
			returnList = getAffectS(n1);
			return returnList;
		}
	}
	else if (n1 == "_" && n2 != "_") {		//affect*(n1, 2)
		if (progLine->getType(n2) != "assign") {
			return returnList;
		}
		else {
			returnList = getAffectSReverse(n2);
			return returnList;
		}
	}
	return returnList;
}

vector<pair<string, vector<string>>> PKB::affectS() {
	vector<pair<string, vector<string>>> returnList;
	for (unsigned i = 0; i < procTable->size(); i++) {
		string curProc = procTable->getProc(i);
		vector<string> assignList = progLine->getAssignsOfProc(curProc);
		for (unsigned j = 0; j < assignList.size(); j++) {
			string line = assignList.at(j);
			vector<string> affectStmts = affectS(line, "_");
			pair<string, vector<string>> pr;
			pr.first = line;
			pr.second = affectStmts;
			returnList.push_back(pr);
		}
	}
	return returnList;
}

vector<string> PKB::getAffectS(string n1) {
	vector<string> returnList;
	vector<bool> processed;
	//string curProc = progLine->getProcedure(n1);
	//vector<string> assignList = progLine->getAssignsOfProc(n1);
	int numLines = progLine->numOfLines();
	for (int i = 0; i < numLines; i++) {
		processed.push_back(false);
	}
	queue<string> q;
	q.push(n1);
	while (!q.empty()) {
		string cur = q.front();
		q.pop();
		vector<string> affectStmts = affect(cur, "_");
		for (unsigned i = 0; i < affectStmts.size(); i++) {
			if (!processed.at(atoi(affectStmts.at(i).c_str()) -1)) {
				processed.at(atoi(affectStmts.at(i).c_str()) -1) = true;
				returnList.push_back(affectStmts.at(i));
				q.push(affectStmts.at(i));
			}
		}
	}
	return returnList;
}

vector<string> PKB::getAffectSReverse(string n2) {
	vector<string> returnList;
	vector<bool> processed;
	int numLines = progLine->numOfLines();
	for (int i = 0; i < numLines; i++) {
		processed.push_back(false);
	}
	queue<string> q;
	q.push(n2);
	while (!q.empty()) {
		vector<string> affectStmts = affect("_", q.front());
		q.pop();
		for (unsigned i = 0; i < affectStmts.size(); i++) {
			if (!processed.at(atoi(affectStmts.at(i).c_str()) - 1)) {
				processed.at(atoi(affectStmts.at(i).c_str()) - 1) = true;
				returnList.push_back(affectStmts.at(i));
				q.push(affectStmts.at(i));
			}
		}
	}
	return returnList;
}

/*void PKB::updateProgLineParent() {
	for (unsigned i = 0; i < progLine->numOfLines(); i++) {
		for (unsigned j = 0; j < parentTable->size(); j++) {
			string parent = parentTable->getTable().at(j).lineNo;
			for (unsigned k = 0; k < parentTable->getTable().at(j).child.size(); k++) {
				string child = parentTable->getTable().at(j).child.at(k);
				if (atoi(child.c_str()) == i + 1) {
					progLine->updateParent(i, parent);
				}
			}
		}
	}
}*/

/*void PKB::updateProgLine() {
	for (unsigned i = 0; i < progLine->numOfLines(); i++) {
		for (unsigned j = 0; j < parentTable->size(); j++) {
			string parent = parentTable->getTable().at(j).lineNo;
			for (unsigned k = 0; k < parentTable->getTable().at(j).child.size(); k++) {
				string child = parentTable->getTable().at(j).child.at(k);
				if (atoi(child.c_str()) == i + 1) {
					progLine->updateParent(i, parent);
				}
			}
		}
		for (unsigned j = 0; j < parentSTable->size(); j++) {
			string parentS = parentSTable->getTable().at(j).lineNo;
			for (unsigned k = 0; k < parentSTable->getTable().at(j).child.size(); k++) {
				string child = parentSTable->getTable().at(j).child.at(k);
				if (atoi(child.c_str()) == i + 1) {
					progLine->updateParentS(i, parentS);
				}
			}
		}
		for (unsigned j = 0; j < followSTable->size(); j++) {
			string followS = followSTable->getTable().at(j).lineNo;
			for (unsigned k = 0; k < followSTable->getTable().at(j).followStmts.size(); k++) {
				string followed = followSTable->getTable().at(j).followStmts.at(k);
				if (atoi(followed.c_str()) == i + 1) {
					progLine->updateFollowS(i, followS);
				}
			}
		}
		for (unsigned j = 0; j < nextTable->size(); j++) {
			string prev = nextTable->getTable().at(j).lineNo;
			for (unsigned k = 0; k < nextTable->getTable().at(j).nextStmts.size(); k++) {
				string next = nextTable->getTable().at(j).nextStmts.at(k);
				if (atoi(next.c_str()) == i + 1) {
					progLine->updateNext(i, prev);
				}
			}
		}
	}
	//exit(EXIT_FAILURE);
}*/

CallLine* PKB::getCallLine() {
	return callLine;
}

void PKB::setCallLine(CallLine* callLine) {
	PKB::callLine = callLine;
}

vector<string> PKB::getExistMod(string start, string var, vector<string> processed) {
	vector<string> returnList;
	vector<string> temp = nextTable->getNext(start);
	vector<string> nextStmts;
	for (unsigned i = 0; i < temp.size(); i++) {
		if (find(processed.begin(), processed.end(), temp.at(i)) == processed.end()) {
			nextStmts.push_back(temp.at(i));
		}
	}
	if (nextStmts.size() == 0) {
		return returnList;
	}
	for (unsigned i = 0; i < nextStmts.size(); i++) {
		string nextLine = nextStmts.at(i);
		processed.push_back(nextLine);
		if (progLine->getType(nextLine) == "assign") {
			if (useTable->isUsed(nextLine, var)) {
				returnList.push_back(nextLine);
			}
			if (!modifyTable->isModified(nextLine, var)) {
				vector<string> result = getExistMod(nextLine, var, processed);
				returnList.insert(returnList.end(), result.begin(), result.end());
			}
		}
		else {
			vector<string> result = getExistMod(nextLine, var, processed);
			returnList.insert(returnList.end(), result.begin(), result.end());
		}
	}
	return returnList;
}

vector<string> PKB::contains(string n1, bool isInt1, string n2, bool isInt2) {
	vector<string> returnList;
	if (isInt1 && isInt2) {		//contains(1,2) never happen
		returnList.push_back("false");
		return returnList;
	}
	else if (isInt1 && !isInt2){		//contains(1, if)
		bool isDone = false;
		if (n2 == "variable" || n2 == "stmtLst" || n2 == "constant" ) {
			for (unsigned i = 0; i < astList.size(); i++) {
				AST* tree = astList.at(i);
				vector<TNode*> lineList = tree->getLine(atoi(n1.c_str()));
				for (unsigned j = 0; j < lineList.size(); j++) {
					if (lineList.at(j)->getParent()->getType() == "stmtLst") {
						vector<TNode*> childList = lineList.at(j)->getChildList();
						for (unsigned k = 0; k < childList.size(); k++) {
							if (childList.at(k)->getType() == n2) {
								returnList.push_back(returnValue(childList.at(k)));
								isDone = true;
							}
						}
					}
					if (isDone) {
						return Utility::removeDuplicate(returnList);
					}
				}
			}
		}	//do
		else {
			return returnList;
		}
	}
	else if (!isInt1 && isInt2)	{		//contains(if ,2)
		if (n1 == "stmtLst") {
			for (unsigned i = 0; i < astList.size(); i++) {
				AST* tree = astList.at(i);
				vector<TNode*> stmtList = tree->getType("stmtLst");
				for (unsigned j = 0; j < stmtList.size(); j++) {
					vector<TNode*> childList = stmtList.at(j)->getChildList();
					for (unsigned k = 0; k < childList.size(); k++) {
						if (to_string(childList.at(k)->getLine()) == n2) {
							returnList.push_back(returnValue(stmtList.at(j)));
							return returnList;
						}
					}
				}
			}
		}//do
		else {
			return returnList;
		}
	}
	return returnList;
}

vector<pair<string, string>> PKB::contains(string n1, string n2) {		//need check stmt
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> tree = astList.at(i)->getTree();
		for (unsigned j = 0; j < tree.size(); j++) {
			TNode* par = tree.at(j);
			if (isEqualType(par->getType(), n1)) {
				vector<TNode*> childList = par->getChildList();
				for (unsigned k = 0; k < childList.size(); k++) {
					if (isEqualType(childList.at(k)->getType(), n2)) {
						pair<string, string> pr;
						pr.first = returnValue(par);
						pr.second = returnValue(childList.at(k));
						returnList.push_back(pr);
					}
				}
			}
		}
	}
	return Utility::removeDuplicate(returnList);
}

vector<string> PKB::sibling(string n1, bool isInt1, string n2, bool isInt2) {
	vector<string> returnList;
	if (isInt1 && isInt2) {		//sibling(1,2)
		if (followSTable->isFollowS(n1, n2) || followSTable->isFollowS(n2, n1)) {
			returnList.push_back("true");
		}
		else {
			returnList.push_back("false");
		}
		return returnList;
	}
	else if (!isInt1 && isInt2) {		//sibling (if ,2)
		vector<string> thisList = followSTable->getNextS(n2);
		vector<string> thatList = followSTable->getPrevS(n2);
		for (unsigned i = 0; i < thisList.size(); i++) {
			if (isEqualType(progLine->getType(thisList.at(i)), n1)) {
				returnList.push_back(thisList.at(i));
			}
		}
		for (unsigned i = 0; i < thatList.size(); i++) {
			if (isEqualType(progLine->getType(thatList.at(i)), n1)) {
				returnList.push_back(thatList.at(i));
			}
		}
		return returnList;
	}
	else if (isInt1 && !isInt2) {		//sibling (1, if)
		vector<string> thisList= followSTable->getNextS(n1);
		vector<string> thatList = followSTable->getPrevS(n1);
		for (unsigned i = 0; i < thisList.size(); i++) {
			if (isEqualType(progLine->getType(thisList.at(i)), n2)) {
				returnList.push_back(thisList.at(i));
			}
		}
		for (unsigned i = 0; i < thatList.size(); i++) {
			if (isEqualType(progLine->getType(thatList.at(i)), n2)) {
				returnList.push_back(thatList.at(i));
			}
		}
		return returnList;
	}
	return returnList;
}

vector<pair<string, string>> PKB::sibling(string n1, string n2) {	//need check stmt
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		vector<TNode*> tree = astList.at(i)->getTree();
		for (unsigned j = 0; j < tree.size(); j++) {
			if (tree.at(j)->getChildList().size() > 1) {
				vector<TNode*> childList = tree.at(j)->getChildList();
				for (unsigned k = 0; k < childList.size(); k++) {
					if (isEqualType(childList.at(k)->getType(),n1)) {
						for (unsigned t = 0; t < childList.size(); t++) {
							if (isEqualType(childList.at(t)->getType(),n2) && t != k) {
								pair<string, string> pr;
								pr.first = returnValue(childList.at(k));
								pr.second = returnValue(childList.at(t));
								returnList.push_back(pr);
							}
						}
					}
				}
			}
		}
	}
	return Utility::removeDuplicate(returnList);
}

string PKB::returnValue(TNode* tnode) {
	string type = tnode->getType();
	if (type == "procedure" || type == "variable" || type == "constant") {
		return tnode->getValue();
	}
	else if (type == "stmt" || type == "assign" || type == "call" || type == "if" || type == "while") {
		return to_string(tnode->getLine());
	}
	else if (type == "stmtLst") {
		return to_string(tnode->getChildList().at(0)->getLine());
	}
	else {
		return type;
	}
}

vector<string> PKB::containsS(string n1, bool isInt1, string n2, bool isInt2) {
	vector<string> returnList;
	if (isInt1 && isInt2) {		//Contains*(1,2)
		if (parentSTable->isParentS(n1, n2)) {
			returnList.push_back("true");
		}
		else {
			returnList.push_back("false");
		}
		return returnList;
	}
	else if (isInt1 && !isInt2) {		//Contains*(1,if)	need check stmt
		vector<TNode*> lineList;
		for (unsigned i = 0; i < astList.size(); i++) {
			AST* tree = astList.at(i);
			lineList = tree->getLine(atoi(n1.c_str()));
			if (lineList.size() > 0) {
				break;
			}
		}
		TNode* node;
		for (unsigned i = 0; i < lineList.size(); i++) {
			if (lineList.at(i)->getParent()->getType() == "stmtLst") {
				node = lineList.at(i);
				break;
			}
		}
		queue<TNode*> q;
		q.push(node);
		while (!q.empty()) {
			vector<TNode*> childList = q.front()->getChildList();
			q.pop();
			for (unsigned i = 0; i < childList.size(); i++) {
				if (isEqualType(childList.at(i)->getType(),n2)) {
					returnList.push_back(returnValue(childList.at(i)));
				}
				q.push(childList.at(i));
			}
		}
		return Utility::removeDuplicate(returnList);
	}
	else if (!isInt1 && isInt2) {	//contains*(if,2)		need check stmt
		vector<TNode*> lineList;
		for (unsigned i = 0; i < astList.size(); i++) {
			AST* tree = astList.at(i);
			lineList = tree->getLine(atoi(n2.c_str()));
			if (lineList.size() > 0) {
				break;
			}
		}
		TNode* node;
		for (unsigned i = 0; i < lineList.size(); i++) {
			if (lineList.at(i)->getParent()->getType() == "stmtLst") {
				node = lineList.at(i);
				break;
			}
		}
		queue<TNode*> q;
		q.push(node);
		while (!q.empty()) {
			TNode* par = q.front()->getParent();
			q.pop();
		
			if (isEqualType(par->getType(),n1)) {
				returnList.push_back(returnValue(par));
			}
			if (par->getType() != "procedure") {
				q.push(par);
			}
		}
		return Utility::removeDuplicate(returnList);
	}
	return returnList;
}

vector<pair<string, string>> PKB::containsS(string n1, string n2) {		//need check stmt
	vector<pair<string, string>> returnList;
	for (unsigned i = 0; i < astList.size(); i++) {
		AST* tree = astList.at(i);
		vector<TNode*> typeList;
		if (n1 != "stmt") {
			typeList = tree->getType(n1);
		}
		else {
			typeList = tree->getStmtNode();
		}
		for (unsigned j = 0; j < typeList.size(); j++) {
			queue<TNode*> q;
			q.push(typeList.at(j));
			while (!q.empty()) {
				vector<TNode*> childList = q.front()->getChildList();
				q.pop();
				for (unsigned k = 0; k < childList.size(); k++) {
					if (isEqualType(childList.at(k)->getType(),n2)) {
						pair<string, string> pr;
						pr.first = returnValue(typeList.at(j));
						pr.second = returnValue(childList.at(k));
						returnList.push_back(pr);
					}
					q.push(childList.at(k));
				}
			}
		}
	}
	return Utility::removeDuplicate(returnList);
}

bool PKB::isEqualType(string type1, string type2) {
	if (type2 != "stmt") {
		return (type1 == type2);
	}
	else {
		return (type1 == "assign" || type1 == "call" || type1 == "if" || type1 == "while");
	}
}