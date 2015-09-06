#ifndef PreSuchThatNode_H
#define PreSuchThatNode_H
#include<string>
using namespace std;

class PreSuchThatNode{
private:
    string synonym;
    PreSuchThatNode* next;
    string firstAttr;
    string secondAttr;

public:
    PreSuchThatNode();
    ~PreSuchThatNode();

    string getSynonym();
    PreSuchThatNode* getNext();
    string getFirstAttr();
    string getSecondAttr();

    void setSynonym(string str);
    void setNext(PreSuchThatNode* node);
    void setFirstAttr(string str);
    void setSecondAttr(string str);
};

#endif
