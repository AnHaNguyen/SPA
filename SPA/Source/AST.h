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
	vector<TNode* > getTree();		//return tree
	void addToTree(TNode* node);	//add a node  to AST
	void makeChild(TNode* par, TNode* child);	//add a child to a node in AST
	void makeParent(TNode* child, TNode* par);	//add parent to a node in AST
	void printTree();	//printout tree
	vector<TNode* > findChild(TNode* parent);	//return all child of a node
	TNode* findParent(TNode* child);	//return parent of a node
	bool isContained(TNode* node); //return true if Node is inside Tree, false otherwise
};

#endif