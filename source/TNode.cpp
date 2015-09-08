#include "TNode.h"
#include <vector>
#include <string>

TNode::TNode() {
	TNode::value = "";
	TNode::type = "";
	TNode::line = 0;
}

	TNode::TNode(string value, string type, int line) {
		TNode::value = value;
		TNode::type = type;
		TNode::line = line;
		parent = new TNode();
	}

	TNode::~TNode() {
	}
	
	void TNode::setValue(string val) {
		value = val;
	}

	void TNode::setType(string type) {
		TNode::type = type;
	}

	void TNode::setLine(int line) {
		TNode::line = line;
	}

	void TNode::setParent(TNode* par) {
		parent = par;
	}

	void TNode::setChild(TNode* child) {
		childList.push_back(child);
	}

	string TNode::getValue() {
		return value;
	}

	string TNode::getType() {
		return type;
	}
	
	int TNode::getLine() {
		return line;
	}

	TNode* TNode::getParent() {
		return parent;
	}

	vector<TNode* > TNode::getChildList() {
		return childList;
	}

	bool TNode::equals(TNode obj) {
		if (value == obj.getValue() && type == obj.getType()
			&& line == obj.getLine()) {
			return true;
		}
		return false;
	}
	