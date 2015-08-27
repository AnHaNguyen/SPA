#ifndef PROCTABLE_H
#define PROCTABLE_H

#include <string>
#include <vector>

using namespace std;

class ProcTable {
private: 
	vector<string> procTable;

public:
	ProcTable();
	~ProcTable();
	void addProc(string proc);
	int indexOf(string proc);
	string getProc(int index);
	int size();
	string printProcTable();
};

#endif