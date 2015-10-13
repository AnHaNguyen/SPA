#ifndef PreWithNode_H
#define PreWithNode_H
#include"AttrRef.h"
#include<string>
using namespace std;

class PreWithNode{
private:
    PreWithNode* next;
	AttrRef leftAttrRef;
	AttrRef rightAttrRef;
	string rightType;


public:
    PreWithNode();
    ~PreWithNode();

    PreWithNode* getNext();
	AttrRef getLeftAttrRef();
	AttrRef getRightAttrRef();
	string getRightType();

    void setNext(PreWithNode* node);
	void setLeftAttrRef(AttrRef ar);
	void setRightAttrRef(AttrRef ar);
	void setRightType(string str);

};

#endif
