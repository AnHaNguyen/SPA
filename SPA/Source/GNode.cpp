#include "GNode.h"
#include <vector>
#include <string>

GNode::GNode(string procedure, int line) {
	GNode::procedure = procedure;
	GNode::line = line;
}

GNode::~GNode() {
}

void GNode::setProcedure(string procedure) {
	GNode::procedure = procedure;
}

void GNode::setLine(int line) {
	GNode::line = line;
}

void GNode::setChild(GNode* child) {
	GNode::child.push_back(child);
}

void GNode::setParent(GNode* parent) {
	GNode::parent.push_back(parent);
}

string GNode::getProcedure() {
	return GNode::procedure;
}

int GNode::getLine() {
	return GNode::line;
}

vector<GNode* > GNode::getChild() {
	return GNode::child;
}

vector<GNode* > GNode::getParent() {
	return GNode::parent;
}
