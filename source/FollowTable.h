#ifndef FOLLOWTABLE_H
#define FOLLOWTABLE_H

#include <vector>
#include <string>
using namespace std;
typedef struct FollowEntry_t {		//follow(prev,next)
	string prev;
	string next;
	FollowEntry_t(string prev, string next) {
		FollowEntry_t::prev = prev;
		FollowEntry_t::next = next;
	}
}FollowEntry;

class FollowTable {
private: vector<FollowEntry_t> followTable;
public: FollowTable();
		~FollowTable();
		bool addToTable(string prev, string next);
		string getNext(string prev);			//return next stmt follows prev, if not found return ""
		string getPrev(string next);			//return prev stmt next follows, if not found return ""
		int size();						//size of table
		bool isContained(string prev);		//check if table contains a prev stmt
		vector<FollowEntry_t> getTable();		//return table
		bool isFollows(string prev, string next);	//check if next follows prev
};
#endif
