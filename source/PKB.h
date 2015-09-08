#include "AST.h"
#include "FollowTable.h"
#include "ModifyTable.h"
#include "ParentTable.h"
#include "UseTable.h"
#include "DesignExtractor.h"
#include "VarTable.h"
#include "ProcTable.h"

class PKB {
private: static FollowTable* followTable;
		 static UseTable* useTable;
		 static ParentTable* parentTable;
		 static ModifyTable* modifyTable;
		 static vector<AST*> astList;
		 static VarTable* varTable;
		 static ProcTable* procTable;

public:
		 PKB();
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
};
