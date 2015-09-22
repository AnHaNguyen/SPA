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
			Assert::AreEqual(followTable.addToTable("0", "1"), true);
			Assert::AreEqual(followTable.addToTable("0", "1"), false);
			Assert::AreEqual(followTable.addToTable("0", "2"), false);
			Assert::AreEqual(followTable.addToTable("1", "2"), true);

			Assert::AreEqual(followTable.getPrev("0"), (string) "");
			Assert::AreEqual(followTable.getPrev("1"), (string)"0");
			Assert::AreEqual(followTable.getPrev("2"), (string)"1");

			Assert::AreEqual(followTable.getNext("0"), (string) "1");
			Assert::AreEqual(followTable.getNext("1"), (string) "2");
			Assert::AreEqual(followTable.getNext("2"), (string)"");

			Assert::AreEqual(followTable.size(), 2);
			Assert::AreEqual(followTable.isContained("0"), true);
			Assert::AreEqual(followTable.isContained("1"), true);
			Assert::AreEqual(followTable.isContained("2"), false);

			Assert::AreEqual(followTable.isFollows("0", "1"), true);
			Assert::AreEqual(followTable.isFollows("1", "2"), true);
			Assert::AreEqual(followTable.isFollows("0", "2"), false);
			Assert::AreEqual(followTable.isFollows("2", "3"), false);
		}
	};
}