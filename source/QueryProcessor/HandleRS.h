#ifndef HandleRS_H
#define HandleRS_H
#include "../Preprocessor/QueryTree.h"
#include "../Preprocessor/AttrRef.h"
#include "../QueryProcessor/HUtility.h"
#include <string>
#include <vector>
using namespace std;

class HandleRS {
public:
	HandleRS();
	~HandleRS();
	string handleSelect(QueryTree * query, PreResultNode * &result);
	bool folAss(string att, string firstAtt, vector < pair<string, string>> folTable, int i);

};
#endif