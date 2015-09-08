#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(ASTTest)
	{
	public:
		
		TEST_METHOD(TestAstMakeChild)
		{
			// TODO: Your test code here
			TNode* procNode = new TNode("Planet", "procedure", 0);
			TNode* node1 = new TNode("", "assign", 1);
			TNode* node3 = new TNode("x", "var", 1);
			TNode* node4 = new TNode("1", "const", 1);
			AST tree;
			tree.addToTree(procNode);
			
			tree.makeChild(procNode, node1);
			tree.addToTree(node1);
			tree.makeChild(node1, node3);
			tree.addToTree(node3);
			tree.makeChild(node1, node4);
			tree.addToTree(node4);
			
			Assert::AreEqual(node1->equals(*(tree.findChild(procNode).at(0))), true);
			Assert::AreEqual(node3->equals(*(tree.findChild(node1).at(0))), true);
			Assert::AreEqual(node4->equals(*(tree.findChild(node1).at(1))), true);
		}

		TEST_METHOD(TestAstMakeParent)
		{
			TNode* procNode = new TNode("Planet", "procedure", 0);
			TNode* node1 = new TNode("", "assign", 1);
			TNode* node3 = new TNode("x", "var", 1);
			TNode* node4 = new TNode("1", "const", 1);
			AST tree;
			tree.addToTree(node3);
			tree.addToTree(node4);

			tree.makeParent(node3, node1);
			Assert::AreEqual(tree.addToTree(node1),true);
			tree.makeParent(node4, node1);
			tree.makeParent(node1, procNode);
			tree.addToTree(procNode);

			Assert::AreEqual(tree.findParent(node1)->equals(*procNode), true);
			Assert::AreEqual(tree.findParent(node3)->equals(*node1), true);
			Assert::AreEqual(tree.findParent(node4)->equals(*node1), true);
		}

		TEST_METHOD(TestAstFindNode) {
			TNode* node1 = new TNode("", "assign", 1);
			TNode* node3 = new TNode("x", "var", 1);
			TNode* node4 = new TNode("", "assign", 1);
			TNode* node5 = new TNode("y", "procedure", 0);
			AST tree;
			tree.addToTree(node1);
			tree.makeChild(node1, node3);
			tree.addToTree(node3);
			Assert::AreEqual(tree.findNode(node1)->equals(*node1), true);
			Assert::AreEqual(tree.findNode(node4)->equals(*node1), true);
			string empty = "";
			Assert::AreEqual(tree.findNode(node5)->equals(TNode("","",0)), true);

		}
	};
}