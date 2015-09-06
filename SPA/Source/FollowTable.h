#ifndef FOLLOWTABLE_H
#define FOLLOWTABLE_H

#include <vector>
#include <string>
using namespace std;
typedef struct FollowEntry_t {		//follow(prev,next)
	int prev;
	int next;
	FollowEntry_t(int prev, int next) {
		FollowEntry_t::prev = prev;
		FollowEntry_t::next = next;
	}
}FollowEntry;

class FollowTable {
private: vector<FollowEntry_t> followTable;
public: FollowTable();
		~FollowTable();
		bool addToTable(int prev, int next);
		int getNext(int prev);			//return next stmt follows prev, if not found return -1
		int getPrev(int next);			//return prev stmt next follows, if not found return -1
		int size();						//size of table
		bool isContained(int prev);		//check if table contains a prev stmt
		vector<FollowEntry_t> getTable();		//return table
};
#endif
