#ifndef VARTABLE_H
#define VARTABLE_H

#include <string>
#include <vector>

using namespace std;

//typedef struct var_element_t {
	//string var;
	//int line;
//} var_element;


class VarTable {
private:
	vector<string> varTable;

public:
	VarTable();
	~VarTable();
	void addVar(string var);
	string getVar(int index);
	string printVarTable();
	int indexOf(string var);
	int size();
}; 

#endif