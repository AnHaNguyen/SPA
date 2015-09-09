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
	bool queryRec(QueryTree* query);

	void handleParent(std::string &firstAtt, std::string &secondAtt, std::vector<int> &parVec);

	void handleModifies(string &firstAtt, string &secondAtt, vector<int> &modVec, vector<string> &varVec);

	vector<int> handleFollows(string &firstAtt, string &secondAtt);

	vector< vector<string> > symTable;
	string handleSelect(QueryTree * query, PreResultNode * &result);
	bool isInt(string &secondAtt);
	vector<int> handlePattern(QueryTree * query, PrePatternNode * &pattern);
	vector<int> handlePType(string &type, string &firstAtt, string &secondAtt);
	string QueryHandler::getSymMean(string sym);
}
#endif