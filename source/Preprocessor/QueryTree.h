#ifndef QueryTree_H
#define QueryTree_H

#include<string>
#include<vector>
#include "PreResultNode.h"
#include "PreSuchThatNode.h"
#include "PrePatternNode.h"
#include "PreWithNode.h"
using namespace std;

class QueryTree{
private:
    bool isValid;
    PreResultNode* result;
    PreSuchThatNode* suchThat;
    PrePatternNode* pattern;
	PreWithNode* with;
    vector< vector<string> > symbolTable;
public:
    QueryTree();
    ~QueryTree();

    bool getValidity();
    PreResultNode* getResult();
    PreSuchThatNode* getSuchThat();
    PrePatternNode* getPattern();
	PreWithNode* getWith();
    vector< vector<string> > getSymbolTable();

    void setValidity(bool boolean);
    void setResult(vector<string> terms);
    void setSuchThat(vector<string> terms);
    void setPattern(vector<string> terms);
	void setWith(vector<string> terms);
    void setSymbolTable(vector<string> terms);

    int getSymbolIndex(vector< vector<string> > table, string str);
	string getSynType(vector< vector<string> > table, string str);
	string getRefType(string str);
	bool isValidSuchThatAttribute(string syn, string first, string second);
	bool isValidPatternAttribute(string syn, string first, string second, string third);
	bool isValidWithAttribute(string left, string right);
	bool isValidResultAttribute(vector<string> table, string synonym, string attr);
	bool isValidQuotation(string str);
	bool isValidRef(string str);
	bool isValidAttrRef(string str);
	bool isValidIdent(string str);
	bool isValidSynonym(vector< vector<string> > table, string str);
	bool isValidName(string str);
	bool isValidStmtRef(vector< vector<string> > table, string str);
	bool isValidEntRef(vector< vector<string> > table, string str);
	bool isValidLineRef(vector< vector<string> > table, string str);
	bool isValidExpressionSpec(string str);
	bool isAssign(string str);
	bool isVariable(string str);
	bool isIf(string str);
	bool isWhile(string str);
	bool isInSymbolTable(vector< vector<string> > table, string str);
	bool isInteger(string str);
	bool containWord(string str, string arr[], int size);
	string toLowerCase(string str);
	vector<string> toExprVector(string str);
	bool isOperation(string str);
	bool isValidChar(vector<string>);
	bool isValidBracket(string str);
	bool isValidNodeRef(string str);

    vector<string> stringToVector(string str, string delimiter);
    string removeSpace(string str);
    string getFirstToken(string str);
    string removeFirstToken(string str);
    string trim(string str);	
	
	void printTable(vector<string> table);
	void printDoubleTable(vector< vector<string> > table);
	int countWords(string str, string delimiter);
};

#endif
