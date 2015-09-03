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
	if (isContained(node)) {
		return false;
	}
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
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*parent).equals(*tree.at(i))) {
			returnList = (*tree.at(i)).getChildList();
			break;
		}
	}
	return returnList;
}

TNode* AST::findParent(TNode* child) {
	TNode* par = NULL;
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*child).equals(*tree.at(i))) {
			par = (*child).getParent();
			break;
		}
	}
	return par;
}

bool AST::isContained(TNode* node) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if ((*node).equals(*tree.at(i))) {
			return true;
		}
	}
	return false;
}

