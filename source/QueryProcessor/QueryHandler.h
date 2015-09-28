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

	void getUseTable(vector<UseEntry_t> &useTab, vector<int> &userTable, vector<string> &usedTable);

	bool getParentTable(PreResultNode * result, string &firstAtt, vector<int> &parTable, string &secondAtt, vector<int> &nestTable);

	void getModifyTable(vector<ModifyEntry_t> &modTab, vector<pair<int, string>> &modTable);

	void getFollowTable(vector<pair<int, int>> &folTable);

	vector<pair<int, string>>  getConstTable();


	vector<int> getAssignTable();

	bool QueryHandler::containSign(string str);

	vector<int> QueryHandler::intersection(vector<int> vec1, vector<pair<int, int>> vec2);
	int QueryHandler::getPos(vector<int> intVec);

	vector<pair<int, string>> QueryHandler::toConvention(vector<ModifyEntry_t>  table);
	vector<pair<int, int>> QueryHandler::toConvention(vector<FollowEntry_t> table);
	vector<int> QueryHandler::toConvention(vector<ParentEntry_t>  table, int x);
	vector<int> QueryHandler::toConvention(vector<UseEntry_t> table, bool x);
	vector<string> QueryHandler::toConvention(vector<UseEntry_t> table, int x);

	void atoPair(pair<string, bool> &firstAttx, string &firstAtt);

	void handleUses(string &firstAtt, string &secondAtt, vector<int> &useVec, vector<string> &uvarVec);

	void handleParent(string &firstAtt, string &secondAtt, vector<int> &parVec);

	void handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &varVec);

	int handleFollows(string &firstAtt, string &secondAtt);

	vector< vector<string> > symTable;
	string handleSelect(QueryTree * query, PreResultNode * &result);
	bool isInt(string &secondAtt);
	string QueryHandler::getSymMean(string sym);
	vector<int> QueryHandler::intersection(vector<int> vec1, vector<int> vec2);
	vector<string> QueryHandler::intersection(vector<string> vec1, vector<string> vec2);
	vector<string> QueryHandler::intersection(vector<string> vec1, vector<pair<int, string>> vec2);
	vector<int> QueryHandler::intersection(vector<int> vec1, vector<pair<int, string>> vec2);
	vector<pair<int, string>> QueryHandler::intersection(vector<pair<int, string>> vec1, vector<pair<int, string>> vec2);
};
#endif