#ifndef HandleRS_H
#define HandleRS_H
#include "../Preprocessor/AttrRef.h"
#include "../QueryProcessor/HUtility.h"
#include "../QueryProcessor/HandlerStruct.h"
#include <string>
#include <vector>
using namespace std;

class HandleRS {
public:
	HandleRS();
	~HandleRS();
	string handleSelect(QueryTree * query, PreResultNode * &result);
	bool folAss(string att, string firstAtt, vector < pair<string, string>> folTable, int i);
	void checkSS(vector<string> &vec, string firstAtt, string secondAtt);
	void checkPSS(vector<pair<string, string>> &vec, string firstAtt, string secondAtt);
	void HandleRS::checkPSV(vector<pair<string, vector<string>>> &vec, string firstAtt, string secondAtt);
	void rmEString(vector<string> vec);

};
#endif