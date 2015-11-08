#ifndef CallLine_h
#define CallLine_h
#include <string>
#include <vector>

using namespace std;
typedef struct CallLineEntry_t {
	string lineNo;
	string callee;
	CallLineEntry_t(string line, string call) {
		lineNo = line;
		callee = call;
	}
} CallLineEntry;

class CallLine {
private: vector<CallLineEntry_t> callLine;
public: CallLine();
		~CallLine();
		void addToTable(string line, string callee);
		string getCallee(string line);
		vector<string> getCallers(string callee);
		vector < pair<string, string>> getTable();
		int size();

};
#endif
