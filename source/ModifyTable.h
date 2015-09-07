#ifndef MODIFYTABLE_H
#define MODIFYTABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct ModifyEntry_t {
	int lineNo;
	string modifiedVar;
	ModifyEntry_t(int lineNo, string var) {
		ModifyEntry_t::lineNo = lineNo;
		ModifyEntry_t::modifiedVar = var;
	}
} ModifyEntry;

class ModifyTable {
private: vector<ModifyEntry_t> modifyTable;
		 
public: 
	ModifyTable();
	~ModifyTable();
	vector<ModifyEntry_t> getTable();			//return whole table
	bool add(int lineNo, string var);
	vector<int> getModifier(string var);		//return all modifier which modify a var
	string getModified(int line);		//return the modified var of a line
	bool isModified(int line, string var);		//check if a line modifies a var
	//void printTable();
	int size();
	bool isContained(int lineNo);			//check if a line is contained in the table
};

#endif
