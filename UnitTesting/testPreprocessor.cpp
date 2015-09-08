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

		TEST_METHOD(SuchThatNumberAttribute)
		{
			QueryPreprocessor pro2;
			string declare2 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input2 = "Select w such that Modifies(a1,v1)";
			QueryTree* tree2 = pro2.startProcess(declare2, input2);
			Assert::AreEqual(true, tree2->getValidity());
			Assert::AreEqual((string)"w", tree2->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree2->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree2->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree2->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"", tree2->getPattern()->getSynonym());
			Assert::AreEqual((string)"", tree2->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"", tree2->getPattern()->getSecondAttr());
		}

		TEST_METHOD(noPattern)
		{
			QueryPreprocessor pro3;
			string declare3 = "assign a1,a2,a3;while w;stmt s1,s2;";
			string input3 = "Select w such that Modifies(3,\"x\") pattern a1(_,_\"x\"_)";
			QueryTree* tree3 = pro3.startProcess(declare3, input3);
			Assert::AreEqual(true, tree3->getValidity());
			Assert::AreEqual((string)"w", tree3->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree3->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"3", tree3->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"\"x\"", tree3->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a1", tree3->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree3->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree3->getPattern()->getSecondAttr());
		}

		TEST_METHOD(noSuchThat)
		{
			QueryPreprocessor pro4;
			string declare4 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input4 = "Select w such that Modifies(a1,v1)";
			QueryTree* tree4 = pro4.startProcess(declare4, input4);
			Assert::AreEqual(true, tree4->getValidity());
			Assert::AreEqual((string)"w", tree4->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree4->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree4->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree4->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"", tree4->getPattern()->getSynonym());
			Assert::AreEqual((string)"", tree4->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"", tree4->getPattern()->getSecondAttr());
		}

		TEST_METHOD(space)
		{
			QueryPreprocessor pro5;
			string declare5 = "assign   a1 ,  a2 ,  a3  ;  while  w ;   stmt s1  ,  s2    ;  variable  v1  ; ";
			string input5 = "  Select w such  that   Modifies  (  a1  , v1  )   pattern  a1(  _  ,  _ \" x \" _   )";
			QueryTree* tree5 = pro5.startProcess(declare5, input5);
			Assert::AreEqual(true, tree5->getValidity());
			Assert::AreEqual((string)"w", tree5->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree5->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree5->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree5->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a1", tree5->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree5->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree5->getPattern()->getSecondAttr());
		}

		TEST_METHOD(suchThatAttrNotAppearInDeclaration)
		{
			QueryPreprocessor pro6;
			string declare6 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input6 = "Select w such that Modifies(bbbbbb, v1) pattern a2(_, _\"x\"_)";
			QueryTree* tree6 = pro6.startProcess(declare6, input6);
			Assert::AreEqual(false, tree6->getValidity());
		}

		TEST_METHOD(patternAttrNotAppearInDeclaration)
		{
			QueryPreprocessor pro7;
			string declare7 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input7 = "Select w such that Modifies(a1, v1) pattern a2(a4, _\"x\"_)";
			QueryTree* tree7 = pro7.startProcess(declare7, input7);
			Assert::AreEqual(false, tree7->getValidity());
		}

		TEST_METHOD(onlyOneQuatationMark)
		{
			QueryPreprocessor pro8;
			string declare8 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input8 = "Select w such that Modifies(a1, v1) pattern a2(a4,_x\"_)";
			QueryTree* tree8 = pro8.startProcess(declare8, input8);
			Assert::AreEqual(false, tree8->getValidity());
		}


	};
}