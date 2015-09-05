#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifyTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyTableTest)
	{
	public:

		TEST_METHOD(TestModTable)
		{
			// TODO: Your test code here
			ModifyTable modTable;
			Assert::AreEqual(modTable.add(0, "x"), true);
			Assert::AreEqual(modTable.add(0, "x"), false);
			Assert::AreEqual(modTable.add(0, "y"), true);
		
			Assert::AreEqual(modTable.getModifier("x").at(0), 0);
			string x = "x";
			string y = "y";
			Assert::AreEqual(modTable.getModified(0).at(0),x);
			Assert::AreEqual(modTable.getModified(0).at(1), y);
			Assert::AreEqual(modTable.size(), 1);
			Assert::AreEqual(modTable.isContained(0), true);
			Assert::AreEqual(modTable.isContained(1), false);

		}
	};
}