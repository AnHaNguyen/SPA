#include <string>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessor.h"
#include "Parser.h"
#include "Parser.h"
#include <string>
#include <stdlib.h>
#include <algorithm>

#include <stdio.h>

#include <fstream>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestPreprocessor
{
	TEST_CLASS(TestStartPreprocessor)
	{
	public:

		TEST_METHOD(TestInteger)
		{
			Parser p;
			string sInteger = "123";
			Assert::AreEqual(true, p.isInteger(sInteger));

		}
		TEST_METHOD(no_such_no_pattern)
		{
			QueryPreprocessor pro0;
			string declare0 = "assign a1,a2;";
			string input0 = "Select a1";
			QueryTree* tree0 = pro0.startProcess(declare0, input0);

			Assert::AreEqual(true, tree0->getValidity());
			Assert::AreEqual((string)"a1", tree0->getResult()->getResult());
			return;
		}

		TEST_METHOD(such_pattern)
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

		TEST_METHOD(pattern_such)
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

		TEST_METHOD(such_that_number_attribute)
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

		TEST_METHOD(no_pattern)
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

		TEST_METHOD(no_such_that)
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

		TEST_METHOD(such_that_attr_not_appear_in_declaration)
		{
			QueryPreprocessor pro6;
			string declare6 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input6 = "Select w such that Modifies(bbbbbb, v1) pattern a2(_, _\"x\"_)";
			QueryTree* tree6 = pro6.startProcess(declare6, input6);
			Assert::AreEqual(false, tree6->getValidity());
		}

		TEST_METHOD(pattern_attr_not_appear_in_declaration)
		{
			QueryPreprocessor pro7;
			string declare7 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input7 = "Select w such that Modifies(a1, v1) pattern a2(a4, _\"x\"_)";
			QueryTree* tree7 = pro7.startProcess(declare7, input7);
			Assert::AreEqual(false, tree7->getValidity());
		}

		TEST_METHOD(only_one_quatation_mark)
		{
			QueryPreprocessor pro8;
			string declare8 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input8 = "Select w such that Modifies(a1, v1) pattern a2(a4,_x\"_)";
			QueryTree* tree8 = pro8.startProcess(declare8, input8);
			Assert::AreEqual(false, tree8->getValidity());
		}

		TEST_METHOD(wrong_pattern_synonym)
		{
			QueryPreprocessor pro9;
			string declare9 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input9 = "Select w such that Modifies(a1,v1) pattern omo(_,_\"x\"_)";
			QueryTree* tree9 = pro9.startProcess(declare9, input9);
			Assert::AreEqual(false, tree9->getValidity());
		}

		TEST_METHOD(wrong_such_that_synonym)
		{
			QueryPreprocessor pro10;
			string declare10 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input10 = "Select w such that blah(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree10 = pro10.startProcess(declare10, input10);
			Assert::AreEqual(false, tree10->getValidity());
		}

		TEST_METHOD(such_that_lack_of_left_bracket)
		{
			QueryPreprocessor pro11;
			string declare11 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input11 = "Select w such that Modifies a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree11 = pro11.startProcess(declare11, input11);
			Assert::AreEqual(false, tree11->getValidity());
		}

		TEST_METHOD(such_that_lack_of_comma)
		{
			QueryPreprocessor pro12;
			string declare12 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input12 = "Select w such that Modifies(a1 v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree12 = pro12.startProcess(declare12, input12);
			Assert::AreEqual(false, tree12->getValidity());
		}

		TEST_METHOD(pattern_lack_of_right_bracket)
		{
			QueryPreprocessor pro13;
			string declare13 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input13 = "Select w such that Modifies(a1,v1) pattern a2(_,_\"x\"_ ";
			QueryTree* tree13 = pro13.startProcess(declare13, input13);
			Assert::AreEqual(false, tree13->getValidity());
		}

		TEST_METHOD(pattern_lack_of_comma)
		{
			QueryPreprocessor pro14;
			string declare14 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input14 = "Select w such that Modifies(a1,v1) pattern a2(_ _\"x\"_)";
			QueryTree* tree14 = pro14.startProcess(declare14, input14);
			Assert::AreEqual(false, tree14->getValidity());
		}

		TEST_METHOD(no_select_keyword)
		{
			QueryPreprocessor pro15;
			string declare15 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input15 = "w such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree15 = pro15.startProcess(declare15, input15);
			Assert::AreEqual(false, tree15->getValidity());
		}

		TEST_METHOD(select_nothing)
		{
			QueryPreprocessor pro16;
			string declare16 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input16 = "Select such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree16 = pro16.startProcess(declare16, input16);
			Assert::AreEqual(false, tree16->getValidity());
		}

		TEST_METHOD(select_two_synonym)
		{
			QueryPreprocessor pro17;
			string declare17 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input17 = "Select a1 a2 such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree17 = pro17.startProcess(declare17, input17);
			Assert::AreEqual(false, tree17->getValidity());
		}

		TEST_METHOD(select_not_in_declare)
		{
			QueryPreprocessor pro18;
			string declare18 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input18 = "Select a4 such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree18 = pro18.startProcess(declare18, input18);
			Assert::AreEqual(false, tree18->getValidity());
		}


		TEST_METHOD(declare_synonym_wrong_syntax)
		{
			QueryPreprocessor pro19;
			string declare19 = "assign 2b, a2, a3; while w; stmt s1, s2;variable v1;";
			string input19 = "Select a2 such that Modifies(a2,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree19 = pro19.startProcess(declare19, input19);
			Assert::AreEqual(false, tree19->getValidity());
		}

		TEST_METHOD(pattern_var_varSub)
		{
			QueryPreprocessor pro20;
			string declare20 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input20 = "Select a1 such that Modifies(a2,v1) pattern a2(\"y\",_\"x\"_)";
			QueryTree* tree20 = pro20.startProcess(declare20, input20);
			Assert::AreEqual(true, tree20->getValidity());
			Assert::AreEqual((string)"a1", tree20->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree20->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree20->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree20->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree20->getPattern()->getSynonym());
			Assert::AreEqual((string)"\"y\"", tree20->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree20->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_varSub)
		{
			QueryPreprocessor pro21;
			string declare21 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input21 = "Select a1 such that Modifies(a2,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree21 = pro21.startProcess(declare21, input21);
			Assert::AreEqual(true, tree21->getValidity());
			Assert::AreEqual((string)"a1", tree21->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree21->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree21->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree21->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree21->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree21->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x\"_", tree21->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_numSub)
		{
			QueryPreprocessor pro22;
			string declare22 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input22 = "Select a1 such that Modifies(a2,v1) pattern a2(_,_\"6\"_)";
			QueryTree* tree22 = pro22.startProcess(declare22, input22);
			Assert::AreEqual(true, tree22->getValidity());
			Assert::AreEqual((string)"a1", tree22->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree22->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree22->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree22->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree22->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree22->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"6\"_", tree22->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_num)
		{
			QueryPreprocessor pro23;
			string declare23 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input23 = "Select a1 such that Modifies(a2,v1) pattern a2(_,\"6\")";
			QueryTree* tree23 = pro23.startProcess(declare23, input23);
			Assert::AreEqual(true, tree23->getValidity());
			Assert::AreEqual((string)"a1", tree23->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree23->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree23->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree23->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree23->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree23->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"\"6\"", tree23->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_var)
		{
			QueryPreprocessor pro24;
			string declare24 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input24 = "Select a1 such that Modifies(a2,v1) pattern a2(_,\"x\")";
			QueryTree* tree24 = pro24.startProcess(declare24, input24);
			Assert::AreEqual(true, tree24->getValidity());
			Assert::AreEqual((string)"a1", tree24->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree24->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree24->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree24->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree24->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree24->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"\"x\"", tree24->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_expr1)
		{
			QueryPreprocessor pro25;
			string declare25 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input25 = "Select a1 such that Modifies(a2,v1) pattern a2(_,\"x+y\")";
			QueryTree* tree25 = pro25.startProcess(declare25, input25);
			Assert::AreEqual(true, tree25->getValidity());
			Assert::AreEqual((string)"a1", tree25->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree25->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree25->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree25->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree25->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree25->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"\"x+y\"", tree25->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_underscore_exprSub)
		{
			QueryPreprocessor pro26;
			string declare26 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input26 = "Select a1 such that Modifies(a2,v1) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree26 = pro26.startProcess(declare26, input26);
			Assert::AreEqual(true, tree26->getValidity());
			Assert::AreEqual((string)"a1", tree26->getResult()->getResult());
			Assert::AreEqual((string)"Modifies", tree26->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree26->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree26->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree26->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree26->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x+y\"_", tree26->getPattern()->getSecondAttr());
		}
	};
}