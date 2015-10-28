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
			Assert::AreEqual(parTable.addToTable("0", "1"), true);
			Assert::AreEqual(parTable.addToTable("0", "1"), false);
			Assert::AreEqual(parTable.addToTable("0", "2"), true);

			Assert::AreEqual(parTable.getChild("0").at(0), (string) "1");
			Assert::AreEqual(parTable.getChild("0").at(1), (string)"2");
			Assert::AreEqual(parTable.getChild("1").size(), (unsigned)0);

			Assert::AreEqual(parTable.getParent("1"), (string) "0");
			Assert::AreEqual(parTable.getParent("2"), (string) "0");
			Assert::AreEqual(parTable.getParent("0"), (string) "");

			Assert::AreEqual(parTable.size(), 1);
			Assert::AreEqual(parTable.isContained("0"), true);
			Assert::AreEqual(parTable.isContained("1"), false);

			Assert::AreEqual(parTable.isParent("0", "1"), true);
			Assert::AreEqual(parTable.isParent("0", "2"), true);
			Assert::AreEqual(parTable.isParent("0", "3"), false);
			Assert::AreEqual(parTable.isParent("1", "2"), false);
			Assert::AreEqual(parTable.isParent("2", "0"), false);
		}
		/*TEST_METHOD(TestGenerateParentSTable) {
			ParentTable parentTable;
			parentTable.addToTable("0", "1");
			parentTable.addToTable("0", "2");
			parentTable.addToTable("1", "4");
			parentTable.addToTable("2", "5");
			parentTable.addToTable("0", "3");
			parentTable.addToTable("5", "7");
			parentTable.addToTable("8", "9");
			parentTable.addToTable("9", "10");
			parentTable.addToTable("11", "12");
			ParentSTable* pTable = parentTable.generateParentSTable();

			Assert::AreEqual(pTable->size(),7);
			Assert::AreEqual(pTable->getTable().at(0).lineNo, (string)"0");
			Assert::AreEqual(pTable->getTable().at(0).child.size(), (unsigned)6);
			Assert::AreEqual(pTable->getTable().at(0).child.at(0), (string)"1");
			Assert::AreEqual(pTable->getTable().at(0).child.at(1), (string)"2");
			Assert::AreEqual(pTable->getTable().at(0).child.at(2), (string)"3");
			Assert::AreEqual(pTable->getTable().at(0).child.at(3), (string)"4");
			Assert::AreEqual(pTable->getTable().at(0).child.at(4), (string)"5");
			Assert::AreEqual(pTable->getTable().at(0).child.at(5), (string)"7");

			Assert::AreEqual(pTable->getTable().at(1).lineNo, (string)"1");
			Assert::AreEqual(pTable->getTable().at(1).child.size(), (unsigned)1);
			Assert::AreEqual(pTable->getTable().at(1).child.at(0), (string)"4");

			Assert::AreEqual(pTable->getTable().at(2).lineNo, (string)"2");
			Assert::AreEqual(pTable->getTable().at(2).child.size(), (unsigned)2);
			Assert::AreEqual(pTable->getTable().at(2).child.at(0), (string)"5");
			Assert::AreEqual(pTable->getTable().at(2).child.at(1), (string)"7");

			Assert::AreEqual(pTable->getTable().at(3).lineNo, (string)"5");
			Assert::AreEqual(pTable->getTable().at(3).child.size(), (unsigned)1);
			Assert::AreEqual(pTable->getTable().at(3).child.at(0), (string)"7");

			Assert::AreEqual(pTable->getTable().at(4).lineNo, (string)"8");
			Assert::AreEqual(pTable->getTable().at(4).child.size(), (unsigned)2);
			Assert::AreEqual(pTable->getTable().at(4).child.at(0), (string)"9");
			Assert::AreEqual(pTable->getTable().at(4).child.at(1), (string)"10");

			Assert::AreEqual(pTable->getTable().at(5).lineNo, (string)"9");
			Assert::AreEqual(pTable->getTable().at(5).child.size(), (unsigned)1);
			Assert::AreEqual(pTable->getTable().at(5).child.at(0), (string)"10");

			Assert::AreEqual(pTable->getTable().at(6).lineNo, (string)"11");
			Assert::AreEqual(pTable->getTable().at(6).child.size(), (unsigned)1);
			Assert::AreEqual(pTable->getTable().at(6).child.at(0), (string)"12");
		}*/
	};
}