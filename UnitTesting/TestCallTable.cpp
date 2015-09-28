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
	};
}