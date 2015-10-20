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
		TEST_METHOD(TestGenerateFollowSTable) {
			FollowTable followTable;
			followTable.addToTable("0", "1");
			followTable.addToTable("1", "2");
			followTable.addToTable("2", "3");
			followTable.addToTable("5", "7");
			followTable.addToTable("8", "9");
			FollowSTable* fTable = followTable.generateFollowSTable();
			
			Assert::AreEqual(fTable->size(), 5);
			Assert::AreEqual(fTable->getTable().at(0).lineNo, (string)"0");
			Assert::AreEqual(fTable->getTable().at(0).followStmts.size(), (unsigned)3);
			Assert::AreEqual(fTable->getTable().at(0).followStmts.at(0), (string)"1");
			Assert::AreEqual(fTable->getTable().at(0).followStmts.at(1), (string)"2");
			Assert::AreEqual(fTable->getTable().at(0).followStmts.at(2), (string)"3");

			Assert::AreEqual(fTable->getTable().at(1).lineNo, (string)"1");
			Assert::AreEqual(fTable->getTable().at(1).followStmts.size(), (unsigned)2);
			Assert::AreEqual(fTable->getTable().at(1).followStmts.at(0), (string)"2");
			Assert::AreEqual(fTable->getTable().at(1).followStmts.at(1), (string)"3");
			
			Assert::AreEqual(fTable->getTable().at(2).lineNo, (string)"2");
			Assert::AreEqual(fTable->getTable().at(2).followStmts.size(), (unsigned)1);
			Assert::AreEqual(fTable->getTable().at(2).followStmts.at(0), (string)"3");
	
			Assert::AreEqual(fTable->getTable().at(3).lineNo, (string)"5");
			Assert::AreEqual(fTable->getTable().at(3).followStmts.size(), (unsigned)1);
			Assert::AreEqual(fTable->getTable().at(3).followStmts.at(0), (string)"7");

			Assert::AreEqual(fTable->getTable().at(4).lineNo, (string)"8");
			Assert::AreEqual(fTable->getTable().at(4).followStmts.size(), (unsigned)1);
			Assert::AreEqual(fTable->getTable().at(4).followStmts.at(0), (string)"9");
		}
	};
}