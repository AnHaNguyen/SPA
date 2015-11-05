#ifndef HandlePT_H
#define HandlePT_H
#include "../PKB.h"
#include "../QueryProcessor/HUtility.h"
#include "../QueryProcessor/HandlerStruct.h"
#include <string>
#include <vector>
using namespace std;

class HandlePT {
public:
	HandlePT();
	~HandlePT();

	void handleAssign(string ptFirst, string ptSecond, 
		vector<string> &patVec, vector<pair<string, vector<string>>> &patTable);
	void HandlePT::handleIf(string ptFirst, string ptSecond, 
		vector<string> &patVec, vector<pair<string, string>> &ifTable);
	void HandlePT::handleWhile(string ptFirst, string ptSecond, 
		vector<string> &patVec, vector<pair<string, string>> &whileTable);
	void checkUnderscore(pair<string, bool> &firstAttU, string &firstAtt);
	bool containSign(string str);

};
#endif