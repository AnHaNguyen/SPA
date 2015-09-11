#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "ModifyTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(DesignExtractorTest)
	{
	public:
		/*method to test:
		processAST;  procecssModTable; processUseTable; processProcTable; 
		FollowTable* processFollowRelationship(AST* ast);
		ParentTable* processParentRelationship(AST* ast);
		*/
		vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
			"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;", 
			"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
			"procedureThird{", "z=5;", "v=z;}" };
		string x = "x";
		string y = "y";
		string z = "z";
		string i = "i";
		string v = "v";
		DesignExtractor ext = DesignExtractor(code);
		TEST_METHOD(TestExtModTable)
		{
			// TODO: Your test code here
			
			ModifyTable* modTable = ext.getModTable();
			Assert::AreEqual(modTable->size(),11);
			Assert::AreEqual(modTable->getTable().at(0).modifiedVar, x);
			Assert::AreEqual(modTable->getTable().at(1).modifiedVar, z);
			Assert::AreEqual(modTable->getTable().at(2).modifiedVar, x);
			Assert::AreEqual(modTable->getTable().at(3).lineNo, 4);
		}

		TEST_METHOD(TestExtUseTable)
		{
			UseTable* useTable = ext.getUseTable();
			Assert::AreEqual(useTable->size(), 7);
			Assert::AreEqual(useTable->getTable().at(0).usedVar.at(0), i);
			Assert::AreEqual(useTable->getTable().at(1).usedVar.at(0), x);
			Assert::AreEqual(useTable->getTable().at(1).usedVar.at(1), y);
			Assert::AreEqual(useTable->getTable().at(1).usedVar.size(), (unsigned) 2);
			Assert::AreEqual(useTable->getTable().at(2).usedVar.at(0), i);
			Assert::AreEqual(useTable->getTable().at(3).usedVar.at(0), z);
			Assert::AreEqual(useTable->getTable().at(3).usedVar.at(1), x);
			Assert::AreEqual(useTable->getTable().at(3).usedVar.at(2), i);
			Assert::AreEqual(useTable->getTable().at(3).usedVar.size(), (unsigned)3);
			Assert::AreEqual(useTable->getTable().at(6).lineNo, 12);
		}

		TEST_METHOD(TestExtProcTable)
		{
			ProcTable* procTable = ext.getProcTable();
			Assert::AreEqual(procTable->size(), 3);
			Assert::AreEqual(procTable->indexOf("First"), 0);
			Assert::AreEqual(procTable->indexOf("Second"), 1);
			Assert::AreEqual(procTable->indexOf("Third"), 2);
		}

		TEST_METHOD(TestExtVarTable) {
			VarTable* varTable = ext.getVarTable();
			Assert::AreEqual(varTable->size(), 5);
			Assert::AreEqual(varTable->indexOf(x), 0);
			Assert::AreEqual(varTable->indexOf(y), 3);
			Assert::AreEqual(varTable->indexOf(z), 1);
			Assert::AreEqual(varTable->indexOf(v), 4);
			Assert::AreEqual(varTable->indexOf(i), 2);
			Assert::AreEqual(varTable->indexOf("t"), -1);
		}

		TEST_METHOD(TestExtConstTable) {
			ConstTable* constTable = ext.getConstTable();
			Assert::AreEqual(constTable->size(), 8);
			Assert::AreEqual(constTable->getConst(1).at(0), (string) "2");
			Assert::AreEqual(constTable->getConst(1).size(), (unsigned) 1);
			Assert::AreEqual(constTable->getConst(2).at(0), (string) "3");
			Assert::AreEqual(constTable->getConst(9).at(0), (string) "2");
			Assert::AreEqual(constTable->getConst(11).at(0), (string) "5");
			Assert::AreEqual(constTable->getConst(13).size(), (unsigned) 0);
		}

		TEST_METHOD(TestExtBuildAST) {
			
			vector <string> code1 = { "procedureFirst{", "x=2;", "z=3;}", 
				"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
				"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
				"procedureThird{", "z=5;", "v=z;}" };
		

			DesignExtractor ex1 = DesignExtractor(code1);
			vector<AST*> astList = ex1.getASTList();
			Assert::AreEqual(astList.size(), (unsigned) 3);
			
			AST* proc1 = astList.at(0);
			Assert::AreEqual(proc1->getTree().size(), (unsigned)8);
			Assert::AreEqual(proc1->getTree().at(0)->getType(), (string) "procedure");
			Assert::AreEqual(proc1->getTree().at(0)->getChildList().at(0)->getType(), (string) "stmtLst");
			Assert::AreEqual(proc1->getTree().at(1)->getChildList().size(), (unsigned)2);
			Assert::AreEqual(proc1->getTree().at(0)->getValue(), (string) "First");
			Assert::AreEqual(proc1->getTree().at(1)->getType(), (string) "stmtLst");
			Assert::AreEqual(proc1->getTree().at(1)->getChildList().at(0)->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(2)->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(2)->getChildList().at(0)->getValue(), (string) "x");
			Assert::AreEqual(proc1->getTree().at(3)->getValue(), (string) "x");
			Assert::AreEqual(proc1->getTree().at(2)->getChildList().at(1)->getValue(), (string) "2");
			Assert::AreEqual(proc1->getTree().at(4)->getValue(), (string) "2");
			Assert::AreEqual(proc1->getTree().at(6)->getParent()->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(7)->getParent()->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(5)->getParent()->getType(), (string) "stmtLst");
			Assert::AreEqual(proc1->getTree().at(5)->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(6)->getValue(), (string) "z");
			Assert::AreEqual(proc1->getTree().at(7)->getValue(), (string) "3");

			AST* proc2 = astList.at(1);
			Assert::AreEqual(proc2->getTree().size(), (unsigned)42);
			Assert::AreEqual(proc2->getTree().at(0)->getType(), (string) "procedure");
			Assert::AreEqual(proc2->getTree().at(0)->getValue(), (string) "Second");
			Assert::AreEqual(proc2->getTree().at(2)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(3)->getValue(), (string) "x");
			Assert::AreEqual(proc2->getTree().at(4)->getValue(), (string) "0");
			Assert::AreEqual(proc2->getTree().at(5)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(6)->getValue(), (string) "i");
			Assert::AreEqual(proc2->getTree().at(7)->getValue(), (string) "5");
			Assert::AreEqual(proc2->getTree().at(8)->getType(), (string) "while");
			Assert::AreEqual(proc2->getTree().at(9)->getValue(), (string) "i");
			Assert::AreEqual(proc2->getTree().at(10)->getType(), (string) "stmtLst");
			Assert::AreEqual(proc2->getTree().at(11)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(12)->getValue(), (string) "x");
			Assert::AreEqual(proc2->getTree().at(13)->getValue(), (string) "x");
			Assert::AreEqual(proc2->getTree().at(14)->getValue(), (string) "2");
			Assert::AreEqual(proc2->getTree().at(15)->getType(), (string) "plus");
			Assert::AreEqual(proc2->getTree().at(16)->getValue(), (string) "y");
			Assert::AreEqual(proc2->getTree().at(17)->getType(), (string) "plus");
			Assert::AreEqual(proc2->getTree().at(18)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(23)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(30)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(35)->getType(), (string) "assign");

			Assert::AreEqual(proc2->getTree().at(1)->getChildList().size(), (unsigned)6);
			Assert::AreEqual(proc2->getTree().at(1)->getChildList().at(0)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(1)->getChildList().at(2)->getType(), (string) "while");
			Assert::AreEqual(proc2->getTree().at(1)->getType(), (string)"stmtLst");

			TNode* ass = proc2->getTree().at(1)->getChildList().at(3);
			Assert::AreEqual(ass->getChildList().at(0)->getValue(), (string) "z");
			Assert::AreEqual(ass->getChildList().at(1)->getType(), (string) "plus");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().size(), (unsigned) 2);
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(1)->getValue(), (string) "i");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getType(), (string) "plus");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getChildList().at(0)->getValue(), (string) "z");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getChildList().at(1)->getValue(), (string) "x");
			
			TNode* whi = proc2->getTree().at(1)->getChildList().at(2);
			Assert::AreEqual(whi->getChildList().at(0)->getValue(), (string) "i");
			
			Assert::AreEqual(whi->getChildList().at(1)->getType(), (string) "stmtLst");
			Assert::AreEqual(whi->getChildList().at(1)->getChildList().size(), (unsigned) 2);
			TNode* ass1 = whi->getChildList().at(1)->getChildList().at(0);
			TNode* ass2 = whi->getChildList().at(1)->getChildList().at(1);
			Assert::AreEqual(ass1->getChildList().at(0)->getValue(), (string) "x");
			Assert::AreEqual(ass1->getChildList().at(1)->getChildList().at(1)->getValue(), (string) "y");
			Assert::AreEqual(ass2->getChildList().at(1)->getChildList().at(1)->getType(), (string) "constant");
			Assert::AreEqual(ass2->getChildList().at(1)->getChildList().at(0)->getValue(), (string) "i");

			AST* proc3 = astList.at(2);
			Assert::AreEqual(proc3->getTree().size(), (unsigned)8);
			Assert::AreEqual(proc3->getTree().at(0)->getType(), (string) "procedure");
			Assert::AreEqual(proc3->getTree().at(0)->getChildList().at(0)->getType(), (string) "stmtLst");
			Assert::AreEqual(proc3->getTree().at(1)->getChildList().size(), (unsigned) 2);
			Assert::AreEqual(proc3->getTree().at(0)->getValue(), (string) "Third");
			Assert::AreEqual(proc3->getTree().at(1)->getChildList().at(0)->getType(), (string) "assign");
			Assert::AreEqual(proc3->getTree().at(1)->getChildList().at(1)->getType(), (string) "assign");
			Assert::AreEqual(proc3->getTree().at(2)->getType(), (string) "assign");
			Assert::AreEqual(proc3->getTree().at(2)->getChildList().at(0)->getValue(), (string) "z");
			Assert::AreEqual(proc3->getTree().at(2)->getChildList().at(1)->getValue(), (string) "5");
			Assert::AreEqual(proc3->getTree().at(3)->getValue(), (string) "z");
			Assert::AreEqual(proc3->getTree().at(4)->getValue(), (string) "5");
			Assert::AreEqual(proc3->getTree().at(5)->getType(), (string) "assign");
			Assert::AreEqual(proc3->getTree().at(5)->getChildList().at(0)->getValue(), (string) "v");
			Assert::AreEqual(proc3->getTree().at(5)->getChildList().at(1)->getValue(), (string) "z");
			Assert::AreEqual(proc3->getTree().at(6)->getValue(), (string) "v");
			Assert::AreEqual(proc3->getTree().at(7)->getValue(), (string) "z");
		}
	};
}