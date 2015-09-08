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
		processAST;  procecssModTable; processUseTable; processVarTable; processProcTable; 
		FollowTable* processFollowRelationship(AST* ast);
		ParentTable* processParentRelationship(AST* ast);
		*/
		vector <string> code = { "procedureFirst{", "x=2;", "z=3;", "callSecond;}",
			"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;", "callThird;" ,
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
			Assert::AreEqual(modTable->getTable().at(3).lineNo, 6);
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
			Assert::AreEqual(useTable->getTable().at(6).lineNo, 16);
		}

		TEST_METHOD(TestExtProcTable)
		{
			ProcTable* procTable = ext.getProcTable();
			Assert::AreEqual(procTable->size(), 3);
			Assert::AreEqual(procTable->indexOf("First"), 0);
			Assert::AreEqual(procTable->indexOf("Second"), 1);
			Assert::AreEqual(procTable->indexOf("Third"), 2);
		}
	};
}