#ifndef PARENTTABLE_H
#define PARENTTABLE_H

#include <vector>
#include <string>
#include "ParentSTable.h"

using namespace std;
typedef struct ParentEntry_t {
	string lineNo;
	vector<string> child;
	ParentEntry_t(string lineNo, string child) {
		ParentEntry_t::lineNo = lineNo;
		ParentEntry_t::child.push_back(child);
	}
} ParentEntry;

class ParentTable {
private:	vector<ParentEntry_t> parentTable;
public:	ParentTable();
		~ParentTable();
		bool addToTable(string lineNo, string child);		//add to Table at lineNo parent with Child
		vector<string> getChild(string parent);			//get CHildList, return empty list if not found
		string getParent(string child);			//get Parent, return "" if not found
		int size();
		bool isContained(string lineNo);
		bool isParent(string lineNo, string child);
		vector<ParentEntry_t> getTable();
		string getParentSlow(string child);
};

#endif
