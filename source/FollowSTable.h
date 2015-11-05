#ifndef FollowSTable_h
#define FollowSTable_h

#include <vector>
#include <string>

using namespace std;
typedef struct FollowSEntry_t {
	string lineNo;
	vector<string> followStmts;
	FollowSEntry_t(string lineNo, string next) {
		FollowSEntry_t::lineNo = lineNo;
		FollowSEntry_t::followStmts.push_back(next);
	}
} FollowSEntry;

class FollowSTable {
private:	vector<FollowSEntry_t> followSTable;
public:	FollowSTable();
		~FollowSTable();
		bool addToTable(string lineNo, string next);		
		vector<string> getPrevS(string next);			
		vector<string> getNextS(string prev);			
		int size();
		bool isContained(string lineNo);
		bool isFollowS(string lineNo, string child);
		vector<FollowSEntry_t> getTable();
		vector<string> getPrevSSlow(string next);
};

#endif

