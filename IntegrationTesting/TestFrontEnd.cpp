#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "Preprocessor\QueryPreprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestInteractParserToPKB) {
			Parser par;
			par.parseInput("simple.txt");
			ModifyTable* modTable = PKB::getModifyTable();
			UseTable* useTable = PKB::getUseTable();
			FollowTable* followTable = PKB::getFollowTable();
			ParentTable* parTable = PKB::getParentTable();
			NextTable* nextTable = PKB::getNextTable();

			Assert::AreEqual(modTable->size(), 21);
			Assert::AreEqual(useTable->size(),17);
			Assert::AreEqual(followTable->size(), 12);
			Assert::AreEqual(parTable->size(), 7);
			Assert::AreEqual(nextTable->size(), 20);
		}

		TEST_METHOD(TestInteractPreprocessorEvaluator) {
			string query = "assign a; Select a such that Follows(1, a)";
			QueryPreprocessor qpp;

			FollowTable* fTable = new FollowTable();
			fTable->addToTable("1", "2");
			ProgLine* pLine = new ProgLine();
			pLine->addToList(1, "assign", "ABC");
			pLine->addToList(2, "assign", "ABC");
			PKB::setFollowTable(fTable);
			PKB::setProgLine(pLine);

			vector<string> result = qpp.start(query);
			//Assert::AreEqual(result.size(), (unsigned) 0);

			Assert::AreEqual(result.size(),(unsigned) 1);
			Assert::AreEqual(result.at(0), (string) "2");


		}
	};
}