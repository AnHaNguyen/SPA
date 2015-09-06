#ifndef TNODE_H
#define TNODE_H

#include <string>
#include <vector>

using namespace std;

class TNode			//Class define Node of AST
{
private:
	string value;	//value of Node, can be null
	string type;	//Type of Node
	int line;		//code line that appears the Node
	TNode* parent;	//represent the parent on the tree( not relationship parent)
	vector<TNode* > childList;		
	
public:
	TNode(string value, string type, int line); //constructor
	
	~TNode();	//destructor

	void setValue(string val);

	void setType(string type);

	void setLine(int line);

	void setParent(TNode* parent);

	void setChild(TNode* right);

	string getValue();

	string getType();

	int getLine();

	TNode* getParent();

	vector<TNode* > getChildList();
	bool equals(TNode obj);		//check if 2 nodes are equals (line, value, type)
};

#endif