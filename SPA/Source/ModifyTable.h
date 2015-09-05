#ifndef MODIFYTABLE_H
#define MODIFYTABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct ModifyEntry_t {
	int lineNo;
	vector<string> modifiedVar;
	ModifyEntry_t(int lineNo, string var) {
		ModifyEntry_t::lineNo = lineNo;
		ModifyEntry_t::modifiedVar.push_back(var);
	}
} ModifyEntry;

class ModifyTable {
private: vector<ModifyEntry_t> modifyTable;
		 
public: 
	ModifyTable();
	~ModifyTable();
	bool add(int lineNo, string var);
	vector<int> getModifier(string var);		//return all modifier which modify a var
	vector<string> getModified(int line);		//return all modified var of a line
	//void printTable();
	int size();
	bool isContained(int lineNo);			//check if a line is contained in the table
};

#endif
