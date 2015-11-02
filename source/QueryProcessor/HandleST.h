#ifndef HandleFollows_H
#define HandleFollows_H
#include "../Preprocessor/QueryTree.h"
#include "../QueryProcessor/HUtility.h"
#include "../FollowTable.h"
#include "../PKB.h"
#include <string>
#include <vector>
using namespace std;

class HandleST {
public:
	HandleST();
	~HandleST();
	string handleFollows(string &firstAtt, string &secondAtt);
	void handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec, vector<string> &varVec);
	void handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec);
	void handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec);
	vector<string> handleNext(string &firstAtt, string &secondAtt);
	vector<string> handleCalls(string firstAtt, string secondAtt);

	bool isFollowsS(string firstAtt, string secondAtt);
	bool isParentS(string firstAtt, string secondAtt);
};
#endif