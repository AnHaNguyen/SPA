#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

using namespace std;

class Utility {
public: Utility();
		~Utility();
		static vector<string> removeDuplicate(vector<string> origin);
		static vector<int> removeDuplicate(vector<int> origin);
};

#endif
