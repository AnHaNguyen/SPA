#ifndef CALLTABLE_H
#define CALLTABLE_H

#include <vector>
#include <string>
#include "Utility.h"

using namespace std;
typedef struct callEntry_t {
	string caller;
	vector<string> callees;
	callEntry_t(string caller, string callee) {
		callEntry_t::caller = caller;
		callEntry_t::callees.push_back(callee);
	}
} callEntry;

class CallTable {
private: vector<callEntry_t> callTable;

public: 
	CallTable();
	~CallTable();
	void add(string caller, string callee);
	//void printTable();
	vector<string> getCallees(string caller);
	vector<string> getCallers(string callee);
	int size();
	bool isContained(string caller);
};


#endif