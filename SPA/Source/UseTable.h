#ifndef USETABLE_H
#define USETABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct UseEntry_t {
	int lineNo;
	vector<string> usedVar;
} UseEntry;

class UseTable {
private: vector<UseEntry_t> useTable;

public:
	UseTable();
	~UseTable();
	void add(int lineNo, string var);
	vector<int> getUser(string var);
	vector<string> getUsed(int line);
	//void printTable();
	int size();
};
#endif
