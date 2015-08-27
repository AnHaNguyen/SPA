#ifndef MODIFYTABLE_H
#define MODIFYTABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct ModifyEntry_t {
	int lineNo;
	vector<string> modifiedVar;
} ModifyEntry;

class ModifyTable {
private: vector<ModifyEntry_t> modifyTable;
		 
public: 
	ModifyTable();
	~ModifyTable();
	void add(int lineNo, string var);
	vector<int> getModifier(string var);
	vector<string> getModified(int line);
	//void printTable();
	int size();
};

#endif
