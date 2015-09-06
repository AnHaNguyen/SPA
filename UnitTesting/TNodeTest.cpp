#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TNodeTest)
	{
	public:

		TEST_METHOD(TestNode)
		{
			// TODO: Your test code here
			TNode* node1 = new TNode("", "assign", 1);
			TNode* node3 = new TNode("x", "var", 1);
			TNode* node4 = new TNode("x", "const", 1);
			string type = "assign";
			Assert::AreEqual(node1->getType(), type);

			Assert::AreEqual(node3->equals(*node4), false);
			node4->setType("var");
			Assert::AreEqual(node3->equals(*node4), true);

			node1->setValue("t");
			string value = "t";
			Assert::AreEqual(node1->getValue(), value);

			Assert::AreEqual(node1->getLine(), 1);
			node1->setLine(2);
			Assert::AreEqual(node1->getLine(), 2);
			node1->setChild(node3);
			node1->setChild(node4);
			vector<TNode* > childList = { node3, node4 };
			vector<TNode* > realList = node1->getChildList();
			Assert::AreEqual(realList.at(0)->equals(*(childList.at(0))), true);
			Assert::AreEqual(realList.at(1)->equals(*(childList.at(1))), true);
		}
	};
}