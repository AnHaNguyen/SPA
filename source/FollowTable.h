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
		int getNext(int prev);
		int getPrev(int next);
		int size();
		bool isContained(int prev);
};
#endif
