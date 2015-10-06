#ifndef QueryHandler_H
#define QueryHandler_H
#include "../PKB.h"
#include "../QueryProcessor/QueryHandler.h"
#include "../Preprocessor/QueryTree.h"
#include "../FollowTable.h"
#include "../ModifyTable.h"
#include "../ParentTable.h"


#include<string>
#include<vector>
using namespace std;

class QueryHandler {
public:
	QueryHandler();
	~QueryHandler();
	vector<string> queryRec(QueryTree* query);

	void atoPair(pair<string, bool> &firstAttx, string &firstAtt);
	void handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec, vector<string> &uvarVec);
	void handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec);
	void handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec, vector<string> &varVec);
	string handleFollows(string &firstAtt, string &secondAtt);
	string handleSelect(QueryTree * query, PreResultNode * &result);

	vector<pair<string, vector<string>>> QueryHandler::toConvention(vector<ModifyEntry_t>  table);
	vector<pair<string, string>> QueryHandler::toConvention(vector<FollowEntry_t> table);
	vector<string> QueryHandler::toConvention(vector<ParentEntry_t>  table, int x);
	vector<string> QueryHandler::toConvention(vector<UseEntry_t> table, bool x);
	vector<string> QueryHandler::toConvention(vector<UseEntry_t> table, int x);

	vector<pair<string, vector<string>>>  getConstTable();
	void getUseTable(vector<UseEntry_t> &useTab, vector<string> &userTable, vector<string> &usedTable);
	bool getParentTable(PreResultNode * result, string &firstAtt, vector<string> &parTable, string &secondAtt, vector<string> &nestTable);
	void getModifyTable(vector<ModifyEntry_t> &modTab, vector<pair<string, vector<string>>> &modTable);
	void getFollowTable(vector<pair<string, string>> &folTable);
	vector<string> QueryHandler::getAssignTable();
	
	int QueryHandler::getPos(vector<int> intVec);
	bool QueryHandler::containSign(string str);
	bool isInt(string &secondAtt);
	string QueryHandler::getSymMean(string sym);
	vector <vector<string>> symTable;
	bool QueryHandler::contain(string str, vector<string> vec);

	vector<string> QueryHandler::intersection(vector<string> vec1, vector<pair<string, string>> vec2);
	vector<string> QueryHandler::intersection(vector<string> vec1, vector<string> vec2);
	vector < pair<string, vector<string>>> QueryHandler::intersection(vector<string> vec1, vector<pair<string, vector<string>>> vec2);
	vector <string> QueryHandler::intersection(vector<string> vec1, vector < pair<string, vector<string>>> vec2, bool check);
};
#endif