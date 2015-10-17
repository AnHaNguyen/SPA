#ifndef ParentSTable_h
#define ParentSTable_h
#include <string>
#include <vector>
#include "ParentTable.h"

using namespace std;
class ParentSTable {
	/*ParentEntry_t{
	string parent;
	vector<string> child;
	}*/
private: vector<ParentEntry_t> parentSTable;
public: ParentSTable();
		~ParentSTable();
		bool addToTable(string lineNo, string child);
		vector<string> getChildS(string line);
		vector<string> getParentS(string child);
		int size();
		bool isContained(string line);
		bool isParentS(string line, string child);
		vector<ParentEntry_t> getTable();
};
#endif