#include <string>
#include <vector>
#include "Utility.h"

Utility::Utility() {
}

Utility::~Utility() {
}

vector<string> Utility::removeDuplicate(vector<string> origin) {
	vector<string> returnVector;
	for (unsigned i = 0; i < origin.size() - 1; i++) {
		unsigned j = i + 1;
		for (; j < origin.size(); j++) {
			if (origin.at(i) == origin.at(j)) {
				break;
			}
		}
		if (j == origin.size()) {
			returnVector.push_back(origin.at(i));
		}
	}
	return returnVector;
}

vector<int> Utility::removeDuplicate(vector<int> origin) {
	vector<int> returnVector;
	for (unsigned i = 0; i < origin.size() - 1; i++) {
		unsigned j = i + 1;
		for (; j < origin.size(); j++) {
			if (origin.at(i) == origin.at(j)) {
				break;
			}
		}
		if (j == origin.size()) {
			returnVector.push_back(origin.at(i));
		}
	}
	return returnVector;
}