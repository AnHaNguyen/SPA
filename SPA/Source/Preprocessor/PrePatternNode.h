#ifndef PrePatternNode_H
#define PrePatternNode_H
#include<string>
using namespace std;

class PrePatternNode{
private:
    string synonym;
    PrePatternNode* next;
    string firstAttr;
    string secondAttr;
    string thirdAttr;

public:
    PrePatternNode();
    ~PrePatternNode();

    string getSynonym();
    PrePatternNode* getNext();
    string getFirstAttr();
    string getSecondAttr();
    string getThirdAttr();

    void setSynonym(string str);
    void setNext(PrePatternNode* node);
    void setFirstAttr(string str);
    void setSecondAttr(string str);
    void setThirdAttr(string str);
};

#endif
