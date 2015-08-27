#include <vector>
#include <string>
#include "AST.h"

AST::AST() {
}

AST::~AST() {
}

vector<TNode* > AST::getTree() {
	return AST::tree;
}

void AST::addRoot(TNode* root){
	
}

void AST::addChild(int parIndex, TNode* child) {
	TNode* par = AST::tree.at(parIndex);
	(*par).setChild(child);
	AST::tree.push_back(child);
}
