#ifndef NEXTTABLE_H
#define NEXTTABLE_H

#include <vector>
#include <string>
using namespace std;
typedef struct NextEntry_t {
	int lineNo;					//stmt no
	vector<int> nextStmts;		//list of stmt that can be next of lineNo
	NextEntry_t(int line, int next) {
		lineNo = line;
		nextStmts.push_back(next);
	}
}NextEntry;
using namespace std;
class NextTable {
private: vector<NextEntry_t> nextTable;
public: NextTable();
		~NextTable();
		vector<NextEntry_t> getTable();			//return table as vector
		bool addToTable(int line, int next);
		int getPrev(int next);					//can confuse w FollowTable
		vector<int> getNext(int prev);
		int size();
		bool isContained(int line);
		bool isNext(int prev, int next);
};

#endif
