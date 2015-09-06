#ifndef CFG_H
#define CFG_H
#include "GNode.h"

using namespace std;
class CFG {
private:
	string procedure;
	vector<GNode* > cfg;

public: CFG(string procedure);
		~CFG();
		string getProcedure();
		int size();	//size of CFG
		void addToGraph(int lineNo);
		void makeChild(int lineNo, int parentLine);
		void makeParent(int lineNo, int childLine);
		vector<int> findParent(int lineNo);
		vector<int> findChild(int lineNo);
};
#endif
