#include <vector>
#include <string>
#include "AST.h"
#include <queue>

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

vector<int> AST::getAssign() {
	vector<int> returnList;
	for (unsigned i = 0; i < tree.size(); i++) {
		if (tree.at(i)->getType() == "assign") {
			returnList.push_back(tree.at(i)->getLine());
		}
	}
	return returnList;
}

vector<int> AST::findSubtree(AST* subtree, bool contains_) {
	vector<int> returnList;
	vector<TNode*> assignList = getType("assign");
	TNode* subtreeRoot = subtree->getTree().at(0);
	
	vector<TNode*> operList;

	for (unsigned i = 0; i < assignList.size(); i++) {
		operList.push_back(assignList.at(i)->getChildList().at(1));	//every assign node has 2 children
	}

	if (!contains_) {				//pattern (_,"x+y")	
		for (unsigned i = 0; i < operList.size(); i++) {
			if (compare(operList.at(i), subtreeRoot)) {
				returnList.push_back(operList.at(i)->getLine());
			}
		}
	}
	else {						//pattern (_,_"x+y"_)
		for (unsigned i = 0; i < operList.size(); i++) {
			if (compareSubtree(operList.at(i), subtreeRoot)) {
				returnList.push_back(operList.at(i)->getLine());
			}
		}
	}
	return returnList;
}

bool AST::compare(TNode* root1, TNode* root2) {
	queue<TNode*> q1;
	queue<TNode*> q2;
	q1.push(root1);
	q2.push(root2);
	while (q1.size() > 0 && q2.size() > 0) {
		TNode* ele1 = q1.front();
		TNode* ele2 = q2.front();
		if (ele1->getType() == ele2->getType()
			&& ele1->getValue() == ele2->getValue()) {
			q1.pop();
			q2.pop();
			for (unsigned j = 0; j < ele1->getChildList().size(); j++) {
				q1.push(ele1->getChildList().at(j));
			}
			for (unsigned j = 0; j < ele2->getChildList().size(); j++) {
				q2.push(ele2->getChildList().at(j));
			}
		}
		else {
			return false;
		}
	}
	if (q1.size() == 0 && q2.size() == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool AST::compareSubtree(TNode* root1, TNode* root2) {
	queue<TNode*> q;
	q.push(root1);
	while (q.size() > 0) {
		TNode* newRoot = q.front();
		if (compare(newRoot, root2)) {
			return true;
		}
		else {
			q.pop();
			for (unsigned i = 0; i < newRoot->getChildList().size(); i++) {
				q.push(newRoot->getChildList().at(i));
			}
		}
	}
	return false;
}