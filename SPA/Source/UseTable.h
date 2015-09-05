#ifndef USETABLE_H
#define USETABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct UseEntry_t {
	int lineNo;
	vector<string> usedVar;
	UseEntry_t(int lineNo, string var) {
		UseEntry_t::lineNo == lineNo;
		UseEntry_t::usedVar.push_back(var);
	}
} UseEntry;

class UseTable {
private: vector<UseEntry_t> useTable;

public:
	UseTable();
	~UseTable();
	bool add(int lineNo, string var);
	vector<int> getUser(string var);
	vector<string> getUsed(int line);
	//void printTable();
	int size();
	bool isContained(int lineNo);
};
#endif
