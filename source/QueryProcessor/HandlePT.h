#ifndef HandlePT_H
#define HandlePT_H
#include "../Preprocessor/QueryTree.h"
#include "../QueryProcessor/HUtility.h"
#include <string>
#include <vector>
using namespace std;

class HandlePT {
public:
	HandlePT();
	~HandlePT();

	void handleAssign(string ptFirst, string ptSecond, string selType, string rs,
		vector<string> &patVec, vector<string> &pvarVec, vector<string> &pconVec);

	void checkUnderscore(pair<string, bool> &firstAttU, string &firstAtt);
	void checkQuotation(pair<string, bool> &AttQ, string &Att);
	bool containSign(string str);

};
#endif