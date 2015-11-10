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
	void handleFollows(string &firstAtt, string &secondAtt, vector<string> &folVec);
	void handleModifies(string &firstAtt, string &secondAtt, vector<string> &modVec);
	void handleParent(string &firstAtt, string &secondAtt, vector<string> &parVec);
	void handleUses(string &firstAtt, string &secondAtt, vector<string> &useVec);
	void handleNext(string &firstAtt, string &secondAtt, vector<string> &nextVec);
	void handleCalls(string firstAtt, string secondAtt, vector<string> &callVec);
	void handleAffect(string firstAtt, string secondAtt, vector<string> &affVec);
	void handleContain(vector<string> &conVec, vector<pair<string, string>> &conTable, string firstAtt, string secondAtt);
	void handleContainS(vector<string> &conVec, vector<pair<string, string>> &conTable, string firstAtt, string secondAtt);
	void handleSibling(vector<string> &sibVec, vector<pair<string, string>> &sibTable, string firstAtt, string secondAtt);

	/*void convertPSM(vector<pair<string, string>> &vec1, vector<pair<string, vector<string>>> &vec2, int check) {
		vector<string> visited;
		if (check == 1) {
			for (size_t i = 0; i < vec1.size(); i++) {
				if (!HUtility().contain(visited, vec1[i].first)) {
					pair<string, vector<string>> temp;
					visited.push_back(vec1[i].first);
					temp.first = vec1[i].first;
					for (size_t j = 0; j < vec1.size(); j++) {
						if (vec1[i].first == vec1[j].first) {
							temp.second.push_back(vec1[j].second);
						}
					}
					vec2.push_back(temp);
				}
			}
		}
		if (check == 2) {
			for (size_t i = 0; i < vec1.size(); i++) {
				if (!HUtility().contain(visited, vec1[i].second)) {
					pair<string, vector<string>> temp;
					visited.push_back(vec1[i].second);
					temp.first = vec1[i].second;
					for (size_t j = 0; j < vec1.size(); j++) {
						if (vec1[i].first == vec1[j].second) {
							temp.second.push_back(vec1[j].first);
						}
					}
					vec2.push_back(temp);
				}
			}
		}
	}*/

	bool isFollowsS(string firstAtt, string secondAtt);
	bool isParentS(string firstAtt, string secondAtt);
};
#endif