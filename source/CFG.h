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
		bool addToGraph(int lineNo);
		void makeChild(int lineNo, int parentLine);
		void makeParent(int lineNo, int childLine);
		vector<GNode*> findParent(int lineNo);
		vector<GNode*> findChild(int lineNo);
};
#endif
