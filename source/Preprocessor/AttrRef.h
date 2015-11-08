#ifndef AttrRef_H
#define AttrRef_H
#include<string>
using namespace std;

class AttrRef{
private:
    string synonym;
	string attr;

public:
    AttrRef();
	AttrRef(string str1, string str2);
    ~AttrRef();

    string getSynonym();
	string getAttr();

    void setSynonym(string str);
	void setAttr(string str);
};

#endif
