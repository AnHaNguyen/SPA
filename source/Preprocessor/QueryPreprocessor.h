#ifndef QueryPreprocessor_H
#define QueryPreprocessor_H

#include<string>
#include<vector>
#include<list>
#include<iterator>
#include"QueryTree.h"

class QueryPreprocessor{
private:
    vector<string> declarations;
    vector<string> relations;
    vector<string> patterns;
	vector<string> withs;
    vector<string> selections;

public:
    QueryPreprocessor();
    ~QueryPreprocessor();

    void setDeclarationTable(string str);
    void setSuchThatTable(string str);
    void setPatternTable(string str);
	void setWithTable(string str);
	void setResultTable(string str);

	vector<string> getDeclarationTable();
	vector<string> getSuchThatTable();
	vector<string> getPatternTable();
	vector<string> getWithTable();
	vector<string> getResultTable();

	vector<string> start(string line);
    QueryTree* startProcess(string declare, string input);
    bool isValidDeclaration(string str);
	bool isValidSelection(string str);
    bool isValidSuchThat();
    bool isValidPattern();
	bool isValidWith();

    bool containWord(string str, string arr[], int size);
	vector<string> extractContent(string str, string clause);
	int identifyKeyword(string str);
	vector<vector<string>> seperateClause(string str, string clause);
	vector<string> setClauseTable(vector< vector<string> > table);

	vector<string> stringToVector(string original, string delimiter);
	int countWords(string str, string delimiter);
	string toLowerCase(string str);
	string trim(string str);
	string removeMultipleSpace(string str);
	string removeSpace(string str);
	string getFirstToken(string str);
	string removeFirstToken(string str);

	void printTable(vector<string> table);
};


#endif
