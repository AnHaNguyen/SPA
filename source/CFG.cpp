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
bool CFG::addToGraph(GNode* node) {
	for (unsigned i = 0; i < cfg.size(); i++){
		if ((*cfg.at(i)).getLine() == node->getLine()) {
			return false;
		}
	}
	cfg.push_back(node);
	return true;
}

//make a node at lineNo to be child of node at parentLine
//both nodes need to be added to graph beforehand.
void CFG::makeChild(GNode* parent, GNode* child) {	
	/*for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == parentLine->getLine()) {
			GNode* newNode = new GNode(lineNo);
			GNode* par = cfg.at(i);
			(*par).setChild(newNode);
			(*newNode).setParent(par);
		}
	}*/
	child->setParent(parent);
	parent->setChild(child);
}

void CFG::makeParent(GNode* child, GNode* parent) {
	/*for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == childLine) {
			GNode* newNode = new GNode(lineNo);
			GNode* child = cfg.at(i);
			(*child).setParent(newNode);
			(*newNode).setChild(child);
		}
	}*/
	parent->setChild(child);
	child->setParent(parent);
}

//find all parents of a stmt
vector<GNode*> CFG::findParent(int lineNo) {
	vector<GNode* > parent;
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == lineNo) {
			parent = (*cfg.at(i)).getParent();
			break;
		}
	}
	return parent;
}

//find all child of stmt
vector<GNode*> CFG::findChild(int lineNo) {
	vector<GNode*> child;
	for (unsigned i = 0; i < cfg.size(); i++) {
		if ((*cfg.at(i)).getLine() == lineNo) {
			child = (*cfg.at(i)).getChild();
			break;
		}
	}
	return child;
}
