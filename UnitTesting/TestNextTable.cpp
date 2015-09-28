#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextTableTest)
	{
	public:

		TEST_METHOD(TestNextTable)
		{
			// TODO: Your test code here
			NextTable nextTable;
			Assert::AreEqual(nextTable.addToTable("0", "1"), true);
			Assert::AreEqual(nextTable.addToTable("0", "1"), false);
			Assert::AreEqual(nextTable.addToTable("0", "2"), true);
			Assert::AreEqual(nextTable.addToTable("3", "1"), true);

			Assert::AreEqual(nextTable.getPrev("1").at(0), (string) "0");
			Assert::AreEqual(nextTable.getPrev("1").at(1), (string)"3");
			Assert::AreEqual(nextTable.getPrev("2").at(0), (string)"0");
			Assert::AreEqual(nextTable.getPrev("0").size(), (unsigned)0);

			Assert::AreEqual(nextTable.getNext("1").size(), (unsigned) 0);
			Assert::AreEqual(nextTable.getNext("0").at(0), (string) "1");
			Assert::AreEqual(nextTable.getNext("0").at(1), (string) "2");
			Assert::AreEqual(nextTable.getNext("3").at(0), (string) "1");

			Assert::AreEqual(nextTable.size(), 2);
			Assert::AreEqual(nextTable.isContained("0"), true);
			Assert::AreEqual(nextTable.isContained("3"), true);
			Assert::AreEqual(nextTable.isContained("1"), false);

			Assert::AreEqual(nextTable.isNext("0", "1"), true);
			Assert::AreEqual(nextTable.isNext("0", "2"), true);
			Assert::AreEqual(nextTable.isNext("3", "1"), true);
			Assert::AreEqual(nextTable.isNext("3", "0"), false);
			Assert::AreEqual(nextTable.isNext("0", "3"), false);
			Assert::AreEqual(nextTable.isNext("1", "2"), false);
			Assert::AreEqual(nextTable.isNext("2", "0"), false);
		}
	};
}