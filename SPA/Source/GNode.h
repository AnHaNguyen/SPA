#ifndef GNODE_H
#define GNODE_H

#include <vector>
#include <string>

using namespace std;
class GNode {
private:
	string procedure;
	int line;
	vector<GNode* > child;
	vector<GNode* > parent;

public:
	GNode(string procedure, int line);		//construction
	
	~GNode();		//destruction
	
	void setProcedure(string procedure);

	void setLine(int line);

	void setChild(GNode* child);

	void setParent(GNode* parent);

	string getProcedure();

	int getLine();

	vector<GNode* > getChild();

	vector<GNode* > getParent();
};

#endif