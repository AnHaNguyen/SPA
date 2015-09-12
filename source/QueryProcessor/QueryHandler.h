#ifndef QueryHandler_H
#define QueryHandler_H
#include"../Preprocessor/QueryTree.h"

#include<string>
#include<vector>
using namespace std;

class QueryHandler {
public:
	QueryHandler();
	~QueryHandler();
	vector<string> queryRec(QueryTree* query);

	bool QueryHandler::containSign(string str);

	void atoPair(std::pair<std::string, bool> &firstAttx, std::string &firstAtt);

	void handleUses(std::string &firstAtt, std::string &secondAtt, std::vector<int> &useVec, std::vector<std::string> &uvarVec);

	void handleParent(std::string &firstAtt, std::string &secondAtt, std::vector<int> &parVec);

	void handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &varVec);

	vector<int> handleFollows(string &firstAtt, string &secondAtt);

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