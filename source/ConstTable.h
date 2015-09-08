#include <vector>
#include <string>

using namespace std;
typedef struct ConstEntry_t {	
	int line;
	string constant;
	ConstEntry_t(int line, string constant) {
		ConstEntry_t::line = line;
		ConstEntry_t::constant = constant;
	}
}ConstEntry;

class ConstTable {
private: vector<ConstEntry_t> constTable;
public: ConstTable();
		~ConstTable();
		bool addToTable(int line, string constant);
		vector<string> getConst(int line);			//return the constants used in a line (for first iter
													//output.size() <= 1)
		vector<int> getLine(string constant);		//return all lines use this const
		bool isContained(string constant);		//check if a constant is in table
		bool isContained(int line);				//check if a line is in table
		bool isConstOfLine(int line, string constant);	//check if a line uses a constant
		int size();
		vector<ConstEntry_t> getTable();			//return whole table
};