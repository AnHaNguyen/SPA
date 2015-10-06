#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProgLine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ProgLineTest)
	{
	public:

		TEST_METHOD(TestProgLine)
		{
			// TODO: Your test code here
			ProgLine progLine;
			Assert::AreEqual(progLine.addToList(1, "assign", "First"), true);
			Assert::AreEqual(progLine.addToList(1, "assign", "Second"), false);
			Assert::AreEqual(progLine.addToList(2, "assign", "First"), true);
			Assert::AreEqual(progLine.addToList(3, "if", "First"), true);
			Assert::AreEqual(progLine.addToList(2, "if", "First"), false);
			Assert::AreEqual(progLine.addToList(4, "assign", "First"), true);
			Assert::AreEqual(progLine.addToList(5, "while", "First"), true);
			Assert::AreEqual(progLine.addToList(6, "assign", "First"), true);
			Assert::AreEqual(progLine.addToList(7, "assign", "Second"), true);
			Assert::AreEqual(progLine.addToList(8, "assign", "Second"), true);
			Assert::AreEqual(progLine.addToList(9, "while", "Second"), true);
			Assert::AreEqual(progLine.addToList(10, "while", "Second"), true);
			Assert::AreEqual(progLine.addToList(11, "assign", "Second"), true);
			Assert::AreEqual(progLine.addToList(12, "if", "Third"), true);
			Assert::AreEqual(progLine.addToList(13, "assign", "Third"), true);

			Assert::AreEqual(progLine.numOfLines(), 13);
			Assert::AreEqual(progLine.getType(1), (string) "assign");
			Assert::AreEqual(progLine.getType(0), (string) "");
			Assert::AreEqual(progLine.getType(3), (string) "if");
			Assert::AreEqual(progLine.getType(14), (string) "");
			Assert::AreEqual(progLine.getType(9), (string) "while");

			Assert::AreEqual(progLine.getProcedure(1), (string) "First");
			Assert::AreEqual(progLine.getProcedure(6), (string) "First");
			Assert::AreEqual(progLine.getProcedure(7), (string) "Second");
			Assert::AreEqual(progLine.getProcedure(11), (string) "Second");
			Assert::AreEqual(progLine.getProcedure(12), (string) "Third");
			Assert::AreEqual(progLine.getProcedure(0), (string) "");
			Assert::AreEqual(progLine.getProcedure(14), (string) "");

			vector<int> first = progLine.getLinesOfProc("First");
			Assert::AreEqual(first.size(), (unsigned)6);
			Assert::AreEqual(first.at(0), 1);
			Assert::AreEqual(first.at(3), 4);
			Assert::AreEqual(first.at(5), 6);
			vector<int> second = progLine.getLinesOfProc("Second");
			Assert::AreEqual(second.size(), (unsigned)5);
			Assert::AreEqual(second.at(0), 7);
			Assert::AreEqual(second.at(3), 10);
			Assert::AreEqual(second.at(4), 11);
			vector<int> third = progLine.getLinesOfProc("Third");
			Assert::AreEqual(third.size(), (unsigned)2);
			Assert::AreEqual(third.at(0), 12);
			Assert::AreEqual(third.at(1), 13);
			Assert::AreEqual(progLine.getLinesOfProc("abc").size(), (unsigned)0);

			vector<int> assigns = progLine.getLinesOfType("assign");
			vector<int> whiles = progLine.getLinesOfType("while");
			vector<int> ifs = progLine.getLinesOfType("if");
			Assert::AreEqual(assigns.size(), (unsigned)8);
			Assert::AreEqual(whiles.size(), (unsigned)3);
			Assert::AreEqual(ifs.size(), (unsigned)2);
			Assert::AreEqual(ifs.at(0), 3);
			Assert::AreEqual(ifs.at(1), 12);
			Assert::AreEqual(whiles.at(0), 5);
			Assert::AreEqual(whiles.at(1), 9);
			Assert::AreEqual(whiles.at(2), 10);
			Assert::AreEqual(assigns.at(0), 1);
			Assert::AreEqual(assigns.at(1), 2);
			Assert::AreEqual(assigns.at(6), 11);
			Assert::AreEqual(assigns.at(7), 13);
		}
	};
}
