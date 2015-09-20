#ifndef USETABLE_H
#define USETABLE_H

#include <string>
#include <vector>
#include "Utility.h"

using namespace std;
typedef struct UseEntry_t {
	string userLine;
	vector<string> usedVar;
	UseEntry_t(string user, string var) {
		UseEntry_t::userLine = user;
		UseEntry_t::usedVar.push_back(var);
	}
} UseEntry;

class UseTable {
private: vector<UseEntry_t> useTable;

public:
	UseTable();
	~UseTable();
	bool addToTable(string userLine, string var);		//include Proc
	vector<string> getUser(string var);		//get the list of line that uses var
	vector<string> getUsed(string user);		//get the list of var used in a line
	bool isUsed(string user, string var);			//check if line uses var
	//void printTable();
	int size();
	bool isContained(string userLine);
	vector<UseEntry_t> getTable();
};
#endif
