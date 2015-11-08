#ifndef CALLTABLE_H
#define CALLTABLE_H

#include <vector>
#include <string>
#include "CallSTable.h"
#include <queue>

using namespace std;
typedef struct CallEntry_t {
	string caller;
	vector<string> callees;
	CallEntry_t(string caller, string callee) {
		CallEntry_t::caller = caller;
		CallEntry_t::callees.push_back(callee);
	}
} callEntry;

class CallTable {
private: vector<CallEntry_t> callTable;
		 bool checkRecurDFS(string caller, vector<string> callees,vector<string> processed);

public: 
	CallTable();
	~CallTable();
	bool addToTable(string caller, string callee);
	//void printTable();
	vector<string> getCallees(string caller);		//Calls("First", P2)
	vector<string> getCallers(string callee);		//Calls(P1,"First")
	int size();
	bool isContained(string caller);
	vector<CallEntry_t> getTable();			//return whole table as vector
	bool isContainedRecur();
	bool isCall(string caller, string callee);		//check Calls(p1,p2);
};


#endif
