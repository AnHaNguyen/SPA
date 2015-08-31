#include "GNode.h"
#include <vector>
#include <string>

GNode::GNode(int line) {
	GNode::line = line;
}

GNode::~GNode() {
}

//void GNode::setProcedure(string procedure) {
//	GNode::procedure = procedure;
//}

void GNode::setLine(int line) {
	GNode::line = line;
}

void GNode::setChild(GNode* child) {
	for (unsigned i = 0; i < GNode::child.size(); i++) {
		if ((*GNode::child.at(i)).getLine() == (*child).getLine()) {
			return;
		}
	}
	GNode::child.push_back(child);
}

void GNode::setParent(GNode* parent) {
	for (unsigned i = 0; i < GNode::parent.size(); i++) {
		if ((*GNode::parent.at(i)).getLine() == (*parent).getLine()) {
			return;
		}
	}
	GNode::parent.push_back(parent);
}

//string GNode::getProcedure() {
//	return GNode::procedure;
//}

int GNode::getLine() {
	return GNode::line;
}

vector<GNode* > GNode::getChild() {
	return GNode::child;
}

vector<GNode* > GNode::getParent() {
	return GNode::parent;
}
