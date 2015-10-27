#ifndef HUtility_H
#define HUtility_H
#include "../PKB.h"
#include "../Preprocessor/QueryTree.h"
#include "../Preprocessor/AttrRef.h"
#include "../QueryProcessor/HUtility.h"
#include "../FollowTable.h"
#include "../ModifyTable.h"
#include "../ParentTable.h"
#include "../CallTable.h"
#include "../ProgLine.h"

#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
using namespace std;

class HUtility {
public:
	HUtility();
	~HUtility();

	string getSymMean(string sym);
	int getPos(vector<int> intVec);
	bool isInt(string &secondAtt);
	bool contain(vector<string> vec, string str);
	
	//Set symTable
	void setSymTable(vector<vector<string>> symbolTable);

	//Get tables
	vector<string> getAssignTable();
	void getUseTable(vector<UseEntry_t> &useTab, vector<string> &userTable, vector<string> &usedTable);
	vector<pair<string, vector<string>>>  getConstTable();
	bool getParentTable(PreResultNode * result, string &firstAtt, vector<string> &parTable, string &secondAtt, vector<string> &nestTable);
	void getModifyTable(vector<pair<string, vector<string>>> &modTable);
	void getFollowTable(vector<pair<string, string>> &folTable);
	void getNextTable(vector<pair<string, vector<string>>> &nextTable);
	void getCallTable(vector<pair<string, vector<string>>> &callTable);

	//To convention
	vector<string> getParentRelated(vector<ParentEntry_t>  table, int x);
	vector<string> getUseRelated(vector<UseEntry_t> table, int x);

	//Intersection
	vector<string> intersection(vector<string> vec1, vector<pair<string, string>> vec2);
	vector<string> intersection(vector<string> vec1, vector<string> vec2);
	vector < pair<string, vector<string>>> intersection(vector<string> vec1, vector<pair<string, vector<string>>> vec2);
	vector <string> intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2, bool check);

	void checkQuotation(pair<string, bool> &AttQ, string &Att);
};
#endif