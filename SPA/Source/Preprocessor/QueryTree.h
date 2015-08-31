#ifndef QueryTree_H
#define QueryTree_H

#include<string>
#include<vector>
#include "PreResultNode.h"
#include "PreSuchThatNode.h"
#include "PrePatternNode.h"
using namespace std;

class QueryTree{
private:
    bool isValid;
    PreResultNode* result;
    PreSuchThatNode* suchThat;
    PrePatternNode* pattern;
    vector< vector<string> > symbolTable;
public:
    QueryTree();
    ~QueryTree();

    bool getValidity();
    PreResultNode* getResult();
    PreSuchThatNode* getSuchThat();
    PrePatternNode* getPattern();
    vector< vector<string> > getSymbolTable();

    void setValidity(bool boolean);
    void setResult(vector<string> terms);
    void setSuchThat(vector<string> terms);
    void setPattern(vector<string> terms);
    void setSymbolTable(vector<string> terms);

    int getSymbolIndex(vector< vector<string> > table, string str);
    vector<string> stringToVector(string str, string delimiter);
    string removeSpace(string str);
    string getFirstToken(string str);
    string removeFirstToken(string str);
    string trim(string str);
};

#endif
