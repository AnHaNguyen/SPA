#ifndef QueryPreprocessor_H
#define QueryPreprocessor_H

#include<string>
#include<vector>
#include"QueryTree.h"

class QueryPreprocessor{
private:
    bool isValidQuery;
    vector<string> declarations;
    vector<string> relations;
    vector<string> patterns;
    vector<string> selections;

public:
    QueryPreprocessor();
    ~QueryPreprocessor();

    void setDeclarationTable(string str);
    void setSuchThatTable(string str);
    void setPatternTable(string str);
    void setResultTable(string str);

    QueryTree* startProcess(string input);
    bool isValidDeclaration(string str);
    bool isValidSuchThat(string str);
    bool isValidPattern(string str);
    bool isValidSelection(string str);

    vector<string> stringToVector(string original, string delimiter);
    int countWords(string str, string delimiter);
    string toLowerCase(string str);
    string trim(string str);
    string removeMultipleSpace(string str);
    string getFirstToken(string str);
    string removeFirstToken(string str);
    bool containWord(string str, string arr[], int size);
	void printTable(vector<string> table);


};


#endif
