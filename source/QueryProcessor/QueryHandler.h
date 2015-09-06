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
	string handleSelect(QueryTree * query, PreResultNode * &result, vector<vector<string>> &symTable);
	vector<int> handleSuchThat(QueryTree * query, PreSuchThatNode * &suchThat, vector<vector<string>> &symTable);
	vector<int> handlePattern(QueryTree * query, PrePatternNode * &pattern, vector<vector<string>> &symTable);
	vector<int> handlePType(string &type, string &firstAtt, string &secondAtt);
	string QueryHandler::getSymMean(string sym, vector<vector<string>> &symTable);
}
#endif