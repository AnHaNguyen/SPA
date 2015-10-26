#ifndef CallSTable_h
#define CallSTable_h
#include <string>
#include <vector>
#include "CallTable.h"

using namespace std;
typedef struct CallSEntry_t {
	string caller;
	vector<string> callees;
	CallSEntry_t(string caller, string callee) {
		CallSEntry_t::caller = caller;
		CallSEntry_t::callees.push_back(callee);
	}
} callSEntry;
class CallSTable {
	/*CallEntry_t{
	string caller;
	vector<string> callees;
	}*/
private: vector<CallSEntry_t> callSTable;
public: CallSTable();
		~CallSTable();
		bool addToTable(string caller, string callee);
		vector<string> getCallerS(string callee);
		vector<string> getCalleeS(string caller);
		int size();
		bool isContained(string caller);
		bool isCallS(string caller, string callee);
		vector<CallSEntry_t> getTable();
};
#endif
