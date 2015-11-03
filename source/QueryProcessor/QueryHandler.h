#ifndef QueryHandler_H
#define QueryHandler_H
#include "../Preprocessor/QueryTree.h"
#include "../Preprocessor/AttrRef.h"
#include "../PKB.h"
#include "../QueryProcessor/HandleRS.h"
#include "../QueryProcessor/HandleST.h"
#include "../QueryProcessor/HandlePT.h"
#include "../QueryProcessor/HUtility.h"
#include "../QueryProcessor/HandlerStruct.h"
#include "../FollowTable.h"
#include "../FollowSTable.h"
#include "../ModifyTable.h"
#include "../ParentTable.h"
#include "../ParentSTable.h"
#include "../ProgLine.h"

#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
using namespace std;

class QueryHandler {
public:
	QueryHandler();
	~QueryHandler();
	vector<string> queryRec(QueryTree* query);
};
#endif