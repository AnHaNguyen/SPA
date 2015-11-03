#ifndef PKB_H
#define PKB_H	

#include "AST.h"
#include "FollowTable.h"
#include "ModifyTable.h"
#include "ParentTable.h"
#include "UseTable.h"
#include "DesignExtractor.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstTable.h"
#include "CallTable.h"
#include "NextTable.h"
#include "CFG.h"
#include "ProgLine.h"
#include "CallSTable.h"
#include "ParentSTable.h"
#include "FollowSTable.h"

class PKB {
private: static FollowTable* followTable;
		 static UseTable* useTable;
		 static ParentTable* parentTable;
		 static ModifyTable* modifyTable;
		 static vector<AST*> astList;
		 static VarTable* varTable;
		 static ProcTable* procTable;
		 static ConstTable* constTable;
		 static CallTable* callTable;
		 static NextTable* nextTable;
		 static vector<CFG*> cfgList;
		 static ProgLine* progLine;
		 static FollowSTable* followSTable;
		 static ParentSTable* parentSTable;
		 static CallSTable* callSTable;
public:
		 PKB();
		 ~PKB();
		 static FollowTable* getFollowTable();				//for Query
		 static void setFollowTable(FollowTable* fTable);			//for Extractor
		 static UseTable* getUseTable();					//for Query
		 static void setUseTable(UseTable* useTable);				//for Extractor
		 static ParentTable* getParentTable();				//for Query
		 static void setParentTable(ParentTable* parTable);			//for Extractor
		 static ModifyTable* getModifyTable();				//for Query
		 static void setModifyTable(ModifyTable* modTable);			//for Extractor
		 static vector<AST*> getASTList();					//for Query
		 static void setASTList(vector<AST*> ast);					//for Extractor
		 static VarTable* getVarTable();					//for Query
		 static void setVarTable(VarTable* varTable);				//for Extractor
		 static ProcTable* getProcTable();					//for Query
		 static void setProcTable(ProcTable* procTable);			//for Extractor
		 static ConstTable* getConstTable();				//for Query
		 static void setConstTable(ConstTable* constTable);			//for Extractor
		 static CallTable* getCallTable();					//for Query
		 static void setCallTable(CallTable* callTable);			//for Extractor
		 static NextTable* getNextTable();					//for Query
		 static void setNextTable(NextTable* nextTable);			//for Extractor
		 static vector<CFG*> getCFGList();					//for Query
		 static void setCFGList(vector<CFG*> cfgList);				//for Extractor
		 static ProgLine* getProgLine();					//for Query
		 static void setProgLine(ProgLine* progLine);				//for Extractor
		 static FollowSTable* getFollowSTable();					//for Query
		 static void setFollowSTable(FollowSTable* followSTable);				//for Extractor
		 static CallSTable* getCallSTable();					//for Query
		 static void setCallSTable(CallSTable* callSTable);				//for Extractor
		 static ParentSTable* getParentSTable();					//for Query
		 static void setParentSTable(ParentSTable* parentSTable);				//for Extractor


		 static vector<string> checkAssign(string pattern, bool contains_);	//to check pattern Assign
		 static vector<string> patternIf(string controlVar, bool containsQ);			
															//to check pattern If
		 static vector<string> patternWhile(string controlVar, bool containsQ);			
															//to check pattern While
		 static bool checkExistMod(string start, string end, string var,vector<string> processed);
		 static vector<pair<string, string>> patternIf();
		 static vector<pair<string, string>> patternWhile();
		 
		 //for Affect
		 static vector<string> affect(string a1, string a2);
		 static vector <pair<string,vector<string>>> affect();
};

#endif
