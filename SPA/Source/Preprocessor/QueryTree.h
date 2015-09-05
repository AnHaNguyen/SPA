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
	string getSynType(vector< vector<string> > table, string str);
	bool isValidSuchThatAttribute(string syn, string first, string second);
	bool isValidPatternAttribute(string syn, string first, string second);
	bool isValidIdent(string str);
	bool isValidSynonym(vector< vector<string> > table, string str);
	bool isValidName(string str);
	bool isValidStmtRef(vector< vector<string> > table, string str);
	bool isValidEntRef(vector< vector<string> > table, string str);
	bool isValidExpressionSpec(string str);
	bool isAssign(string str);
	bool isInSymbolTable(vector< vector<string> > table, string str);
	bool isInteger(string str);
	bool containWord(string str, string arr[], int size);
	string toLowerCase(string str);

    vector<string> stringToVector(string str, string delimiter);
    string removeSpace(string str);
    string getFirstToken(string str);
    string removeFirstToken(string str);
    string trim(string str);	
	
	void printTable(vector<string> table);
	void printDoubleTable(vector< vector<string> > table);
};

#endif
