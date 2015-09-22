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
		 bool checkRecurDFS(string caller, vector<string> callees);

public: 
	CallTable();
	~CallTable();
	bool addToTable(string caller, string callee);
	//void printTable();
	vector<string> getCallees(string caller);		//Calls("First", P2)
	vector<string> getCallers(string callee);		//Calls(P1,"First")
	int size();
	bool isContained(string caller);
	vector<callEntry_t> getTable();			//return whole table as vector
	bool isContainedRecur();
	bool isCall(string caller, string callee);		//check Calls(p1,p2);
};


#endif