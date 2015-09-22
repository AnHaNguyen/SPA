#ifndef CONSTTABLE_H
#define CONSTTABLE_H

#include <vector>
#include <string>

using namespace std;
typedef struct ConstEntry_t {	
	string line;
	vector<string> constants;
	ConstEntry_t(string line, string constant) {
		ConstEntry_t::line = line;
		ConstEntry_t::constants.push_back(constant);
	}
}ConstEntry;

class ConstTable {
private: vector<ConstEntry_t> constTable;
public: ConstTable();
		~ConstTable();
		bool addToTable(string line, string constant);		//also include proc and containers
		vector<string> getConst(string line);			//return the constants used in a line
		vector<string> getLine(string constant);		//return all lines use this const
		//bool isContained(string constant);		//check if a constant is in table
		bool isContained(string line);				//check if a line is in table
		bool isConstOfLine(string line, string constant);	//check if a line uses a constant
		int size();
		vector<ConstEntry_t> getTable();			//return whole table
};

#endif
