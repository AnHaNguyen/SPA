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
			string declare0 = "assign a1,a2;";
			string input0 = "Select a1";
			QueryTree* tree0 = pro0.startProcess(declare0, input0);

			Assert::AreEqual(true, tree0->getValidity());
			Assert::AreEqual((string)"a1", tree0->getResult()->getResult());
			return;
		}

		TEST_METHOD(SuchPattern)
		{
			QueryPreprocessor pro001;
			string declare001 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input001 = "Select w such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree001 = pro001.startProcess(declare001, input001);

			Assert::AreEqual(true, tree001->getValidity());
			Assert::AreEqual((string)"w", tree001->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree001->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree001->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree001->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree001->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree001->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree001->getPattern()->getSecondAttr());
			return;
		}

		TEST_METHOD(PatternSuch)
		{
			QueryPreprocessor pro100;
			string declare100 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input100 = "Select w pattern a2(_,_\"x\"_) such that Modifies(a1,v1)";
			QueryTree* tree100 = pro100.startProcess(declare100, input100);

			Assert::AreEqual(true, tree100->getValidity());
			Assert::AreEqual((string)"w", tree100->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree100->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree100->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree100->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree100->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree100->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree100->getPattern()->getSecondAttr());
			return;
		}

		TEST_METHOD(noPattern)
		{
			QueryPreprocessor pro2;
			string declare2 = "assign a1,a2,a3;while w;stmt s1,s2;";
			string input2 = "Select w such that Modifies(3,\"x\") pattern a1(_,_\"x\"_)";
			QueryTree* tree2 = pro2.startProcess(declare2, input2);
			Assert::AreEqual(true, tree2->getValidity());
			Assert::AreEqual((string)"w", tree2->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree2->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"3", tree2->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"\"x\"", tree2->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a1", tree2->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree2->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree2->getPattern()->getSecondAttr());
		}

		TEST_METHOD(SuchThatNumberAttribute)
		{
			QueryPreprocessor pro3;
			string declare3 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input3 = "Select w such that Modifies(a1,v1)";
			QueryTree* tree3 = pro3.startProcess(declare3, input3);
			Assert::AreEqual(true, tree3->getValidity());
			Assert::AreEqual((string)"w", tree3->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree3->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree3->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree3->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"", tree3->getPattern()->getSynonym());
			Assert::AreEqual((string)"", tree3->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"", tree3->getPattern()->getSecondAttr());
		}

		TEST_METHOD(noSuchThat)
		{
			QueryPreprocessor pro3;
			string declare3 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input3 = "Select w such that Modifies(a1,v1)";
			QueryTree* tree3 = pro3.startProcess(declare3, input3);
			Assert::AreEqual(true, tree3->getValidity());
			Assert::AreEqual((string)"w", tree3->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree3->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree3->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree3->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"", tree3->getPattern()->getSynonym());
			Assert::AreEqual((string)"", tree3->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"", tree3->getPattern()->getSecondAttr());
		}
	};
}