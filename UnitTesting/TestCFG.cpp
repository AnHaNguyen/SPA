#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFG.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(CFGTest)
	{
	public:
		CFG* cfg = new CFG("First");

		TEST_METHOD(TestCFGMakeChild)
		{
			// TODO: Your test code here
			Assert::AreEqual(cfg->getProcedure(), (string) "First");
			GNode* node1 = new GNode(1);
			GNode* node3 = new GNode(2);
			GNode* node4 = new GNode(3);


			Assert::AreEqual(cfg->addToGraph(node1), true);
			cfg->makeChild(node1, node3);
			Assert::AreEqual(cfg->addToGraph(node3), true);
			Assert::AreEqual(cfg->addToGraph(node1), false);

			cfg->makeChild(node1, node4);
			cfg->addToGraph(node4);

			Assert::AreEqual(node3->getLine(), node1->getChild().at(0)->getLine());
			Assert::AreEqual(node4->getLine(), node1->getChild().at(1)->getLine());
			Assert::AreEqual(node1->getLine(), node3->getParent().at(0)->getLine());
			Assert::AreEqual(node1->getLine(), node4->getParent().at(0)->getLine());
		}

		TEST_METHOD(TestCFGMakeParent)
		{
			GNode* node1 = new GNode(1);
			GNode* node3 = new GNode(2);
			GNode* node4 = new GNode(3);


			Assert::AreEqual(cfg->addToGraph(node1), true);
			Assert::AreEqual(cfg->addToGraph(node3), true);
			cfg->makeParent(node3, node1);
			Assert::AreEqual(cfg->addToGraph(node1), false);
			cfg->addToGraph(node4);
			cfg->makeParent(node4, node1);


			Assert::AreEqual(node3->getLine(), node1->getChild().at(0)->getLine());
			Assert::AreEqual(node4->getLine(), node1->getChild().at(1)->getLine());
			Assert::AreEqual(node1->getLine(), node3->getParent().at(0)->getLine());
			Assert::AreEqual(node1->getLine(), node4->getParent().at(0)->getLine());
		}

		TEST_METHOD(TestCFGBuildNextTable) {
			for (unsigned i = 1; i <= 10; i++) {
				cfg->addToGraph(new GNode(i));
			}
			vector<GNode*> table = cfg->getTable();
			cfg->makeChild(table.at(0), table.at(1));
			cfg->makeChild(table.at(1), table.at(2));
			cfg->makeChild(table.at(2), table.at(3));
			cfg->makeChild(table.at(2), table.at(5));
			cfg->makeChild(table.at(3), table.at(4));
			cfg->makeChild(table.at(5), table.at(6));
			cfg->makeChild(table.at(4), table.at(6));
			cfg->makeChild(table.at(6), table.at(7));
			cfg->makeChild(table.at(7), table.at(8));
			cfg->makeChild(table.at(8), table.at(6));
			cfg->makeChild(table.at(6), table.at(9));

			NextTable* nextTable = new NextTable();
			cfg->buildNextTable(nextTable);
			Assert::AreEqual(nextTable->size(), 9);
			Assert::AreEqual(nextTable->isNext("1", "2"),true);
			Assert::AreEqual(nextTable->isNext("2", "3"), true);
			Assert::AreEqual(nextTable->isNext("3", "4"), true);
			Assert::AreEqual(nextTable->isNext("3", "6"), true);
			Assert::AreEqual(nextTable->isNext("4", "5"), true);
			Assert::AreEqual(nextTable->isNext("5", "7"), true);
			Assert::AreEqual(nextTable->isNext("7", "8"), true);
			Assert::AreEqual(nextTable->isNext("7", "10"), true);
			Assert::AreEqual(nextTable->isNext("1", "3"), false);
			Assert::AreEqual(nextTable->isNext("4", "7"), false);
			Assert::AreEqual(nextTable->isNext("8", "10"), false);
			Assert::AreEqual(nextTable->isNext("9", "7"), true);
		}
	};
}