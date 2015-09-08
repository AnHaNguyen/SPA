#include <vector>
#include <string>
#include "AST.h"

AST::AST() {
}

AST::~AST() {
}

vector<TNode* > AST::getTree() {
	return tree;
}

bool AST::addToTree(TNode* node){
	/*for (unsigned i = 0; i < tree.size(); i++) {		//to check if a node inside tree already
		if (tree.at(i)->equals(*node)) {
			return false;
		}
	}*/
	tree.push_back(node);
	return true;
}

void AST::makeChild(TNode* par, TNode* child) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*par).equals(*(tree.at(i)))) {
			(*tree.at(i)).setChild(child);
			(*child).setParent(tree.at(i));
			break;
		}
	}
}

void AST::makeParent(TNode* child, TNode* par){
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*child).equals(*(tree.at(i)))) {
			(*tree.at(i)).setParent(par);
			(*par).setChild(tree.at(i));
			break;
		}
	}
}

vector<TNode* > AST::findChild(TNode* parent) {
	vector <TNode* > returnList;
	TNode* node = findNode(parent);
	if (node->getType() != "") {
		return node->getChildList();
	}
	return returnList;
}

TNode* AST::findParent(TNode* child) {
	TNode* par = new TNode();				//empty node
	TNode* node = findNode(child);
	if (node->getType() != "") {
		return node->getParent();
	}
	return par;
}

TNode* AST::findNode(TNode* node) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*node).equals(*tree.at(i))) {
			return tree.at(i);
		}
	}
	return new TNode();				//empty node
}

vector <TNode* > AST::getLine(int line) {
	vector<TNode* > returnList;
	for (unsigned i = 0; i < tree.size(); i++) {
		if (tree.at(i)->getLine() == line) {
			returnList.push_back(tree.at(i));
		}
	}
	return returnList;
}

string AST::getProcedure() {
	if (tree.size() == 0) {
		return "";
	}
	else if (tree.at(0)->getType() != "procedure") {
		return "";
	}
	return tree.at(0)->getValue();
}

vector<TNode* > AST::getType(string type) {
	vector<TNode* > returnList;
	for (unsigned i = 0; i < tree.size(); i++) {
		if (tree.at(i)->getType() == type) {
			returnList.push_back(tree.at(i));
		}
	}
	return returnList;
}
