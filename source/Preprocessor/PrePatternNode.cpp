#include "PrePatternNode.h"

PrePatternNode::PrePatternNode(){
    synonym="";
    next=NULL;
    firstAttr="";
    secondAttr="";
    thirdAttr="";
}


PrePatternNode::~PrePatternNode(){
}


//getter
string PrePatternNode::getSynonym(){
    return synonym;
}

PrePatternNode* PrePatternNode::getNext(){
    return next;
}

string PrePatternNode::getFirstAttr(){
    return firstAttr;
}

string PrePatternNode::getSecondAttr(){
    return secondAttr;
}

string PrePatternNode::getThirdAttr(){
    return thirdAttr;
}



//setter
void PrePatternNode::setSynonym(string str){
    synonym=str;
}

void PrePatternNode::setNext(PrePatternNode* node){
    next=node;
}

void PrePatternNode::setFirstAttr(string node){
    firstAttr=node;
}


void PrePatternNode::setSecondAttr(string node){
    secondAttr=node;
}

void PrePatternNode::setThirdAttr(string node){
    thirdAttr=node;
}

