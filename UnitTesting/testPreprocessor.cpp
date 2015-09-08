#include <string>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestPreprocessor
{
	TEST_CLASS(TestStartPreprocessor)
	{
	public:

		TEST_METHOD(NoSuchNoPattern)
		{
			QueryPreprocessor pro0;
			string declare0 = "assign a1, a2;";
			string input0 = "Select a1";
			QueryTree* tree0 = pro0.startProcess(declare0, input0);

			Assert::AreEqual(tree0->getResult()->getResult(), (string)"a1");
			return;
		}

		TEST_METHOD(SuchPattern)
		{
			QueryPreprocessor pro001;
			string declare001 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input001 = "Select w such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree001 = pro001.startProcess(declare001, input001);

			Assert::AreEqual(tree001->getResult()->getResult(), (string)"w");
			Assert::AreEqual(tree001->getSuchThat()->getSynonym(), (string)"Modifies");
			Assert::AreEqual(tree001->getSuchThat()->getFirstAttr(), (string)"a1");
			Assert::AreEqual(tree001->getSuchThat()->getSecondAttr(), (string)"v1");
			Assert::AreEqual(tree001->getPattern()->getSynonym(), (string)"a2");
			Assert::AreEqual(tree001->getPattern()->getFirstAttr(), (string)"_");
			Assert::AreEqual(tree001->getPattern()->getSecondAttr(), (string)"_\"x\"_");
			return;
		}

		TEST_METHOD(PatternSuch)
		{
			QueryPreprocessor pro001;
			string declare001 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input001 = "Select w such that pattern a2(_,_\"x\"_)  Modifies(a1,v1)";
			QueryTree* tree001 = pro001.startProcess(declare001, input001);

			Assert::AreEqual(tree001->getResult()->getResult(), (string)"w");
			Assert::AreEqual(tree001->getSuchThat()->getSynonym(), (string)"Modifies");
			Assert::AreEqual(tree001->getSuchThat()->getFirstAttr(), (string)"a1");
			Assert::AreEqual(tree001->getSuchThat()->getSecondAttr(), (string)"v1");
			Assert::AreEqual(tree001->getPattern()->getSynonym(), (string)"a2");
			Assert::AreEqual(tree001->getPattern()->getFirstAttr(), (string)"_");
			Assert::AreEqual(tree001->getPattern()->getSecondAttr(), (string)"_\"x\"_");
			return;
		}
	};
}