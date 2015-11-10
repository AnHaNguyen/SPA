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

		TEST_METHOD(prog_line)
		{
			QueryPreprocessor pro;
			string declare = "assign a1, a2; stmt s1; prog_line n; variable v;";
			string input = "Select n pattern a1(_,\"x\") such that Uses(n,v)";
			QueryTree* tree = pro.startProcess(declare, input);

			Assert::AreEqual(true, tree->getValidity());
			Assert::AreEqual((string)"n", tree->getResult()->getResult().getSynonym());
			return;
		}

		TEST_METHOD(no_such_no_pattern)
		{
			QueryPreprocessor pro0;
			string declare0 = "assign a1,a2;";
			string input0 = "Select a1";
			QueryTree* tree0 = pro0.startProcess(declare0, input0);

			Assert::AreEqual(true, tree0->getValidity());
			Assert::AreEqual((string)"a1", tree0->getResult()->getResult().getSynonym());
			return;
		}

		TEST_METHOD(such_pattern)
		{
			QueryPreprocessor pro001;
			string declare001 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input001 = "Select w such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree001 = pro001.startProcess(declare001, input001);

			Assert::AreEqual(true, tree001->getValidity());
			Assert::AreEqual((string)"w", tree001->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"w", tree100->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"w", tree2->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"w", tree3->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"w", tree4->getResult()->getResult().getSynonym());
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
			string input5 = "  Select w such  that   Modifies  (  a1  ,  v1  )   pattern  a1(  _   ,  _  \"   x   +   y  \"  _   )   with   s1  .  stmt#   =   6";
			QueryTree* tree5 = pro5.startProcess(declare5, input5);
			//Assert::AreEqual(true, tree5->getValidity());
			Assert::AreEqual((string)"w", tree5->getResult()->getResult().getSynonym());
			Assert::AreEqual((string)"Modifies", tree5->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree5->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree5->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a1", tree5->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree5->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x+y\"_", tree5->getPattern()->getSecondAttr());
			Assert::AreEqual((string)"s1", tree5->getWith()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"stmt#", tree5->getWith()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"6", tree5->getWith()->getRightType());
		}

		TEST_METHOD(such_that_attr_not_appear_in_declaration)
		{
			QueryPreprocessor pro6;
			string declare6 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input6 = "Select w such that Modifies(bbbbbb, v1) pattern a2(_, _\"x\"_)";
			QueryTree* tree6 = pro6.startProcess(declare6, input6);
			Assert::AreEqual(false, tree6->getValidity());
		}

		TEST_METHOD(pattern_attr_not_variable)
		{
			QueryPreprocessor pro7;
			string declare7 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input7 = "Select w such that Modifies(a1, v1) pattern a2(a1, _\"x\"_)";
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
			Assert::AreEqual((string)"a1", tree20->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree21->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree22->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree23->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree24->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree25->getResult()->getResult().getSynonym());
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
			Assert::AreEqual((string)"a1", tree26->getResult()->getResult().getSynonym());
			Assert::AreEqual((string)"Modifies", tree26->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree26->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree26->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"a2", tree26->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree26->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_\"x+y\"_", tree26->getPattern()->getSecondAttr());
		}

		TEST_METHOD(Modifies_first_attr_underscore)
		{
			QueryPreprocessor pro27;
			string declare27 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input27 = "Select a1 such that Modifies(_,v1) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree27 = pro27.startProcess(declare27, input27);
			Assert::AreEqual(false, tree27->getValidity());
		}

		TEST_METHOD(Modifies_second_attr_not_variable)
		{
			QueryPreprocessor pro28;
			string declare28 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input28 = "Select a1 such that Modifies(a1,a2) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree28 = pro28.startProcess(declare28, input28);
			Assert::AreEqual(false, tree28->getValidity());
		}

		TEST_METHOD(parent_first_attr_not_container_stmt)
		{
			QueryPreprocessor pro29;
			string declare29 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input29 = "Select a1 such that Parent(a1,a2) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree29 = pro29.startProcess(declare29, input29);
			Assert::AreEqual(false, tree29->getValidity());
		}

		TEST_METHOD(parent_second_attr_not_assign_or_stmt)
		{
			QueryPreprocessor pro30;
			string declare30 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input30 = "Select a1 such that Parent(a1,v1) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree30 = pro30.startProcess(declare30, input30);
			Assert::AreEqual(false, tree30->getValidity());
		}

		TEST_METHOD(such_that_three_attribute)
		{
			QueryPreprocessor pro31;
			string declare31 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input31 = "Select a1 such that Modifies(a1,v1,a2) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree31 = pro31.startProcess(declare31, input31);
			Assert::AreEqual(false, tree31->getValidity());
		}

		TEST_METHOD(pattern_five_attribute)
		{
			QueryPreprocessor pro32;
			string declare32 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input32 = "Select a1 such that Modifies(a1,v1) pattern a2(_,_\"x+y\"_,_,_)";
			QueryTree* tree32 = pro32.startProcess(declare32, input32);
			Assert::AreEqual(false, tree32->getValidity());
		}

		TEST_METHOD(such_taht_missing_attribute)
		{
			QueryPreprocessor pro33;
			string declare33 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input33 = "Select a1 such that Modifies(a1,) pattern a2(_,_\"x+y\"_)";
			QueryTree* tree33 = pro33.startProcess(declare33, input33);
			Assert::AreEqual(false, tree33->getValidity());
		}

		TEST_METHOD(pattern_missing_attribute)
		{
			QueryPreprocessor pro34;
			string declare34 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input34 = "Select a1 such that Modifies(a1,v1) pattern a2(_,)";
			QueryTree* tree34 = pro34.startProcess(declare34, input34);
			Assert::AreEqual(false, tree34->getValidity());
		}

		TEST_METHOD(missing_all_attribute)
		{
			QueryPreprocessor pro35;
			string declare35 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input35 = "Select a1 such that Modifies(,) pattern a2(,)";
			QueryTree* tree35 = pro35.startProcess(declare35, input35);
			Assert::AreEqual(false, tree35->getValidity());
		}


		TEST_METHOD(such_that_table)
		{
			QueryPreprocessor pro36;
			string input36 = "Select w such that Modifies(a1,v1) and Follows(1,1) such that Modifies(1,v1) pattern a2(_,\"x\") such that Uses(s1,v1)";
			pro36.setSuchThatTable(input36);
			Assert::AreEqual((string)"Modifies(1,v1)", pro36.getSuchThatTable()[0]);
			Assert::AreEqual((string)"Modifies(a1,v1)", pro36.getSuchThatTable()[1]);
			Assert::AreEqual((string)"Follows(1,1)", pro36.getSuchThatTable()[2]);
			Assert::AreEqual((string)"Uses(s1,v1)", pro36.getSuchThatTable()[3]);
		}

		TEST_METHOD(such_that_nodes)
		{
			QueryPreprocessor pro37;
			string declare37 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;";
			string input37 = "Select w such that Modifies(a1,v1) and Follows(s1,s2) such that Modifies(1,v1) pattern a2(_,\"x\") such that Uses(s1,v1)";
			QueryTree* tree37 = pro37.startProcess(declare37, input37);

			Assert::AreEqual((string)"Modifies", tree37->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"Modifies", tree37->getSuchThat()->getNext()->getSynonym());
			Assert::AreEqual((string)"Follows", tree37->getSuchThat()->getNext()->getNext()->getSynonym());
			Assert::AreEqual((string)"Uses", tree37->getSuchThat()->getNext()->getNext()->getNext()->getSynonym());
		}

		TEST_METHOD(pattern_table)
		{
			QueryPreprocessor pro38;
			string input38 = "Select w pattern a1(\"x\", \"y\") and a2(_,\"y\") such that Modifies(1,v1) pattern a3(_,\"x\") pattern a4(\"a\", \"b\")";
			pro38.setPatternTable(input38);
			Assert::AreEqual((string)"a1(\"x\", \"y\")", pro38.getPatternTable()[0]);
			Assert::AreEqual((string)"a2(_,\"y\")", pro38.getPatternTable()[1]);
			Assert::AreEqual((string)"a3(_,\"x\")", pro38.getPatternTable()[2]);
			Assert::AreEqual((string)"a4(\"a\", \"b\")", pro38.getPatternTable()[3]);
		}


		TEST_METHOD(pattern_nodes)
		{
			QueryPreprocessor pro39;
			string declare39 = "assign a1, a2, a3, a4; while w; stmt s1, s2;variable v1;";
			string input39 = "Select w pattern a1(\"x\", \"y\") and a2(_,\"y\") such that Modifies(1,v1) pattern a3(_,\"x\") pattern a4(\"a\", _\"b\"_)";
			QueryTree* tree39 = pro39.startProcess(declare39, input39);

			Assert::AreEqual((string)"a1", tree39->getPattern()->getSynonym());
			Assert::AreEqual((string)"a2", tree39->getPattern()->getNext()->getSynonym());
			Assert::AreEqual((string)"a3", tree39->getPattern()->getNext()->getNext()->getSynonym());
			Assert::AreEqual((string)"a4", tree39->getPattern()->getNext()->getNext()->getNext()->getSynonym());
		}

		TEST_METHOD(select_Boolean)
		{
			QueryPreprocessor pro40;
			string declare40 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;";
			string input40 = "Select BOOLEAN such that Modifies(a1,v1) pattern a2(_,_\"x\"_)";
			QueryTree* tree40 = pro40.startProcess(declare40, input40);
			Assert::AreEqual(true, tree40->getValidity());
			Assert::AreEqual((string)"BOOLEAN", tree40->getResult()->getResult().getSynonym());
		}

		TEST_METHOD(pattern_if) {
			QueryPreprocessor pro41;
			string declare41 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input41 = "Select w pattern ifstat(\"x\",_,_)";
			QueryTree* tree41 = pro41.startProcess(declare41, input41);
			Assert::AreEqual(true, tree41->getValidity());
			Assert::AreEqual((string)"ifstat", tree41->getPattern()->getSynonym());
			Assert::AreEqual((string)"\"x\"", tree41->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_", tree41->getPattern()->getSecondAttr());
			Assert::AreEqual((string)"_", tree41->getPattern()->getThirdAttr());
		}

		TEST_METHOD(pattern_while) {
			QueryPreprocessor pro42;
			string declare42 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input42 = "Select w pattern w(v1,_)";
			QueryTree* tree42 = pro42.startProcess(declare42, input42);
			Assert::AreEqual(true, tree42->getValidity());
			Assert::AreEqual((string)"w", tree42->getPattern()->getSynonym());
			Assert::AreEqual((string)"v1", tree42->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"_", tree42->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_not_variable) {
			QueryPreprocessor pro43;
			string declare43 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input43 = "Select w pattern w(a1,_)";
			QueryTree* tree43 = pro43.startProcess(declare43, input43);
			Assert::AreEqual(false, tree43->getValidity());
		}

		TEST_METHOD(pattern_while_wrong_num_attr) {
			QueryPreprocessor pro44;
			string declare44 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input44 = "Select w pattern w(v1,_,_)";
			QueryTree* tree44 = pro44.startProcess(declare44, input44);
			Assert::AreEqual(false, tree44->getValidity());
		}

		TEST_METHOD(pattern_if_wrong_num_attr) {
			QueryPreprocessor pro45;
			string declare45 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input45 = "Select w pattern ifstat(v1,_)";
			QueryTree* tree45 = pro45.startProcess(declare45, input45);
			Assert::AreEqual(false, tree45->getValidity());
		}

		TEST_METHOD(such_that_next) {
			QueryPreprocessor pro46;
			string declare46 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input46 = "Select w such that Next(s1,a1)";
			QueryTree* tree46 = pro46.startProcess(declare46, input46);
			Assert::AreEqual(true, tree46->getValidity());
			Assert::AreEqual((string)"Next", tree46->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"s1", tree46->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"a1", tree46->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(such_that_Calls_p1) {
			QueryPreprocessor pro47;
			string declare47 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;procedure p1,p2;";
			string input47 = "Select w such that Calls(p1,p2)";
			QueryTree* tree47 = pro47.startProcess(declare47, input47);
			Assert::AreEqual(true, tree47->getValidity());
			Assert::AreEqual((string)"Calls", tree47->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"p1", tree47->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"p2", tree47->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(such_that_Calls_proName_underscore) {
			QueryPreprocessor pro48;
			string declare48 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;procedure p1,p2;";
			string input48 = "Select w such that Calls(\"third\",_)";
			QueryTree* tree48 = pro48.startProcess(declare48, input48);
			Assert::AreEqual(true, tree48->getValidity());
			Assert::AreEqual((string)"Calls", tree48->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"\"third\"", tree48->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"_", tree48->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(such_that_Modifies_procedure) {
			QueryPreprocessor pro49;
			string declare49 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;procedure p1,p2;";
			string input49 = "Select w such that Modifies(p1,v1)";
			QueryTree* tree49 = pro49.startProcess(declare49, input49);
			Assert::AreEqual(true, tree49->getValidity());
			Assert::AreEqual((string)"Modifies", tree49->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"p1", tree49->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"v1", tree49->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(such_that_Uses_procedure) {
			QueryPreprocessor pro50;
			string declare50 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;procedure p1,p2;";
			string input50 = "Select w such that Uses(p1,\"x\")";
			QueryTree* tree50 = pro50.startProcess(declare50, input50);
			Assert::AreEqual(true, tree50->getValidity());
			Assert::AreEqual((string)"Uses", tree50->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"p1", tree50->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"\"x\"", tree50->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(with_table)
		{
			QueryPreprocessor pro51;
			string input51 = "Select w with n1 = 10 and p1.procName = v1.varName such that Modifies(1,v1) with s1.stmt# = c1.value with c1.value = 2";
			pro51.setWithTable(input51);
			Assert::AreEqual(4, (int)pro51.getWithTable().size());
			Assert::AreEqual((string)"n1 = 10", pro51.getWithTable()[0]);
			Assert::AreEqual((string)"p1.procName = v1.varName", pro51.getWithTable()[1]);
			Assert::AreEqual((string)"s1.stmt# = c1.value", pro51.getWithTable()[2]);
			Assert::AreEqual((string)"c1.value = 2", pro51.getWithTable()[3]);
		}

		TEST_METHOD(with_nodes)
		{
			QueryPreprocessor pro52;
			string declare52 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input52 = "Select w with n1=10 and p1.procName=v1.varName such that Modifies(1,v1) with s1.stmt#=c1.value with c1.value=8 with p1.procName=\"main\"";
			QueryTree* tree52 = pro52.startProcess(declare52, input52);

			Assert::AreEqual(true, tree52->getValidity());
			Assert::AreEqual((string)"n1", tree52->getWith()->getLeftType());

			Assert::AreEqual((string)"p1", tree52->getWith()->getNext()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"procName", tree52->getWith()->getNext()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"v1", tree52->getWith()->getNext()->getRightAttrRef().getSynonym());
			Assert::AreEqual((string)"varName", tree52->getWith()->getNext()->getRightAttrRef().getAttr());

			Assert::AreEqual((string)"s1", tree52->getWith()->getNext()->getNext()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"stmt#", tree52->getWith()->getNext()->getNext()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"c1", tree52->getWith()->getNext()->getNext()->getRightAttrRef().getSynonym());
			Assert::AreEqual((string)"value", tree52->getWith()->getNext()->getNext()->getRightAttrRef().getAttr());

			Assert::AreEqual((string)"c1", tree52->getWith()->getNext()->getNext()->getNext()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"value", tree52->getWith()->getNext()->getNext()->getNext()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"8", tree52->getWith()->getNext()->getNext()->getNext()->getRightType());

			Assert::AreEqual((string)"p1", tree52->getWith()->getNext()->getNext()->getNext()->getNext()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"procName", tree52->getWith()->getNext()->getNext()->getNext()->getNext()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"\"main\"", tree52->getWith()->getNext()->getNext()->getNext()->getNext()->getRightType());
		}

		TEST_METHOD(with_invalid_refType_1)
		{
			QueryPreprocessor pro53;
			string declare53 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input53 = "Select w with n1 = p1.procName";
			QueryTree* tree53 = pro53.startProcess(declare53, input53);

			Assert::AreEqual(false, tree53->getValidity());
		}

		TEST_METHOD(with_invalid_refType_2)
		{
			QueryPreprocessor pro54;
			string declare54 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input54 = "Select w with p1 = \"main\"";
			QueryTree* tree54 = pro54.startProcess(declare54, input54);

			Assert::AreEqual(false, tree54->getValidity());
		}

		TEST_METHOD(with_invalid_refType_3)
		{
			QueryPreprocessor pro55;
			string declare55 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input55 = "Select w with p1. = \"main\"";
			QueryTree* tree55 = pro55.startProcess(declare55, input55);

			Assert::AreEqual(false, tree55->getValidity());
		}

		TEST_METHOD(result_table)
		{
			QueryPreprocessor pro56;
			string input56 = "Select <w, v1.varName, s1, c1.value> such that Modifies(a1,v1)";
			pro56.setResultTable(input56);
			Assert::AreEqual(4, (int)pro56.getResultTable().size());
			Assert::AreEqual((string)"w", pro56.getResultTable()[0]);
			Assert::AreEqual((string)"v1.varName", pro56.getResultTable()[1]);
			Assert::AreEqual((string)"s1", pro56.getResultTable()[2]);
			Assert::AreEqual((string)"c1.value", pro56.getResultTable()[3]);
		}

		TEST_METHOD(result_nodes)
		{
			QueryPreprocessor pro57;
			string declare57 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input57 = "Select <w, v1.varName, s1, c1.value> such that Modifies(a1,v1)";
			QueryTree* tree57 = pro57.startProcess(declare57, input57);

			Assert::AreEqual(true, tree57->getValidity());
			Assert::AreEqual((string)"w", tree57->getResult()->getResult().getSynonym());
			Assert::AreEqual((string)"v1", tree57->getResult()->getNext()->getResult().getSynonym());
			Assert::AreEqual((string)"varName", tree57->getResult()->getNext()->getResult().getAttr());
			Assert::AreEqual((string)"s1", tree57->getResult()->getNext()->getNext()->getResult().getSynonym());
			Assert::AreEqual((string)"c1", tree57->getResult()->getNext()->getNext()->getNext()->getResult().getSynonym());
			Assert::AreEqual((string)"value", tree57->getResult()->getNext()->getNext()->getNext()->getResult().getAttr());
		}

		TEST_METHOD(result_invalid_ref_1)
		{
			QueryPreprocessor pro58;
			string declare58 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input58 = "Select <w, v1., s1, c1.value> such that Modifies(a1,v1)";
			QueryTree* tree58 = pro58.startProcess(declare58, input58);
			Assert::AreEqual(false, tree58->getValidity());
		}

		TEST_METHOD(result_invalid_ref_2)
		{
			QueryPreprocessor pro59;
			string declare59 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input59 = "Select <w, v1.procName, s1, c1.value> such that Modifies(a1,v1)";
			QueryTree* tree59 = pro59.startProcess(declare59, input59);
			Assert::AreEqual(false, tree59->getValidity());
		}

		TEST_METHOD(result_invalid_comma)
		{
			QueryPreprocessor pro60;
			string declare60 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input60 = "Select <w, v1.varName,  , c1.value> such that Modifies(a1,v1)";
			QueryTree* tree60 = pro60.startProcess(declare60, input60);
			Assert::AreEqual(false, tree60->getValidity());
		}

		TEST_METHOD(such_that_sorting)
		{
			QueryPreprocessor pro61;
			string declare61 = "assign a1, a2, a3; while w; stmt s1, s2;variable v1;prog_line n1,n2;procedure p1;constant c1;";
			string input61 = "Select w such that Next(20,a1) and Next(1,2) and Modifies(a1,v1) and Parent(s1,s2) and Parent*(1,2) and Parent(4,5) and Parent(2,s1)";
			QueryTree* tree61 = pro61.startProcess(declare61, input61);
			Assert::AreEqual(true, tree61->getValidity());
			Assert::AreEqual((string)"Modifies(a1,v1)", pro61.getSuchThatTable()[0]);
			Assert::AreEqual((string)"Parent(4,5)", pro61.getSuchThatTable()[1]);
			Assert::AreEqual((string)"Parent(2,s1)", pro61.getSuchThatTable()[2]);
			Assert::AreEqual((string)"Parent(s1,s2)", pro61.getSuchThatTable()[3]);
			Assert::AreEqual((string)"Parent*(1,2)", pro61.getSuchThatTable()[4]);
			Assert::AreEqual((string)"Next(1,2)", pro61.getSuchThatTable()[5]);
			Assert::AreEqual((string)"Next(20,a1)", pro61.getSuchThatTable()[6]);
		}

		TEST_METHOD(such_that_affects) {
			QueryPreprocessor pro62;
			string declare62 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input62 = "Select a1 such that Affects(a2,a1)";
			QueryTree* tree62 = pro62.startProcess(declare62, input62);
			Assert::AreEqual(true, tree62->getValidity());
			Assert::AreEqual((string)"Affects", tree62->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a2", tree62->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"a1", tree62->getSuchThat()->getSecondAttr());
		}

		TEST_METHOD(pattern_invalid_expr_1) {
			QueryPreprocessor pro63;
			string declare63 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input63 = "Select a1 pattern a1(_, \"+y\")";
			QueryTree* tree63 = pro63.startProcess(declare63, input63);
			Assert::AreEqual(false, tree63->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_2) {
			QueryPreprocessor pro64;
			string declare64 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input64 = "Select a1 pattern a1(_, \"x+y=j\")";
			QueryTree* tree64 = pro64.startProcess(declare64, input64);
			Assert::AreEqual(false, tree64->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_lack_left_bracket) {
			QueryPreprocessor pro65;
			string declare65 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input65 = "Select a1 pattern a1(_, \"(x+y)+(6+8\")";
			QueryTree* tree65 = pro65.startProcess(declare65, input65);
			Assert::AreEqual(false, tree65->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_4) {
			QueryPreprocessor pro66;
			string declare66 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input66 = "Select a1 pattern a1(_, \"2(x+y)\")";
			QueryTree* tree66 = pro66.startProcess(declare66, input66);
			Assert::AreEqual(false, tree66->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_5) {
			QueryPreprocessor pro67;
			string declare67 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input67 = "Select a1 pattern a1(_, \"(+5)\")";
			QueryTree* tree67 = pro67.startProcess(declare67, input67);
			Assert::AreEqual(false, tree67->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_6) {
			QueryPreprocessor pro68;
			string declare68 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input68 = "Select a1 pattern a1(_, \"(4+)\")";
			QueryTree* tree68 = pro68.startProcess(declare68, input68);
			Assert::AreEqual(false, tree68->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_7) {
			QueryPreprocessor pro69;
			string declare69 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input69 = "Select a1 pattern a1(_, \"(x+y)6\")";
			QueryTree* tree69 = pro69.startProcess(declare69, input69);
			Assert::AreEqual(false, tree69->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_8) {
			QueryPreprocessor pro70;
			string declare70 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input70 = "Select a1 pattern a1(_, \"(3*+y)\")";
			QueryTree* tree70 = pro70.startProcess(declare70, input70);
			Assert::AreEqual(false, tree70->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_9) {
			QueryPreprocessor pro71;
			string declare71 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input71 = "Select a1 pattern a1(_, \"(x+y)(8+4)\")";
			QueryTree* tree71 = pro71.startProcess(declare71, input71);
			Assert::AreEqual(false, tree71->getValidity());
		}

		TEST_METHOD(pattern_expr_full) {
			QueryPreprocessor pro72;
			string declare72 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input72 = "Select a1 pattern a1(_, \"(x+y*(7+6))*((zz-y)+83+4)-big+(3*good-654)-2\")";
			QueryTree* tree72 = pro72.startProcess(declare72, input72);
			Assert::AreEqual(true, tree72->getValidity());
			Assert::AreEqual((string)"a1", tree72->getPattern()->getSynonym());
			Assert::AreEqual((string)"_", tree72->getPattern()->getFirstAttr());
			Assert::AreEqual((string)"\"(x+y*(7+6))*((zz-y)+83+4)-big+(3*good-654)-2\"", tree72->getPattern()->getSecondAttr());
		}

		TEST_METHOD(pattern_invalid_format_lack_underscore) {
			QueryPreprocessor pro73;
			string declare73 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input73 = "Select a1 pattern a1(_, _\"x\")";
			QueryTree* tree73 = pro73.startProcess(declare73, input73);
			Assert::AreEqual(false, tree73->getValidity());
		}

		TEST_METHOD(pattern_invalid_expr_lack_right_bracket) {
			QueryPreprocessor pro74;
			string declare74 = "assign a1,a2,a3;while w;stmt s1,s2;variable v1;if ifstat;";
			string input74 = "Select a1 pattern a1(_, _\"x\")";
			QueryTree* tree74 = pro74.startProcess(declare74, input74);
			Assert::AreEqual(false, tree74->getValidity());
		}

		TEST_METHOD(such_that_Modifeis_firstAttr_quotation) {
			QueryPreprocessor pro75;
			string declare75 = "assign a; variable v; procedure p1, p2;";
			string input75 = "Select a such that Modifies(\"Main\", v)";
			QueryTree* tree75 = pro75.startProcess(declare75, input75);
			Assert::AreEqual(true, tree75->getValidity());
		}

		TEST_METHOD(Follows_with_S_Stmt_2) {
			QueryPreprocessor pro76;
			string declare76 = "assign a; variable v; procedure p1, p2; stmt s;";
			string input76 = "Select s such that Follows(1,s) with s.stmt# = 2";
			QueryTree* tree76 = pro76.startProcess(declare76, input76);
			Assert::AreEqual(true, tree76->getValidity());
			Assert::AreEqual((string)"Follows", tree76->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"1", tree76->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"s", tree76->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"s", tree76->getWith()->getLeftAttrRef().getSynonym());
			Assert::AreEqual((string)"stmt#", tree76->getWith()->getLeftAttrRef().getAttr());
			Assert::AreEqual((string)"2", tree76->getWith()->getRightType());
		}

		TEST_METHOD(duplicated_such_that_clause) {
			QueryPreprocessor pro77;
			string declare77 = "assign a; variable v; procedure p1, p2; stmt s;";
			string input77 = "Select s such that Follows(1,s) and Follows(1,s)";
			QueryTree* tree77 = pro77.startProcess(declare77, input77);
			Assert::AreEqual(true, tree77->getValidity());
			Assert::AreEqual((string)"Follows", tree77->getSuchThat()->getSynonym());
			Assert::IsNull(tree77->getSuchThat()->getNext());
		}

		TEST_METHOD(duplicated_delclaration) {
			QueryPreprocessor pro78;
			string declare78 = "assign a; variable a; procedure p1, p2; stmt s;";
			string input78 = "Select s such that Follows(1,s)";
			QueryTree* tree78 = pro78.startProcess(declare78, input78);
			Assert::AreEqual(false, tree78->getValidity());
		}

		TEST_METHOD(extra_word_between_clause) {
			QueryPreprocessor pro79;
			string declare79 = "assign a1,a2; variable v; procedure p1, p2; stmt s;";
			string input79 = "Select s such that Follows(1,s) pattern a2(_,_) yyy such that Next(a1,a2)";
			QueryTree* tree79 = pro79.startProcess(declare79, input79);
			Assert::AreEqual(false, tree79->getValidity());
		}

		TEST_METHOD(such_that_contain_sibling) {
			QueryPreprocessor pro80;
			string declare80 = "assign a1,a2; variable v; procedure p1, p2; stmt s;plus add;";
			string input80 = "Select s such that Contains*(a1,add)   and   Sibling(p1,6)";
			QueryTree* tree80 = pro80.startProcess(declare80, input80);
			Assert::AreEqual(true, tree80->getValidity());
			Assert::AreEqual((string)"Contains*", tree80->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"a1", tree80->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"add", tree80->getSuchThat()->getSecondAttr());
			Assert::AreEqual((string)"Sibling", tree80->getSuchThat()->getNext()->getSynonym());
			Assert::AreEqual((string)"p1", tree80->getSuchThat()->getNext()->getFirstAttr());
			Assert::AreEqual((string)"6", tree80->getSuchThat()->getNext()->getSecondAttr());

		}

		TEST_METHOD(empty_declaration) {
			QueryPreprocessor pro81;
			string declare81 = "\n";
			string input81 = "Select BOOLEAN such that Next(1,2)";
			QueryTree* tree81 = pro81.startProcess(declare81, input81);
			Assert::AreEqual(true, tree81->getValidity());
			Assert::AreEqual((string)"Next", tree81->getSuchThat()->getSynonym());
			Assert::AreEqual((string)"1", tree81->getSuchThat()->getFirstAttr());
			Assert::AreEqual((string)"2", tree81->getSuchThat()->getSecondAttr());

		}

		TEST_METHOD(empty_clause) {
			QueryPreprocessor pro82;
			string declare82 = "prog_line n;";
			string input82 = "Select BOOLEAN with such that Follows(1,2)";
			QueryTree* tree82 = pro82.startProcess(declare82, input82);
			Assert::AreEqual(false, tree82->getValidity());
		}
	};
}