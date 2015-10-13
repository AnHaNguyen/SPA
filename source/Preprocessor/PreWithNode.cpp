#include "PreWithNode.h"
#include "AttrRef.h"

PreWithNode::PreWithNode(){
    next=NULL;
	leftAttrRef;
	rightAttrRef;
	leftType = "";
	rightType = "";
}


PreWithNode::~PreWithNode(){
}


//getter
PreWithNode* PreWithNode::getNext(){
    return next;
}

AttrRef PreWithNode::getLeftAttrRef() {
	return leftAttrRef;
}

AttrRef PreWithNode::getRightAttrRef() {
	return rightAttrRef;
}

string PreWithNode::getLeftType() {
	return leftType;
}

string PreWithNode::getRightType() {
	return rightType;
}

//setter
void PreWithNode::setNext(PreWithNode* node){
    next=node;
}

void PreWithNode::setLeftAttrRef(AttrRef ar) {
	leftAttrRef = ar;
}

void PreWithNode::setRightAttrRef(AttrRef ar) {
	rightAttrRef = ar;
}

void PreWithNode::setLeftType(string str) {
	leftType = str;
}

void PreWithNode::setRightType(string str) {
	rightType = str;
}

