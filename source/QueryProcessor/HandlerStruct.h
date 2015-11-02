#ifndef HandlerStruct_H
#define HandlerStruct_H
#include <string>
#include <vector>

using namespace std;

typedef struct RSEntry_t {
	string firstAtt;
	string secondAtt;
	int synCount;
	vector<string> vec;
	vector < pair<string, vector<string>>> table;
	vector <pair<string, string>> folTable;
} RSEntry;

typedef struct attEntry_t {
	string att;
	vector<int> reAtt;
	vector<int> reClause;
} attEntry;

typedef struct asgPat_t {
	string line;
	string modVar;
	vector<string> useVar;
} asgPat;
#endif