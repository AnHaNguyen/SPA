#ifndef MODIFYTABLE_H
#define MODIFYTABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct ModifyEntry_t {
	string modifier;
	vector<string> modifiedVar;

	ModifyEntry_t(string proc, string var) {
		ModifyEntry_t::modifier = proc;
		ModifyEntry_t::modifiedVar.push_back(var);
	}
} ModifyEntry;

class ModifyTable {
private: vector<ModifyEntry_t> modifyTable;
		 
public: 
	ModifyTable();
	~ModifyTable();
	vector<ModifyEntry_t> getTable();			//return whole table
	bool addToTable(string modifier, string var);
	vector<string> getModifier(string var);		//return all modifiers which modify a var
	vector<string> getModified(string modifier);		//return the modified vars of a line/proc
	bool isModified(string modifier, string var);		//check if a line/proc modifies a var
	//void printTable();
	int size();
	bool isContained(string lineNo);			//check if a line is contained in the table
};

#endif
