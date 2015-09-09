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

	vector<int> handleFollows(std::string &firstAtt, std::string &secondAtt);

	vector< vector<string> > symTable;
	string handleSelect(QueryTree * query, PreResultNode * &result);
	bool isInt(std::string &secondAtt);
	vector<int> handlePattern(QueryTree * query, PrePatternNode * &pattern);
	vector<int> handlePType(string &type, string &firstAtt, string &secondAtt);
	string QueryHandler::getSymMean(string sym);
}
#endif