#include <string>
#include <vector>

using namespace std;
typedef struct stmt_t{
	string procedure;
	string type;
	stmt_t(string proc, string typ) {
		procedure = proc;
		type = typ;
	}
} stmt;
class StmtList {
private: vector<stmt_t> stmtList;		//list contains all stmt and its type (assign/if/while/call)
public: 
		StmtList();
		~StmtList();
		bool addToList(int lineNo, string type, string proc);		//add a stmt to List, return true if successful
		string getType(int lineNo);					//get Type of a stmt
		string getProcedure(int lineNo);			//get Procedure of a stmt
		int numOfLines();			//return number of lines of whole program
		vector<int> getLinesOfProc(string procedure);		//get all stmt of a proc
		vector<int> getLinesOfType(string type);	//get all stmt of a type
};
