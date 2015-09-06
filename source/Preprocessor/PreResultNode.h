#ifndef PreResultNode_H
#define PreResultNode_H

#include <string>
using namespace std;

class PreResultNode{
private:
    string result;
public:
    PreResultNode();
    ~PreResultNode();

    string getResult();
    void setResult(string str);
};

#endif
