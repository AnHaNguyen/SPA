#ifndef HandleFollows_H
#define HandleFollows_H
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
	void handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec);
	void handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec);
	void handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec);
	void handleNext(string &firstAtt, string &secondAtt, vector<string> &nextVec);
	void handleCalls(string firstAtt, string secondAtt, vector<string> &callVec);
	void handleAffect(string firstAtt, string secondAtt, vector<string> &affVec);

	bool isFollowsS(string firstAtt, string secondAtt);
	bool isParentS(string firstAtt, string secondAtt);
};
#endif