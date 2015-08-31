#include "PreResultNode.h"

PreResultNode::PreResultNode(){
    result = "";
}

PreResultNode::~PreResultNode(){
}


//getter
string PreResultNode::getResult(){
    return result;
}


//setter
void PreResultNode::setResult(string str){
    result = str;
}
