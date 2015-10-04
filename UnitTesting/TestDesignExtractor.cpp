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

		TEST_METHOD(TestExtCallTable) 
		{
			vector<string>code2 = { "procedureFirst{", "callSecond;", "callThird;","callFourth;}",
			"procedureSecond{", "y=y+3+x;", "callThird;}",
			"procedureThird{", "callFourth;}",
			"procedureFourth{", "y=1;}"};

			DesignExtractor ext2 = DesignExtractor(code2);
			CallTable* callTable = ext2.getCallTable();

			Assert::AreEqual(callTable->size(), 3);
			Assert::AreEqual(callTable->getCallees("First").at(0), (string) "Second");
			Assert::AreEqual(callTable->getCallees("First").at(1), (string) "Third");
			Assert::AreEqual(callTable->getCallees("First").at(2), (string) "Fourth");
			Assert::AreEqual(callTable->getCallees("Second").at(0), (string) "Third");

			Assert::AreEqual(callTable->getCallers("Third").at(0), (string) "First");
			Assert::AreEqual(callTable->getCallers("Third").at(1), (string) "Second");
			Assert::AreEqual(callTable->getCallers("Fourth").at(0), (string) "First");
			Assert::AreEqual(callTable->getCallers("Second").at(0), (string) "First");
		}

		TEST_METHOD(TestExtModUseTable)
		{
			vector <string> code = { "procedureFirst{","iftthen{", "k=2+u;}", "else{", "c=2+o;}}",
				"procedureSecond{", "x=0;", "whilei{" ,"y=x+2;", "x=3+m;", "callFirst;}" ,"x=y+z;}",
				"procedureThird{", "z=5;", "callSecond;", "callFourth;", "callFifth;}",
				"procedureFourth{", "e=e+1;", "f=1+w;}",
				"procedureFifth{", "r=r+5;}" };

			DesignExtractor ext = DesignExtractor(code);
			ModifyTable* modTable = ext.getModTable();
			UseTable* useTable = ext.getUseTable();
			
			Assert::AreEqual(modTable->isModified("First", "k"), true);
			Assert::AreEqual(modTable->isModified("First", "c"), true);
			Assert::AreEqual(modTable->isModified("First", "t"), false);

			// Test for if-the-else stmt
			Assert::AreEqual(modTable->isModified("1", "t"), false);
			Assert::AreEqual(modTable->isModified("1", "k"), true);
			Assert::AreEqual(modTable->isModified("1", "c"), true);
			Assert::AreEqual(modTable->isModified("2", "k"), true);
			Assert::AreEqual(modTable->isModified("3", "c"), true);

			Assert::AreEqual(modTable->isModified("Second", "k"), true);
			Assert::AreEqual(modTable->isModified("Second", "c"), true);
			Assert::AreEqual(modTable->isModified("Second", "i"), false);
			Assert::AreEqual(modTable->isModified("Second", "x"), true);
			Assert::AreEqual(modTable->isModified("Second", "y"), true);

			// test for while stmt
			Assert::AreEqual(modTable->isModified("5", "i"), false);
			Assert::AreEqual(modTable->isModified("5", "y"), true);
			Assert::AreEqual(modTable->isModified("5", "x"), true);
			// call in procedure Second
			Assert::AreEqual(modTable->isModified("8", "k"), true);
			Assert::AreEqual(modTable->isModified("8", "c"), true);
			// container contains call in procedure Second
			Assert::AreEqual(modTable->isModified("5", "k"), true);
			Assert::AreEqual(modTable->isModified("5", "c"), true);


			// Test for useTable
			Assert::AreEqual(useTable->isUsed("1", "t"), true);
			Assert::AreEqual(useTable->isUsed("First", "u"), true);
			Assert::AreEqual(useTable->isUsed("First", "o"), true);

			Assert::AreEqual(useTable->isUsed("Second", "i"), true);
			Assert::AreEqual(useTable->isUsed("Second", "x"), true);
			Assert::AreEqual(useTable->isUsed("Second", "m"), true);
			Assert::AreEqual(useTable->isUsed("Second", "y"), true);
			Assert::AreEqual(useTable->isUsed("Second", "z"), true);
			Assert::AreEqual(useTable->isUsed("Second", "t"), true);
			Assert::AreEqual(useTable->isUsed("Second", "u"), true);
			Assert::AreEqual(useTable->isUsed("Second", "o"), true);

			// Test for while loop with call 
			Assert::AreEqual(useTable->isUsed("5", "t"), true);
			Assert::AreEqual(useTable->isUsed("5", "u"), true);
			Assert::AreEqual(useTable->isUsed("5", "o"), true);

			Assert::AreEqual(useTable->isUsed("8", "t"), true);
			Assert::AreEqual(useTable->isUsed("8", "u"), true);
			Assert::AreEqual(useTable->isUsed("8", "o"), true);

			Assert::AreEqual(useTable->isUsed("Second", "t"), true);
			Assert::AreEqual(useTable->isUsed("Second", "u"), true);
			Assert::AreEqual(useTable->isUsed("Second", "o"), true);

			// Test for procedure Third
			Assert::AreEqual(useTable->isUsed("Third", "i"), true);
			Assert::AreEqual(useTable->isUsed("Third", "x"), true);
			Assert::AreEqual(useTable->isUsed("Third", "m"), true);
			Assert::AreEqual(useTable->isUsed("Third", "y"), true);
			Assert::AreEqual(useTable->isUsed("Third", "z"), true);

			// Procedure First
			Assert::AreEqual(useTable->isUsed("Third", "t"), true);
			Assert::AreEqual(useTable->isUsed("Third", "u"), true);
			Assert::AreEqual(useTable->isUsed("Third", "o"), true);

			
			Assert::AreEqual(useTable->isUsed("Third", "e"), true);
			Assert::AreEqual(useTable->isUsed("Third", "w"), true);
			Assert::AreEqual(useTable->isUsed("Third", "r"), true);
		}

		TEST_METHOD(TestExtProcTable)
		{
			vector <string> code = { "procedureFirst{", "x=2;}",
				"procedureSecond{", "y=2;}",
				"procedureThird{", "z=2;}" };
			DesignExtractor ext = DesignExtractor(code);

			ProcTable* procTable = ext.getProcTable();
			Assert::AreEqual(procTable->size(), 3);
			Assert::AreEqual(procTable->indexOf("First"), 0);
			Assert::AreEqual(procTable->indexOf("Second"), 1);
			Assert::AreEqual(procTable->indexOf("Third"), 2);
		}

		TEST_METHOD(TestExtVarTable) {
			vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
				"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
				"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
				"procedureThird{", "z=5;", "v=z;}" };
			DesignExtractor ext = DesignExtractor(code);

			VarTable* varTable = ext.getVarTable();
			Assert::AreEqual(varTable->size(), 5);
			Assert::AreEqual(varTable->indexOf("x"), 0);
			Assert::AreEqual(varTable->indexOf("y"), 3);
			Assert::AreEqual(varTable->indexOf("z"), 1);
			Assert::AreEqual(varTable->indexOf("v"), 4);
			Assert::AreEqual(varTable->indexOf("i"), 2);
			Assert::AreEqual(varTable->indexOf("t"), -1);
		}

		TEST_METHOD(TestExtConstTable) {
			vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
				"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
				"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
				"procedureThird{", "z=5;", "v=z;}" };
			DesignExtractor ext = DesignExtractor(code);

			ConstTable* constTable = ext.getConstTable();
			Assert::AreEqual(constTable->size(), 8);
			Assert::AreEqual(constTable->getConst("1").at(0), (string) "2");
			Assert::AreEqual(constTable->getConst("1").size(), (unsigned) 1);
			Assert::AreEqual(constTable->getConst("2").at(0), (string) "3");
			Assert::AreEqual(constTable->getConst("9").at(0), (string) "2");
			Assert::AreEqual(constTable->getConst("11").at(0), (string) "5");
			Assert::AreEqual(constTable->getConst("13").size(), (unsigned) 0);
		}

		TEST_METHOD(TestExtBuildAST) {
			vector <string> code1 = { "procedureFirst{", "x=2;","ifxthen{", "z=3+z;}",
				"else{", "z=1;}}",
				"procedureSecond{", "i=5;", "callThird" , "whilei{" ,"x=x+2+y;}" ,"z=z+2+i;}",
				"procedureThird{", "z=5;", "v=z;}" };

			DesignExtractor ex1 = DesignExtractor(code1);
			vector<AST*> astList = ex1.getASTList();
			// check number of procedures
			Assert::AreEqual(astList.size(), (unsigned) 3);
			
			AST* proc1 = astList.at(0);
			Assert::AreEqual(proc1->getTree().size(), (unsigned)17);
			// test for 1st node: First:procedure
			Assert::AreEqual(proc1->getTree().at(0)->getType(), (string) "procedure");
			Assert::AreEqual(proc1->getTree().at(0)->getValue(), (string)"First");
			// test for 2nd node: stmtLst
			Assert::AreEqual(proc1->getTree().at(0)->getChildList().at(0)->getType(), STMTLST);
			Assert::AreEqual(proc1->getTree().at(1)->getType(), STMTLST);
			Assert::AreEqual(proc1->getTree().at(1)->getChildList().at(1)->getType(), IF);
			Assert::AreEqual(proc1->getTree().at(1)->getChildList().size(), (unsigned)2);
			// test for 3rd node: assign
			Assert::AreEqual(proc1->getTree().at(1)->getChildList().at(0)->getType(), ASSIGN);
			Assert::AreEqual(proc1->getTree().at(2)->getType(), (string) "assign");
			Assert::AreEqual(proc1->getTree().at(2)->getChildList().at(0)->getValue(), (string) "x");
			Assert::AreEqual(proc1->getTree().at(3)->getValue(), (string) "x");
			// test for 4th node: 2:constant
			Assert::AreEqual(proc1->getTree().at(2)->getChildList().at(1)->getValue(), (string) "2");
			Assert::AreEqual(proc1->getTree().at(4)->getValue(), (string) "2");
			// test for 5th node: if
			Assert::AreEqual(proc1->getTree().at(5)->getType(), IF);
			Assert::AreEqual(proc1->getTree().at(5)->getChildList().at(0)->getType(), VARIABLE);
			Assert::AreEqual(proc1->getTree().at(6)->getType(), VARIABLE);
			// test for 7th node: then stmtlst
			Assert::AreEqual(proc1->getTree().at(7)->getType(), STMTLST);
			Assert::AreEqual(proc1->getTree().at(7)->getValue(), THEN);
			// test for 8th, 9th node: assign
			Assert::AreEqual(proc1->getTree().at(8)->getType(), ASSIGN);
			Assert::AreEqual(proc1->getTree().at(8)->getChildList().at(0)->getType(), VARIABLE);
			Assert::AreEqual(proc1->getTree().at(9)->getType(), VARIABLE);
			Assert::AreEqual(proc1->getTree().at(9)->getValue(), (string) "z");
			//test for 12th node: plus
			Assert::AreEqual(proc1->getTree().at(8)->getChildList().at(1)->getType(), PLUS_TEXT);
			Assert::AreEqual(proc1->getTree().at(12)->getType(), PLUS_TEXT);
			// test for 10th: variable z
			Assert::AreEqual(proc1->getTree().at(10)->getValue(), (string) "3");
			Assert::AreEqual(proc1->getTree().at(12)->getChildList().at(0)->getValue(), (string) "3");
			// test for 11th: const 3
			Assert::AreEqual(proc1->getTree().at(11)->getValue(), (string) "z");
			Assert::AreEqual(proc1->getTree().at(12)->getChildList().at(1)->getValue(), (string) "z");
			// test for 13th node: else:stmtlst
			Assert::AreEqual(proc1->getTree().at(13)->getValue(), ELSE);
			Assert::AreEqual(proc1->getTree().at(13)->getType(), STMTLST);
			Assert::AreEqual(proc1->getTree().at(5)->getChildList().at(2)->getValue(), ELSE);
			// test for 14th node: assign
			Assert::AreEqual(proc1->getTree().at(13)->getChildList().at(0)->getType(), ASSIGN);
			Assert::AreEqual(proc1->getTree().at(14)->getType(), ASSIGN);
			//test for 15th node: z: variable
			Assert::AreEqual(proc1->getTree().at(15)->getValue(), (string) "z");
			Assert::AreEqual(proc1->getTree().at(15)->getType(), VARIABLE);
			// test for 16th node: 1: constant
			Assert::AreEqual(proc1->getTree().at(14)->getChildList().at(1)->getValue(), (string) "1");
			Assert::AreEqual(proc1->getTree().at(14)->getChildList().at(1)->getType(), CONSTANT);

			AST* proc2 = astList.at(1);
			
			Assert::AreEqual(proc2->getTree().size(), (unsigned)23);
			// test for procedure Second
			Assert::AreEqual(proc2->getTree().at(0)->getType(), (string) "procedure");
			Assert::AreEqual(proc2->getTree().at(0)->getValue(), (string) "Second");
			Assert::AreEqual(proc2->getTree().at(2)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(3)->getValue(), (string) "i");
			Assert::AreEqual(proc2->getTree().at(4)->getValue(), (string) "5");
			// test for 5th Node: Third: Call
			Assert::AreEqual(proc2->getTree().at(5)->getType(), CALL);
			Assert::AreEqual(proc2->getTree().at(5)->getValue(), (string) "Third");

			Assert::AreEqual(proc2->getTree().at(6)->getType(), WHILE);
			Assert::AreEqual(proc2->getTree().at(7)->getValue(), (string) "i");
			Assert::AreEqual(proc2->getTree().at(8)->getType(), STMTLST);

			Assert::AreEqual(proc2->getTree().at(9)->getType(), ASSIGN);
			Assert::AreEqual(proc2->getTree().at(10)->getValue(), (string) "x");
			Assert::AreEqual(proc2->getTree().at(10)->getType(), VARIABLE);
			Assert::AreEqual(proc2->getTree().at(12)->getValue(), (string) "2");
			Assert::AreEqual(proc2->getTree().at(13)->getType(), PLUS_TEXT);
			Assert::AreEqual(proc2->getTree().at(14)->getValue(), (string) "y");
			Assert::AreEqual(proc2->getTree().at(15)->getType(), PLUS_TEXT);
			Assert::AreEqual(proc2->getTree().at(16)->getType(), ASSIGN);
			Assert::AreEqual(proc2->getTree().at(17)->getValue(), (string) "z");
			Assert::AreEqual(proc2->getTree().at(18)->getValue(), (string) "z");
			Assert::AreEqual(proc2->getTree().at(19)->getType(), CONSTANT);

			Assert::AreEqual(proc2->getTree().at(1)->getChildList().size(), (unsigned)4);
			Assert::AreEqual(proc2->getTree().at(1)->getChildList().at(0)->getType(), (string) "assign");
			Assert::AreEqual(proc2->getTree().at(1)->getChildList().at(2)->getType(), (string) "while");
			Assert::AreEqual(proc2->getTree().at(1)->getChildList().at(1)->getType(), CALL);

			TNode* ass = proc2->getTree().at(1)->getChildList().at(3);
			Assert::AreEqual(ass->getChildList().at(0)->getValue(), (string) "z");
			Assert::AreEqual(ass->getChildList().at(1)->getType(), (string) "plus");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().size(), (unsigned) 2);
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(1)->getValue(), (string) "i");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getType(), (string) "plus");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getChildList().at(0)->getValue(), (string) "z");
			Assert::AreEqual(ass->getChildList().at(1)->getChildList().at(0)->getChildList().at(1)->getValue(), (string) "2");
			
			TNode* whi = proc2->getTree().at(1)->getChildList().at(2);
			Assert::AreEqual(whi->getChildList().at(0)->getValue(), (string) "i");
			
			Assert::AreEqual(whi->getChildList().at(1)->getType(), (string) "stmtLst");
			Assert::AreEqual(whi->getChildList().at(1)->getChildList().size(), (unsigned) 1);
			TNode* ass1 = whi->getChildList().at(1)->getChildList().at(0);
			Assert::AreEqual(ass1->getChildList().at(0)->getValue(), (string) "x");
			Assert::AreEqual(ass1->getChildList().at(1)->getChildList().at(1)->getValue(), (string) "y");

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

		TEST_METHOD(TestExtParentTable) {
			vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
				"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
				"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
				"procedureThird{", "z=5;", "v=z;}" };
			DesignExtractor ext = DesignExtractor(code);

			ParentTable* parentTable = ext.getParentTable();

			vector<string> childrenOfWhile = parentTable->getChild("5");
			Assert::AreEqual(childrenOfWhile.size(), (unsigned) 2);
			Assert::AreEqual(childrenOfWhile.at(0), (string) "6");
			Assert::AreEqual(childrenOfWhile.at(1), (string) "7");
		}

		TEST_METHOD(TestExtFollowTable) {
			vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
				"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
				"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
				"procedureThird{", "z=5;", "v=z;}" };
			DesignExtractor ext = DesignExtractor(code);

			FollowTable* followTable = ext.getFollowTable();

			Assert::AreEqual(followTable->size(), 8);

			Assert::AreEqual(followTable->getNext("1"), (string) "2");
			Assert::AreEqual(followTable->getPrev("2"), (string) "1");
			Assert::AreEqual(followTable->getNext("2"), (string) "");

			Assert::AreEqual(followTable->getNext("3"), (string) "4");
			Assert::AreEqual(followTable->getPrev("4"), (string) "3");
			Assert::AreEqual(followTable->getNext("4"), (string) "5");
			Assert::AreEqual(followTable->getPrev("5"), (string) "4");

			Assert::AreNotEqual(followTable->getNext("5"), (string) "6");
			Assert::AreNotEqual(followTable->getPrev("6"), (string) "5");
			Assert::AreEqual(followTable->getNext("6"), (string) "7");
			Assert::AreEqual(followTable->getPrev("7"), (string) "6");

			Assert::AreEqual(followTable->getNext("5"), (string) "8");
			Assert::AreEqual(followTable->getPrev("8"), (string) "5");

			Assert::AreNotEqual(followTable->getNext("10"), (string) "11");
			Assert::AreNotEqual(followTable->getPrev("11"), (string) "10");
			Assert::AreEqual(followTable->getNext("11"), (string) "12");
		}
	};
}
