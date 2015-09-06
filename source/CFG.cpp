#include "CFG.h"
#include "GNode.h"
#include <string>
#include <vector>

CFG::CFG(string proc) {
	procedure = proc;
}

CFG::~CFG() {
}

int CFG::size() {
	return cfg.size();
}

string CFG::getProcedure() {
	return procedure;
}

//add a node to a graph
void CFG::addToGraph(int lineNo) {
	GNode* newNode = new GNode(lineNo);
	for (unsigned i = 0; i < cfg.size(); i++){
		if ((*cfg.at(i)).getLine() == lineNo) {
			return;
		}
	}
	cfg.push_back(newNode);
}

//make a node at lineNo to be child of node at parentLine
//both nodes need to be added to graph beforehand.
void CFG::makeChild(int lineNo, int parentLine) {	
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == parentLine) {
			GNode* newNode = new GNode(lineNo);
			GNode* par = cfg.at(i);
			(*par).setChild(newNode);
			(*newNode).setParent(par);
		}
	}
}

void CFG::makeParent(int lineNo, int childLine) {
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == childLine) {
			GNode* newNode = new GNode(lineNo);
			GNode* child = cfg.at(i);
			(*child).setParent(newNode);
			(*newNode).setChild(child);
		}
	}
}

//find all parents of a stmt
vector<int> CFG::findParent(int lineNo) {
	vector<int> parentStmt;
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == lineNo) {
			vector<GNode* > parent = (*cfg.at(i)).getParent();
			for (unsigned j = 0; j < parent.size(); j++) {
				parentStmt.push_back((*parent.at(j)).getLine());
			}
			break;
		}
	}
	return parentStmt;
}

//find all child of stmt
vector<int> CFG::findChild(int lineNo) {
	vector<int> childStmt;
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == lineNo) {
			vector<GNode* > child = (*cfg.at(i)).getChild();
			for (unsigned j = 0; j < child.size(); j++) {
				childStmt.push_back((*child.at(j)).getLine());
			}
			break;
		}
	}
	return childStmt;
}