#include <string>
#include <vector>
#include "Utility.h"

Utility::Utility() {
}

Utility::~Utility() {
}

vector<string> Utility::removeDuplicate(vector<string> origin) {
	vector<string> returnVector;
	for (unsigned i = 0; i < origin.size(); i++) {
		if (!isContained(returnVector, origin.at(i))) {
			returnVector.push_back(origin.at(i));
		}
	}
	return returnVector;
}

vector<int> Utility::removeDuplicate(vector<int> origin) {
	vector<int> returnVector;
	for (unsigned i = 0; i < origin.size(); i++) {
		if (!isContained(returnVector, origin.at(i))) {
			returnVector.push_back(origin.at(i));
		}
	}
	return returnVector;
}

bool Utility::isContained(vector<string> list, string element) {
	for (unsigned i = 0; i < list.size(); i++) {
		if (list.at(i) == element) {
			return true;
		}
	}
	return false;
}

bool Utility::isContained(vector<int> list, int element) {
	for (unsigned i = 0; i < list.size(); i++) {
		if (list.at(i) == element) {
			return true;
		}
	}
	return false;
}

void Utility::sort(vector<string> origin) {
	sort(origin);
}

void Utility::sort(vector<int> origin) {
	sort(origin);
}