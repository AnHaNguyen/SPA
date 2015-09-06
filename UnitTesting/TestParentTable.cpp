#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParentTableTest)
	{
	public:

		TEST_METHOD(TestParentTable)
		{
			// TODO: Your test code here
			ParentTable parTable;
			Assert::AreEqual(parTable.addToTable(0, 1), true);
			Assert::AreEqual(parTable.addToTable(0, 1), false);
			Assert::AreEqual(parTable.addToTable(0, 2), true);

			Assert::AreEqual(parTable.getChild(0).at(0), 1);
			Assert::AreEqual(parTable.getChild(0).at(1), 2);
			Assert::AreEqual(parTable.getChild(1).size(), (unsigned) 0);
			
			Assert::AreEqual(parTable.getParent(1), 0);
			Assert::AreEqual(parTable.getParent(2), 0);
			Assert::AreEqual(parTable.getParent(0), -1);

			Assert::AreEqual(parTable.size(), 1);
			Assert::AreEqual(parTable.isContained(0), true);
			Assert::AreEqual(parTable.isContained(1), false);

		}
	};
}