#ifndef HandlePT_H
#define HandlePT_H
#include "../PKB.h"
#include "../Preprocessor/QueryTree.h"
#include "../QueryProcessor/HUtility.h"
#include "../QueryProcessor/HandlerStruct.h"
#include <string>
#include <vector>
using namespace std;

class HandlePT {
public:
	HandlePT();
	~HandlePT();

	void handleAssign(string ptFirst, string ptSecond, string selType, string rs,
		vector<string> &patVec, vector<pair<string, vector<string>>> patTable, vector<asgPat_t> asgPat);
	void checkUnderscore(pair<string, bool> &firstAttU, string &firstAtt);
	bool containSign(string str);

};
#endif