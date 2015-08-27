#include "TNode.h"
#include <vector>

	TNode::TNode(string value, string type, int line) {
		TNode::value = value;
		TNode::type = type;
		TNode::line = line;
		TNode::parent = NULL;
	}

	TNode::~TNode() {
	}
	
	void TNode::setValue(string val) {
		TNode::value = val;
	}

	void TNode::setType(string type) {
		TNode::type = type;
	}

	void TNode::setLine(int line) {
		TNode::line = line;
	}

	void TNode::setParent(TNode* par) {
		TNode::parent = par;
	}

	void TNode::setChild(TNode* child) {
		TNode::childList.push_back(child);
	}

	string TNode::getValue() {
		return TNode::value;
	}

	string TNode::getType() {
		return TNode::type;
	}
	
	int TNode::getLine() {
		return TNode::line;
	}

	TNode* TNode::getParent() {
		return TNode::parent;
	}

	vector<TNode* > TNode::getChildList() {
		return TNode::childList;
	}

