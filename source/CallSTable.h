#ifndef CallSTable_h
#define CallSTable_h
#include <string>
#include <vector>
#include "CallTable.h"

using namespace std;
class CallSTable {
	/*callEntry_t{
	string caller;
	vector<string> callees;
	}*/
private: vector<callEntry_t> callSTable;
public: CallSTable();
		~CallSTable();
		bool addToTable(string caller, string callee);
		vector<string> getCallerS(string callee);
		vector<string> getCalleeS(string caller);
		int size();
		bool isContained(string caller);
		bool isCallS(string caller, string callee);
		vector<callEntry_t> getTable();
};
#endif
