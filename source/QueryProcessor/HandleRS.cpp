#include "../QueryProcessor/HandleRS.h"
HandleRS::HandleRS() {}
HandleRS::~HandleRS() {}

string HandleRS::handleSelect(QueryTree * query, PreResultNode * &result) {	
	if (query->getResult() != NULL) {
		result = query->getResult();
		string rs = result->getResult().getSynonym();
		return HUtility().getSymMean(rs);
	}
	else return "";
}

//Check if getPrev(a) or getNext(a) has a = assign
bool HandleRS::folAss(string att, string firstAtt, vector<pair<string, string>> folTable, int i) {
	if (HUtility().getSymMean(att) == "assign" && att == firstAtt) {
		if (HUtility().contain(HUtility().getAssignTable(), folTable[i].first)) {
			return true;
		}
	}
	if (HUtility().getSymMean(att) == "assign" && att != firstAtt) {
		if (HUtility().contain(HUtility().getAssignTable(), folTable[i].second)) {
			return true;
		}
	}
	return false;
}