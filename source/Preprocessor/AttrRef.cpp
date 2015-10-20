#include "AttrRef.h"

AttrRef::AttrRef(){
    synonym="";
	attr = "";
}

AttrRef::AttrRef(string str1, string str2) {
	synonym = str1;
	attr = str2;
}


AttrRef::~AttrRef(){
}


//getter
string AttrRef::getSynonym(){
    return synonym;
}

string AttrRef::getAttr() {
	return attr;
}


//setter
void AttrRef::setSynonym(string str){
    synonym=str;
}


void AttrRef::setAttr(string str){
    attr=str;
}


