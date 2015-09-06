#include "PreSuchThatNode.h"


PreSuchThatNode::PreSuchThatNode(){
    synonym="";
    next=NULL;
    firstAttr="";
    secondAttr="";;
}


PreSuchThatNode::~PreSuchThatNode(){
}


//getter
string PreSuchThatNode::getSynonym(){
    return synonym;
}

PreSuchThatNode* PreSuchThatNode::getNext(){
    return next;
}

string PreSuchThatNode::getFirstAttr(){
    return firstAttr;
}

string PreSuchThatNode::getSecondAttr(){
    return secondAttr;
}


//setter
void PreSuchThatNode::setSynonym(string str){
    synonym=str;
}

void PreSuchThatNode::setNext(PreSuchThatNode* node){
    next=node;
}

void PreSuchThatNode::setFirstAttr(string str){
    firstAttr=str;
}

void PreSuchThatNode::setSecondAttr(string str){
    secondAttr=str;
}
