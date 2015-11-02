#ifndef NEXTTABLE_H
#define NEXTTABLE_H

#include <vector>
#include <string>
using namespace std;
typedef struct NextEntry_t {
	string lineNo;					//stmt no
	vector<string> nextStmts;		//list of stmt that can be next of lineNo
	NextEntry_t(string line, string next) {
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
		bool addToTable(string line, string next);
		vector<string> getPrev(string next);					//return all stmt that next can be exct after
		vector<string> getNext(string prev);					//return all stmt that can be exct after prev
		int size();
		bool isContained(string line);
		bool isNext(string prev, string next);					//check if Next(prev,next)
		vector<string> getPrevS(string next);					//return all stmt that next can be exct after
		vector<string> getNextS(string prev);
		bool isNextS(string prev, string next);
		vector<NextEntry_t> getNextSTable();
		void addListToTable(string line, vector<string> list);
};

#endif
