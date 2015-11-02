#ifndef HUtility_H
#define HUtility_H
#include "../PKB.h"
#include "../Preprocessor/QueryTree.h"
#include "../Preprocessor/AttrRef.h"
#include "../QueryProcessor/HandlerStruct.h"
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
	bool isInt(string att);
	bool contain(vector<string> vec, string str);
	bool contain(vector<int> vec, int str);
	int contain(vector<attEntry_t> vec, string str);

	//Set symTable
	void setSymTable(vector<vector<string>> symbolTable);

	//Get tables
	vector<string> getAssignTable();
	void HUtility::getUseTable(vector<pair<string, vector<string>>> &useTable);
	vector<pair<string, vector<string>>>  getConstTable();
	void getParentTable(vector<pair<string, vector<string>>> &parTable);
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
	int intersectionSS(vector<string> &vec1, vector<string> &vec2, int check);
	int intersectionPSS(vector<string> &vec1, vector<pair<string, string>> &vec2, int check);
	int intersectionPSV(vector<string> &vec1, vector<pair<string, vector<string>>> &vec2, int check);
	int intersectionPPSS(vector<pair<string, string>> &vec1, vector<pair<string, string>> &vec2, int att1, int att2);
	int intersectionPPSM(vector<pair<string, string>> &vec1, vector<pair<string, vector<string>>> &vec2, int att1, int att2);
	int intersectionPPSV(vector<pair<string, vector<string>>> &vec1, vector<pair<string, vector<string>>> &vec2, int att1, int att2);

	void checkQuotation(pair<string, bool> &AttQ, string &Att);
};
#endif