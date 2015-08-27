#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "TNode.h"

using namespace std;
class AST {
private:
	vector<TNode* > tree;
public:
	AST();
	~AST();
	vector<TNode* > getTree();
	void addRoot(TNode* root);
	void addChild(int parIndex, TNode* child);
	void addParent(TNode* child, TNode* par);
	void printTree();
};

#endif