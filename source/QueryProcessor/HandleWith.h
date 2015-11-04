#ifndef HandleWith_H
#define HandleWith_H

#include "../PKB.h"
#include "../QueryProcessor/HUtility.h"
#include <string>
#include <vector>

using namespace std;

class HandleWith {
public:
	HandleWith();
	~HandleWith();

	void handleWith(vector<string> &withVec, string firstAtt, string secondAtt);

};
#endif