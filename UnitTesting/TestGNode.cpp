#include "stdafx.h"
#include "CppUnitTest.h"
#include "GNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(GNodeTest)
	{
	public:

		TEST_METHOD(TesGNode)
		{
			// TODO: Your test code here
			GNode* node1 = new GNode(1);
			GNode* node3 = new GNode(2);
			GNode* node4 = new GNode(3);
			
			Assert::AreEqual(node1->getLine(), 1);
			node1->setLine(4);
			Assert::AreEqual(node1->getLine(), 4);

			node1->setChild(node3);
			node1->setChild(node4);
			vector<GNode* > childList = { node3, node4 };
			vector<GNode* > realList = node1->getChild();
			Assert::AreEqual(realList.at(0)->getLine(),childList.at(0)->getLine());
			Assert::AreEqual(realList.at(1)->getLine(), childList.at(1)->getLine());
		
			node3->setParent(node1);
			node3->setParent(node4);
			Assert::AreEqual(node3->getParent().at(0)->getLine(), node1->getLine());
			Assert::AreEqual(node3->getParent().at(1)->getLine(), node4->getLine());
		}
	};
}