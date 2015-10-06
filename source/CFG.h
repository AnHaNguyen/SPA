#ifndef CFG_H
#define CFG_H
#include "GNode.h"
#include "NextTable.h"

using namespace std;
class CFG {
private:
	string procedure;
	vector<GNode* > cfg;

public: CFG(string procedure);
		~CFG();
		string getProcedure();
		int size();	//size of CFG
		bool addToGraph(GNode* node);
		void makeChild(GNode* parent, GNode* child);			//parent already inside CFG
		void makeParent(GNode* child, GNode* parent);			//child already inside CFG
		vector<GNode*> findParent(int lineNo);
		vector<GNode*> findChild(int lineNo);
		void buildNextTable(NextTable* nextTable);
};
#endif
