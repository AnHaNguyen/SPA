#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(CallTableTest)
	{
	public:

		TEST_METHOD(TestCallTable)
		{
			// TODO: Your test code here
			CallTable callTable;
			Assert::AreEqual(callTable.addToTable("First", "Second"), true);
			Assert::AreEqual(callTable.addToTable("First", "Third"), true);
			Assert::AreEqual(callTable.addToTable("First", "Second"), false);
			Assert::AreEqual(callTable.addToTable("Second", "Third"), true);

			Assert::AreEqual(callTable.getCallees("First").size(), (unsigned) 2);
			Assert::AreEqual(callTable.getCallees("First").at(0), (string) "Second");
			Assert::AreEqual(callTable.getCallees("First").at(1), (string) "Third");
			
			Assert::AreEqual(callTable.getCallers("Second").at(0),(string) "First");
			Assert::AreEqual(callTable.getCallers("Third").at(0), (string) "First");
			Assert::AreEqual(callTable.getCallers("Third").at(1), (string) "Second");
			
			Assert::AreEqual(callTable.size(), 2);
			
			Assert::AreEqual(callTable.isCall("First", "Second"), true);
			Assert::AreEqual(callTable.isCall("Second", "Third"), true);
			Assert::AreEqual(callTable.isCall("Third", "First"), false);
			Assert::AreEqual(callTable.isCall("First", "Third"), true);
		}

		TEST_METHOD(TestCallTableRecur) {
			CallTable callTable;
			
			Assert::AreEqual(callTable.addToTable("First", "Second"), true);
			Assert::AreEqual(callTable.addToTable("First", "Third"), true);
			Assert::AreEqual(callTable.addToTable("Second", "Third"), true);
			Assert::AreEqual(callTable.addToTable("Second", "Fourth"), true);
			Assert::AreEqual(callTable.addToTable("Second", "Fifth"), true);
			

			//Assert::AreEqual(callTable.size(), 3);
			Assert::AreEqual(callTable.isContainedRecur(),false);
			Assert::AreEqual(callTable.addToTable("Fifth", "Second"), true);
			Assert::AreEqual(callTable.isContainedRecur(), true);
		
			CallTable callTable2;
			callTable2.addToTable("First", "First");
			Assert::AreEqual(callTable2.isContainedRecur(), true);
		}
		/*TEST_METHOD(TestGenerateCallSTable) {
			CallTable callTable;
			callTable.addToTable("0", "1");
			callTable.addToTable("0", "2");
			callTable.addToTable("1", "4");
			callTable.addToTable("2", "5");
			callTable.addToTable("0", "3");
			callTable.addToTable("5", "7");
			callTable.addToTable("8", "9");
			callTable.addToTable("9", "10");
			callTable.addToTable("11", "12");
			CallSTable* cTable = callTable.generateCallSTable();

			Assert::AreEqual(cTable->size(), 7);
			Assert::AreEqual(cTable->getTable().at(0).caller, (string)"0");
			Assert::AreEqual(cTable->getTable().at(0).callees.size(), (unsigned)6);
			Assert::AreEqual(cTable->getTable().at(0).callees.at(0), (string)"1");
			Assert::AreEqual(cTable->getTable().at(0).callees.at(1), (string)"2");
			Assert::AreEqual(cTable->getTable().at(0).callees.at(2), (string)"3");
			Assert::AreEqual(cTable->getTable().at(0).callees.at(3), (string)"4");
			Assert::AreEqual(cTable->getTable().at(0).callees.at(4), (string)"5");
			Assert::AreEqual(cTable->getTable().at(0).callees.at(5), (string)"7");

			Assert::AreEqual(cTable->getTable().at(1).caller, (string)"1");
			Assert::AreEqual(cTable->getTable().at(1).callees.size(), (unsigned)1);
			Assert::AreEqual(cTable->getTable().at(1).callees.at(0), (string)"4");

			Assert::AreEqual(cTable->getTable().at(2).caller, (string)"2");
			Assert::AreEqual(cTable->getTable().at(2).callees.size(), (unsigned)2);
			Assert::AreEqual(cTable->getTable().at(2).callees.at(0), (string)"5");
			Assert::AreEqual(cTable->getTable().at(2).callees.at(1), (string)"7");

			Assert::AreEqual(cTable->getTable().at(3).caller, (string)"5");
			Assert::AreEqual(cTable->getTable().at(3).callees.size(), (unsigned)1);
			Assert::AreEqual(cTable->getTable().at(3).callees.at(0), (string)"7");

			Assert::AreEqual(cTable->getTable().at(4).caller, (string)"8");
			Assert::AreEqual(cTable->getTable().at(4).callees.size(), (unsigned)2);
			Assert::AreEqual(cTable->getTable().at(4).callees.at(0), (string)"9");
			Assert::AreEqual(cTable->getTable().at(4).callees.at(1), (string)"10");

			Assert::AreEqual(cTable->getTable().at(5).caller, (string)"9");
			Assert::AreEqual(cTable->getTable().at(5).callees.size(), (unsigned)1);
			Assert::AreEqual(cTable->getTable().at(5).callees.at(0), (string)"10");

			Assert::AreEqual(cTable->getTable().at(6).caller, (string)"11");
			Assert::AreEqual(cTable->getTable().at(6).callees.size(), (unsigned)1);
			Assert::AreEqual(cTable->getTable().at(6).callees.at(0), (string)"12");
		}*/
	};
}