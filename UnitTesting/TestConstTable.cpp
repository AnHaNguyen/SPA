#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConstTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConstTableTest)
	{
	public:

		TEST_METHOD(TestConstTable)
		{
			// TODO: Your test code here
			ConstTable constTable;
			Assert::AreEqual(constTable.addToTable("0", "2"), true);
			Assert::AreEqual(constTable.addToTable("0", "2"), false);
			Assert::AreEqual(constTable.addToTable("0", "3"), true);
			Assert::AreEqual(constTable.addToTable("1", "2"), true);

			Assert::AreEqual(constTable.size(), 2);

			Assert::AreEqual(constTable.getConst("0").size(), (unsigned) 2);
			Assert::AreEqual(constTable.getConst("0").at(0), (string) "2");
			Assert::AreEqual(constTable.getConst("0").at(1), (string) "3");
			
			Assert::AreEqual(constTable.getLine("2").at(0), (string) "0");
			Assert::AreEqual(constTable.getLine("2").at(1), (string) "1");
			
			
			Assert::AreEqual(constTable.isConstOfLine("0", "2"), true);
			Assert::AreEqual(constTable.isConstOfLine("0", "3"), true);
			Assert::AreEqual(constTable.isConstOfLine("3", "0"), false);
			Assert::AreEqual(constTable.isConstOfLine("2", "0"), false);
			Assert::AreEqual(constTable.isConstOfLine("0", "0"), false);
			Assert::AreEqual(constTable.isConstOfLine("1", "2"), true);
		}
	};
}