#ifndef CallSTable_h
#define CallSTable_h
#include <string>
#include <vector>
#include "CallTable.h"

using namespace std;
typedef struct callSEntry_t {
	string caller;
	vector<string> callees;
	callSEntry_t(string caller, string callee) {
		callSEntry_t::caller = caller;
		callSEntry_t::callees.push_back(callee);
	}
} callSEntry;
class CallSTable {
	/*callEntry_t{
	string caller;
	vector<string> callees;
	}*/
private: vector<callSEntry_t> callSTable;
public: CallSTable();
		~CallSTable();
		bool addToTable(string caller, string callee);
		vector<string> getCallerS(string callee);
		vector<string> getCalleeS(string caller);
		int size();
		bool isContained(string caller);
		bool isCallS(string caller, string callee);
		vector<callSEntry_t> getTable();
};
#endif
