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
		static void sort(vector<int> origin);
		static void sort(vector<string> origin);
		static bool isContained(vector<string> list, string element);
		static bool isContained(vector<int> list, int element);
};

#endif
