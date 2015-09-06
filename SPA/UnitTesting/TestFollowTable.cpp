#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowTableTest)
	{
	public:

		TEST_METHOD(TestFollowTable)
		{
			// TODO: Your test code here
			FollowTable followTable;
			Assert::AreEqual(followTable.addToTable(0, 1), true);
			Assert::AreEqual(followTable.addToTable(0, 1), false);
			Assert::AreEqual(followTable.addToTable(0, 2), false);
			Assert::AreEqual(followTable.addToTable(1, 2), true);

			Assert::AreEqual(followTable.getPrev(0), -1);
			Assert::AreEqual(followTable.getPrev(1), 0);
			Assert::AreEqual(followTable.getPrev(2), 1);

			Assert::AreEqual(followTable.getNext(0), 1);
			Assert::AreEqual(followTable.getNext(1), 2);
			Assert::AreEqual(followTable.getNext(2), -1);

			Assert::AreEqual(followTable.size(), 2);
			Assert::AreEqual(followTable.isContained(0), true);
			Assert::AreEqual(followTable.isContained(1), true);
			Assert::AreEqual(followTable.isContained(2), false);

		}
	};
}