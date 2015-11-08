#include "PreResultNode.h"

PreResultNode::PreResultNode(){
    result;
	next=NULL;
}

PreResultNode::~PreResultNode(){
}


//getter
AttrRef PreResultNode::getResult(){
    return result;
}
PreResultNode* PreResultNode::getNext() {
	return next;
}

//setter
void PreResultNode::setResult(AttrRef ar){
    result = ar;
}
void PreResultNode::setNext(PreResultNode* node) {
	next = node;
}
