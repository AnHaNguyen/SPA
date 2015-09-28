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
	};
}