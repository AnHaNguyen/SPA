#ifndef ParentSTable_h
#define ParentSTable_h
#include <string>
#include <vector>
#include "ParentTable.h"

using namespace std;
typedef struct ParentSEntry_t {
	string lineNo;
	vector<string> child;
	ParentSEntry_t(string lineNo, string child) {
		ParentSEntry_t::lineNo = lineNo;
		ParentSEntry_t::child.push_back(child);
	}
} ParentSEntry;
class ParentSTable {
	/*ParentEntry_t{
	string parent;
	vector<string> child;
	}*/
private: vector<ParentSEntry_t> parentSTable;
public: ParentSTable();
		~ParentSTable();
		bool addToTable(string lineNo, string child);
		vector<string> getChildS(string line);
		vector<string> getParentS(string child);
		int size();
		bool isContained(string line);
		bool isParentS(string line, string child);
		vector<ParentSEntry_t> getTable();
};
#endif