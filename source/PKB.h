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
		 static AST* ast;
		 static VarTable* varTable;
		 static ProcTable* procTable;

public:
		 PKB();
		 static FollowTable* getFollowTable();
		 static void setFollowTable(FollowTable* fTable);
		 static UseTable* getUseTable();
		 static void setUseTable(UseTable* useTable);
		 static ParentTable* getParentTable();
		 static void setParentTable(ParentTable* parTable);
		 static ModifyTable* getModifyTable();
		 static void setModifyTable(ModifyTable* modTable);
		 static AST* getAST();
		 static void setAST(AST* ast);
		 static VarTable* getVarTable();
		 static void setVarTable(VarTable* varTable);
		 static ProcTable* getProcTable();
		 static void setProcTable(ProcTable* procTable);
};
