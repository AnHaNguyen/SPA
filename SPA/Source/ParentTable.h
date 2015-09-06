#ifndef PARENTTABLE_H
#define PARENTTABLE_H

#include <vector>
#include <string>

using namespace std;
typedef struct ParentEntry_t {
	int lineNo;
	vector<int> child;
	ParentEntry_t(int lineNo, int child) {
		ParentEntry_t::lineNo = lineNo;
		ParentEntry_t::child.push_back(child);
	}
} ParentEntry;

class ParentTable {
private:	vector<ParentEntry_t> parentTable;
public:	ParentTable();
		~ParentTable();
		bool addToTable(int lineNo, int child);		//add to Table at lineNo parent with Child
		vector<int> getChild(int parent);			//get CHildList, return empty list if not found
		int getParent(int child);			//get Parent, return -1 if not found
		int size();
		bool isContained(int lineNo);
		vector<ParentEntry_t> getTable();
};

#endif