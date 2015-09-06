#include "stdafx.h"
#include "CppUnitTest.h"
#include "UseTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UseTableTest)
	{
	public:

		TEST_METHOD(TestUseTable)
		{
			// TODO: Your test code here
			UseTable useTable;
			Assert::AreEqual(useTable.add(0, "x"), true);
			Assert::AreEqual(useTable.add(0, "x"), false);
			Assert::AreEqual(useTable.add(0, "y"), true);

			Assert::AreEqual(useTable.getUser("x").at(0), 0);
			string x = "x";
			string y = "y";
			Assert::AreEqual(useTable.getUsed(0).at(0), x);
			Assert::AreEqual(useTable.getUsed(0).at(1), y);
			Assert::AreEqual(useTable.size(), 1);
			Assert::AreEqual(useTable.isContained(0), true);
			Assert::AreEqual(useTable.isContained(1), false);

			Assert::AreEqual(useTable.isUsed(0, "x"), true);
			Assert::AreEqual(useTable.isUsed(0, "z"), false);
			Assert::AreEqual(useTable.isUsed(1, "x"), false);
		}
	};
}