#include "ModifyTable.h"
#include "UseTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include <vector>
#include <string>
#include <iostream>

bool processModTable(ModifyTable* modTable, vector <string> code);
bool processUseTable(UseTable* useTable, vector <string> code);
bool processVarTable(VarTable* varTable, vector <string> code);
bool processProcTable(ProcTable* procTable, vector<string> code);
bool isConst(string var);

int main() {
	ModifyTable* modTable = new ModifyTable();
	UseTable* useTable = new UseTable();
	VarTable* varTable = new VarTable();
	ProcTable* procTable = new ProcTable();
	vector <string> code;
	processModTable(modTable, code);
	processUseTable(useTable, code);
	processProcTable(procTable, code);
	processVarTable(varTable, code);
}

bool processModTable(ModifyTable* modTable, vector <string> code) {
	
	for (unsigned i = 0; i < code.size(); i++) {			
		string line = code.at(i);
		unsigned pos = line.find('=');
		if (pos != string::npos){
			string var = line.substr(0, pos);
			modTable->add(i, var);							// i stands for line number, not index
		}
	}
	return true;
}

bool processUseTable(UseTable* useTable, vector<string> code) {
	for (unsigned i = 0; i < code.size(); i++) {
		string line = code.at(i);
		string type = line.substr(0, 5);
		if (type == "while") {
			useTable->add(i, line.substr(5, line.size() - 6));			//start after while (5), length = size - start - '{'
		}
		else {								//i stands for line number, not index
			unsigned pos = line.find('=');
			if (pos != string::npos) {
				line = line.substr(pos + 1, line.size() - pos - 1);			//remove =
				pos = line.find('+');
				while (pos != string::npos) {
					string var = line.substr(0, pos);
					if (!isConst(var)) {
						useTable->add(i, var);
					}
					line = line.substr(pos + 1, line.size() - pos - 1);		//remove +
					pos = line.find('+');
				}
				if (line.find('}') != string::npos) {
					line = line.substr(0, line.size() - 2);			//remove '}' and ';'
				}
				else {
					line = line.substr(0, line.size() - 1);		//remove ';'
				}
				if (!isConst(line)) {
					useTable->add(i, line);
				}
			}
		}
	}
	return true;
}

bool isConst(string var){
	return (isdigit(var[0]));
}

bool processVarTable(VarTable* varTable, vector<string> code) {
	UseTable* useTable = new UseTable();
	ModifyTable* modTable = new ModifyTable();
	processUseTable(useTable, code);
	processModTable(modTable, code);
	for (unsigned i = 0; i < useTable->size(); i++) {
		for (unsigned j = 0; j < useTable->getTable().at(i).usedVar.size(); j++) {
			string var = useTable->getTable().at(i).usedVar.at(i);
			varTable->addVar(var);
		}
	}
	for (unsigned i = 0; i < modTable->size(); i++) {
		string var = modTable->getTable().at(i).modifiedVar;
		varTable->addVar(var);
	}
	return true;
}

bool processProcTable(ProcTable* procTable, vector<string> code) {
	for (unsigned i = 0; i < code.size(); i++) {
		string proc = "procedure";
		unsigned pos = code.at(i).find(proc);
		if (pos != string::npos) {
			procTable->addProc(code.at(i).substr(9, code.at(i).size() - 10));	//start after "Proc.." minus '{'
		}
	}	
	return true;
}
