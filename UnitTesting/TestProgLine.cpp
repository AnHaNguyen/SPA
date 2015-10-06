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
			Assert::AreEqual(progLine.getType((string)"1"), (string) "assign");
			Assert::AreEqual(progLine.getType((string) "0"), (string) "");
			Assert::AreEqual(progLine.getType((string) "3"), (string) "if");
			Assert::AreEqual(progLine.getType((string)"14"), (string) "");
			Assert::AreEqual(progLine.getType((string)"9"), (string) "while");

			Assert::AreEqual(progLine.getProcedure((string)"1"), (string) "First");
			Assert::AreEqual(progLine.getProcedure((string)"6"), (string) "First");
			Assert::AreEqual(progLine.getProcedure((string)"7"), (string) "Second");
			Assert::AreEqual(progLine.getProcedure((string)"11"), (string) "Second");
			Assert::AreEqual(progLine.getProcedure((string)"12"), (string) "Third");
			Assert::AreEqual(progLine.getProcedure((string)"0"), (string) "");
			Assert::AreEqual(progLine.getProcedure((string)"14"), (string) "");

			vector<string> first = progLine.getLinesOfProc("First");
			Assert::AreEqual(first.size(), (unsigned)6);
			Assert::AreEqual(first.at(0),(string) "1");
			Assert::AreEqual(first.at(3),(string) "4");
			Assert::AreEqual(first.at(5),(string) "6");
			vector<string> second = progLine.getLinesOfProc("Second");
			Assert::AreEqual(second.size(), (unsigned)5);
			Assert::AreEqual(second.at(0),(string) "7");
			Assert::AreEqual(second.at(3),(string) "10");
			Assert::AreEqual(second.at(4), (string)"11");
			vector<string> third = progLine.getLinesOfProc("Third");
			Assert::AreEqual(third.size(), (unsigned)2);
			Assert::AreEqual(third.at(0), (string)"12");
			Assert::AreEqual(third.at(1), (string) "13");
			Assert::AreEqual(progLine.getLinesOfProc("abc").size(), (unsigned)0);

			vector<string> assigns = progLine.getLinesOfType("assign");
			vector<string> whiles = progLine.getLinesOfType("while");
			vector<string> ifs = progLine.getLinesOfType("if");
			Assert::AreEqual(assigns.size(), (unsigned)8);
			Assert::AreEqual(whiles.size(), (unsigned)3);
			Assert::AreEqual(ifs.size(), (unsigned)2);
			Assert::AreEqual(ifs.at(0), (string)"3");
			Assert::AreEqual(ifs.at(1), (string)"12");
			Assert::AreEqual(whiles.at(0), (string)"5");
			Assert::AreEqual(whiles.at(1), (string)"9");
			Assert::AreEqual(whiles.at(2), (string)"10");
			Assert::AreEqual(assigns.at(0), (string)"1");
			Assert::AreEqual(assigns.at(1), (string)"2");
			Assert::AreEqual(assigns.at(6), (string)"11");
			Assert::AreEqual(assigns.at(7), (string)"13");
		}
	};
}
