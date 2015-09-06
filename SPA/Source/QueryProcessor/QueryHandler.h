#ifndef QueryHandler_H
#define QueryHandler_H
#include"../Preprocessor/QueryTree.h"

#include<string>

class QueryHandler {
public:
		QueryHandler();
		~QueryHandler();
		bool queryRec(QueryTree* query);
		vector<int> handlePT(PrePatternNode * pattern);
		vector<int> handlePType(std::string &type, std::string &firstAtt, std::string &secondAtt);
		vector<int> handleST(PreSuchThatNode * suchThat);
};
#endif