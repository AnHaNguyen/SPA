#ifndef PreResultNode_H
#define PreResultNode_H
#include "AttrRef.h"
#include <string>
using namespace std;

class PreResultNode{
private:
    AttrRef result;
	PreResultNode* next;
public:
    PreResultNode();
    ~PreResultNode();

    AttrRef getResult();
	PreResultNode* getNext();

    void setResult(AttrRef ar);
	void setNext(PreResultNode* node);
};

#endif
