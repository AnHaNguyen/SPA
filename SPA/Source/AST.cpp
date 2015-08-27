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

void AST::addRoot(TNode* root){
	
}

void AST::addChild(int parIndex, TNode* child) {
	TNode* par = tree.at(parIndex);
	(*par).setChild(child);
	tree.push_back(child);
}
