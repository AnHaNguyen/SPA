#include "stdafx.h"
#include "CppUnitTest.h"

#include "QueryPreprocessor.h"
#include "PKB.h"
#include "DesignExtractor.h"
#include "QueryProcessor\QueryHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestProcessor {
	TEST_CLASS(TestProcessor) {
		public:
		TEST_METHOD(Processor_Follows) {
			vector<string> code = {
				"procedure First{",
				"x=1;",			// 1
				"z=3;",			// 2
				"while x{",		// 3  parent
				"x=x-1;",		// 4
				"while x{",		// 5  p
				"y=x-1;",		// 6
				"while y{",		// 7  p
				"x=x+2;",		// 8
				"while y{",		// 9  p
				"y=1+1;",		// 10
				"while x{",		// 11 p
				"x=2;",			// 12
				"x=x+1;}}}}}}",	// 13
				"procedure Second{",
				"a=1;",			// 14
				"while a{",		// 15 p
				"b=1+a;",		// 16
				"while a{",		// 17 p
				"b=b-1;}}}",	// 18
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
			 *	stmt s1,s2;
			 *	Select s1 such that Follows(s1, s2);
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("stmt s1,s2");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("s1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Follows(s1, s2)");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(9, int(results.size()));
			Assert::AreEqual(string("1"), results[0]);
			Assert::AreEqual(string("2"), results[1]);
			Assert::AreEqual(string("4"), results[2]);
			Assert::AreEqual(string("6"), results[3]);
			Assert::AreEqual(string("8"), results[4]);
			Assert::AreEqual(string("10"), results[5]);
			Assert::AreEqual(string("12"), results[6]);
			Assert::AreEqual(string("14"), results[7]);
			Assert::AreEqual(string("16"), results[8]);



			/**
			 *	stmt s1,s2;
			 *	Select s2 such that Follows(s1, s2);
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("stmt s1,s2");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("s2");
			tree2->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Follows(s1, s2)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			// For Tue
			// This seems to be returning the stmt for s1, not s2.
			results = handler.queryRec(tree1);
			/*Assert::AreEqual(9, int(results.size()));
			Assert::AreEqual(string("2"), results[0]);
			Assert::AreEqual(string("3"), results[1]);
			Assert::AreEqual(string("5"), results[2]);
			Assert::AreEqual(string("7"), results[3]);
			Assert::AreEqual(string("9"), results[4]);
			Assert::AreEqual(string("11"), results[5]);
			Assert::AreEqual(string("13"), results[6]);
			Assert::AreEqual(string("15"), results[7]);
			Assert::AreEqual(string("17"), results[8]);*/



			/**
			 *	stmt s1;
			 *	Select s1 such that Follows(9, s1);
			 *
			 *	stmt# 9 is the last statement at its nesting level
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree3->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Follows(9, s1)");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);
			Assert::AreEqual(0, int(results.size()));



			/**
			 *	stmt s1;
			 *	Select s1 such that Follows(16, s1);
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("stmt s1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree4->setResult(selections4);

			vector<string> relations4;
			relations4.push_back("Follows(16, s1)");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			results = handler.queryRec(tree4);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("17"), results[0]);



			/**
		 	 *	stmt s1;
			 *	Select s1 such that Follows(s1, 8);
			 *
			 *	stmt# 8 is the first stmt in its nesting level
			 */
			QueryTree* tree5 = new QueryTree();

			vector<string> declarations5;
			declarations5.push_back("stmt s1");
			tree5->setSymbolTable(declarations5);

			vector<string> selections5;
			selections5.push_back("s1");
			tree5->setResult(selections5);

			vector<string> relations5;
			relations5.push_back("Follows(s1, 8)");
			tree5->setSuchThat(relations5);

			tree5->setPattern(emptyVector);

			results = handler.queryRec(tree5);
			Assert::AreEqual(0, int(results.size()));



			/**
			 *	stmt s1;
			 *	Select s1 such that Follows(s1, 13);
		  	 */
			QueryTree* tree6 = new QueryTree();

			vector<string> declarations6;
			declarations6.push_back("stmt s1");
			tree6->setSymbolTable(declarations6);

			vector<string> selections6;
			selections6.push_back("s1");
			tree6->setResult(selections6);

			vector<string> relations6;
			relations6.push_back("Follows(s1, 13)");
			tree6->setSuchThat(relations6);

			tree6->setPattern(emptyVector);

			results = handler.queryRec(tree6);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("12"), results[0]);
		}



		TEST_METHOD(Processor_FollowsStar) {
			vector<string> code = {
				"procedure First{",
				"x=1;",			// 1
				"z=3;",			// 2
				"while x{",		// 3  parent
				"x=x-1;",		// 4
				"while x{",		// 5  p
				"y=x-1;",		// 6
				"while y{",		// 7  p
				"x=x+2;",		// 8
				"while y{",		// 9  p
				"y=1+1;",		// 10
				"while x{",		// 11 p
				"x=2;",			// 12
				"x=x+1;",		// 13
				"y=x-1;",		// 14
				"z=x+y;",		// 15
				"y=y+1;}}}}}}",	// 16
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
			*	stmt s1,s2;
			*	Select s1 such that Follows*(s1, s2);
			*/
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("stmt s1,s2");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("s1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Follows*(s1, s2)");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(10, int(results.size()));
			Assert::AreEqual(string("1"), results[0]);
			Assert::AreEqual(string("2"), results[1]);
			Assert::AreEqual(string("4"), results[2]);
			Assert::AreEqual(string("6"), results[3]);
			Assert::AreEqual(string("8"), results[4]);
			Assert::AreEqual(string("10"), results[5]);
			Assert::AreEqual(string("12"), results[6]);
			Assert::AreEqual(string("13"), results[7]);
			Assert::AreEqual(string("14"), results[8]);
			Assert::AreEqual(string("15"), results[9]);



			/**
			*	stmt s1,s2;
			*	Select s2 such that Follows*(s1, s2);
			*/
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("stmt s1,s2");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("s2");
			tree2->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Follows*(s1, s2)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			// For Tue
			// This seems to be returning the stmt for s1, not s2.
			results = handler.queryRec(tree1);
			/*Assert::AreEqual(10, int(results.size()));
			Assert::AreEqual(string("2"), results[0]);
			Assert::AreEqual(string("3"), results[1]);
			Assert::AreEqual(string("5"), results[2]);
			Assert::AreEqual(string("7"), results[3]);
			Assert::AreEqual(string("9"), results[4]);
			Assert::AreEqual(string("11"), results[5]);
			Assert::AreEqual(string("13"), results[6]);
			Assert::AreEqual(string("14"), results[7]);
			Assert::AreEqual(string("15"), results[8]);
			Assert::AreEqual(string("16"), results[9]);*/



			/**
			*	stmt s1;
			*	Select s1 such that Follows*(16, s1);
			*
			*	stmt# 16 is the last statement at its nesting level
			*/
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree1->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Follows*(16, s1)");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);
			Assert::AreEqual(0, int(results.size()));



			/**
			*	stmt s1;
			*	Select s1 such that Follows*(13, s1);
			*/
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("stmt s1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree1->setResult(selections4);

			vector<string> relations4;
			relations4.push_back("Follows*(13, s1)");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			results = handler.queryRec(tree4);
			Assert::AreEqual(3, int(results.size()));
			Assert::AreEqual(string("14"), results[0]);
			Assert::AreEqual(string("15"), results[1]);
			Assert::AreEqual(string("16"), results[2]);



			/**
			*	stmt s1;
			*	Select s1 such that Follows(s1, 12);
			*
			*	stmt# 12 is the first stmt in its nesting level
			*/
			QueryTree* tree5 = new QueryTree();

			vector<string> declarations5;
			declarations5.push_back("stmt s1");
			tree5->setSymbolTable(declarations5);

			vector<string> selections5;
			selections5.push_back("s1");
			tree1->setResult(selections5);

			vector<string> relations5;
			relations5.push_back("Follows*(s1, 12)");
			tree5->setSuchThat(relations5);

			tree5->setPattern(emptyVector);

			results = handler.queryRec(tree5);
			Assert::AreEqual(0, int(results.size()));



			/**
			*	stmt s1;
			*	Select s1 such that Follows(s1, 15);
			*/
			QueryTree* tree6 = new QueryTree();

			vector<string> declarations6;
			declarations6.push_back("stmt s1");
			tree6->setSymbolTable(declarations6);

			vector<string> selections6;
			selections6.push_back("s1");
			tree1->setResult(selections6);

			vector<string> relations6;
			relations6.push_back("Follows*(s1, 15)");
			tree6->setSuchThat(relations6);

			tree6->setPattern(emptyVector);

			results = handler.queryRec(tree6);
			Assert::AreEqual(3, int(results.size()));
			Assert::AreEqual(string("14"), results[0]);
			Assert::AreEqual(string("13"), results[1]);
			Assert::AreEqual(string("12"), results[2]);
		}



		TEST_METHOD(Processor_Parent) {
			vector<string> code = {
				"procedure First{",
				"x=1;",			// 1
				"z=3;",			// 2
				"while x{",		// 3  parent
				"x=x-1;",		// 4
				"while x{",		// 5  p
				"y=x-1;",		// 6
				"while y{",		// 7  p
				"x=x+2;",		// 8
				"while y{",		// 9  p
				"y=1+1;",		// 10
				"while x{",		// 11 p
				"x=2;}}}}}}",	// 12
				"procedure Second{",
				"a=1;",			// 13
				"while a{",		// 14 p
				"b=1+a;",		// 15
				"while a{",		// 16 p
				"b=b-1;}}}",	// 17
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
			 *	stmt s1,s2;
			 *	Select s1 such that Parent(s1, s2);
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("stmt s1,s2");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("s1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Parent(s1, s2)");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(7, int(results.size()));
			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("7"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("11"), results[4]);
			Assert::AreEqual(string("14"), results[5]);
			Assert::AreEqual(string("16"), results[6]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Parent(8, s1);
			 *
			 *	Note: stmt# 8 is not a container statement
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("stmt s1");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("s1");
			tree1->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Parent(8, s1)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			results = handler.queryRec(tree2);
			Assert::AreEqual(0, int(results.size()));



			/**
			 *	stmt s1;
			 *	Select s1 such that Parent(7, s1);
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree1->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Parent(7, s1)");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);
			Assert::AreEqual(2, int(results.size()));
			Assert::AreEqual(string("8"), results[0]);
			Assert::AreEqual(string("9"), results[1]);



			/**
		 	 *	stmt s1;
			 *	Select s1 such that Parent(s1, 8);
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("stmt s1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree1->setResult(selections4);

			vector<string> relations4;
			relations4.push_back("Parent(s1, 8)");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			results = handler.queryRec(tree4);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
		}



		TEST_METHOD(Processor_ParentStar) {
			vector<string> code = {
				"procedure First{",
				"x=1;",			// 1
				"z=3;",			// 2
				"while x{",		// 3  parent
				"x=x-1;",		// 4
				"while x{",		// 5  p
				"y=x-1;",		// 6
				"while y{",		// 7  p
				"x=x+2;",		// 8
				"while y{",		// 9  p
				"y=1+1;",		// 10
				"while x{",		// 11 p
				"x=2;}}}}}}",	// 12
				"procedure Second{",
				"a=1;",			// 13
				"while a{",		// 14 p
				"b=1+a;",		// 15
				"while a{",		// 16 p
				"b=b-1;}}}",	// 17
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;


			/**
			 *	stmt s1,s2;
			 *	Select s1 such that Parent*(s1, s2);
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("stmt s1,s2");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("s1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Parent*(s1, s2)");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(7, int(results.size()));
			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("7"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("11"), results[4]);
			Assert::AreEqual(string("14"), results[5]);
			Assert::AreEqual(string("16"), results[6]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Parent*(s1, 8);
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("stmt s1");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("s1");
			tree2->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Parent*(s1, 8)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			results = handler.queryRec(tree2);

			Assert::AreEqual(3, int(results.size()));
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("7"), results[2]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Parent*(8, s1);
			 *
			 *	Note: stmt# 8 is not a container statement, hence invalid query
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree3->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Parent*(8, s1)");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);

			Assert::AreEqual(0, int(results.size()));



			/**
			 *	stmt s1;
			 *	Select s1 such that Parent*(7, s1);
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("stmt s1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree4->setResult(selections4);

			vector<string> relations4;
			relations4.push_back("Parent*(7, s1)");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			results = handler.queryRec(tree4);

			// For Tue
			// Expected: 8 9 10 11 12
			// Missing:       ^  ^  ^
			Assert::AreEqual(5, int(results.size()));
			Assert::AreEqual(string("8"), results[0]);
			Assert::AreEqual(string("9"), results[1]);
			Assert::AreEqual(string("10"), results[2]);
			Assert::AreEqual(string("11"), results[3]);
			Assert::AreEqual(string("12"), results[4]);
		}



		TEST_METHOD(Processor_Modifies) {
			vector<string> code = {
				"procedure First{",
				"x=3;",				// 1
				"y=2;",				// 2
				"z=1;",				// 3
				"a=x+y;",			// 4
				"b=x+y+z+a+5;",		// 5
				"c=a+b;",			// 6
				"call Second;",		// 7
				"while y{",			// 8
				"y=y-1;",			// 9
				"z=y+x;",			// 10
				"b=y+z;}",			// 11
				"while x{",			// 12
				"x=x-1;}}", 		// 13
				"procedure Second{",
				"x=4+x;",			// 14
				"y=1+y;",			// 15
				"a=a-1;",			// 16
				"while x{",			// 17
				"z=x-1;",			// 18
				"call Third;}}",	// 19
				"procedure Third{",
				"z=x+1;",			// 20
				"i=x+y;",			// 21
				"while i{",			// 22
				"while x{",			// 23
				"z=i-1;",			// 24
				"j=1+1;",			// 25
				"x=1+1;}}}",		// 26
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
			 *	assign a1;
			 *	Select a1 such that Modifies(a1, "x");
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("assign a1");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("a1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Modifies(a1, \"x\")");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(4, int(results.size()));
			Assert::AreEqual(string("1"), results[0]);
			Assert::AreEqual(string("13"), results[1]);
			Assert::AreEqual(string("14"), results[2]);
			Assert::AreEqual(string("26"), results[3]);



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(5, v1);
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("variable v1");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("v1");
			tree2->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Modifies(5, v1)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			results = handler.queryRec(tree2);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("b"), results[0]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Modifies(s1, "z");
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree3->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Modifies(s1, \"z\")");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(11, int(results.size()));
			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("8"), results[2]);
			Assert::AreEqual(string("10"), results[3]);
			Assert::AreEqual(string("17"), results[4]);
			Assert::AreEqual(string("18"), results[5]);
			Assert::AreEqual(string("19"), results[6]);
			Assert::AreEqual(string("20"), results[7]);
			Assert::AreEqual(string("22"), results[8]);
			Assert::AreEqual(string("23"), results[9]);
			Assert::AreEqual(string("24"), results[10]);



			/**
			 *	while w1;
			 *	Select w1 such that Modifies(w1, "x");
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("while w1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("w1");
			tree4->setResult(selections4);

			vector<string> relations4;
			relations4.push_back("Modifies(w1, \"x\")");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			// For Tue
			// Expected: 12 17 22 23
			// Missing:      ^
			results = handler.queryRec(tree4);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(4, int(results.size()));
			Assert::AreEqual(string("12"), results[0]);
			Assert::AreEqual(string("17"), results[1]);
			Assert::AreEqual(string("22"), results[2]);
			Assert::AreEqual(string("23"), results[3]);



			/**
			 *	---
			 *  Repeat of Modifies Test 2
			 *  ---
			 */
			/*QueryTree* tree5 = new QueryTree();

			vector<string> declarations5;
			declarations5.push_back("variable v1");
			tree5->setSymbolTable(declarations5);

			vector<string> selections5;
			selections5.push_back("v1");
			tree5->setResult(selections5);

			vector<string> relations5;
			relations5.push_back("Modifies(5, v1)");
			tree5->setSuchThat(relations5);

			tree5->setPattern(emptyVector);

			results = handler.queryRec(tree5);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("b"), results[0]);
			*/



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(8, v1);
			 *
			 *	stmt# 8 is a while stmt
			 */
			QueryTree* tree6 = new QueryTree();

			vector<string> declarations6;
			declarations6.push_back("variable v1");
			tree6->setSymbolTable(declarations6);

			vector<string> selections6;
			selections6.push_back("v1");
			tree6->setResult(selections6);

			vector<string> relations6;
			relations6.push_back("Modifies(8, v1)");
			tree6->setSuchThat(relations6);

			tree6->setPattern(emptyVector);

			results = handler.queryRec(tree6);
			sort(results.begin(), results.end());
			Assert::AreEqual(3, int(results.size()));
			Assert::AreEqual(string("b"), results[0]);
			Assert::AreEqual(string("y"), results[1]);
			Assert::AreEqual(string("z"), results[2]);



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(17, v1);
			 *
			 *	stmt# 17 is a while stmt with another nested call-stmt
			 */
			QueryTree* tree7 = new QueryTree();

			vector<string> declarations7;
			declarations7.push_back("variable v1");
			tree7->setSymbolTable(declarations7);

			vector<string> selections7;
			selections7.push_back("v1");
			tree7->setResult(selections7);

			vector<string> relations7;
			relations7.push_back("Modifies(17, v1)");
			tree7->setSuchThat(relations7);

			tree7->setPattern(emptyVector);

			results = handler.queryRec(tree7);
			sort(results.begin(), results.end());
			Assert::AreEqual(4, int(results.size()));
			Assert::AreEqual(string("i"), results[0]);
			Assert::AreEqual(string("j"), results[1]);
			Assert::AreEqual(string("x"), results[2]);
			Assert::AreEqual(string("z"), results[3]);



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(22, v1);
			 *
			 *	stmt# 22 is a nested-while statement
			 */
			QueryTree* tree8 = new QueryTree();

			vector<string> declarations8;
			declarations8.push_back("variable v1");
			tree8->setSymbolTable(declarations8);

			vector<string> selections8;
			selections8.push_back("v1");
			tree8->setResult(selections8);

			vector<string> relations8;
			relations8.push_back("Modifies(22, v1)");
			tree8->setSuchThat(relations8);

			tree8->setPattern(emptyVector);

			results = handler.queryRec(tree8);
			sort(results.begin(), results.end());
			Assert::AreEqual(3, int(results.size()));
			Assert::AreEqual(string("j"), results[0]);
			Assert::AreEqual(string("x"), results[1]);
			Assert::AreEqual(string("z"), results[2]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Modifies(s1, "i");
			 *
			 *	"i" is a variable in nested-procedure calls
			 */
			QueryTree* tree9 = new QueryTree();

			vector<string> declarations9;
			declarations9.push_back("stmt s1");
			tree9->setSymbolTable(declarations9);

			vector<string> selections9;
			selections9.push_back("s1");
			tree9->setResult(selections9);

			vector<string> relations9;
			relations9.push_back("Modifies(s1, \"j\")");
			tree9->setSuchThat(relations9);

			tree9->setPattern(emptyVector);

			results = handler.queryRec(tree9);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("17"), results[1]);
			Assert::AreEqual(string("19"), results[2]);
			Assert::AreEqual(string("22"), results[3]);
			Assert::AreEqual(string("23"), results[4]);
			Assert::AreEqual(string("25"), results[5]);



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(7, v1);
			 *
			 *	stmt# 7 is the procedure call
			 */
			QueryTree* tree10 = new QueryTree();

			vector<string> declarations10;
			declarations10.push_back("variable v1");
			tree10->setSymbolTable(declarations10);

			vector<string> selections10;
			selections10.push_back("v1");
			tree10->setResult(selections10);

			vector<string> relations10;
			relations10.push_back("Modifies(7, v1)");
			tree10->setSuchThat(relations10);

			tree10->setPattern(emptyVector);

			results = handler.queryRec(tree10);
			sort(results.begin(), results.end());
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("a"), results[0]);
			Assert::AreEqual(string("i"), results[1]);
			Assert::AreEqual(string("j"), results[2]);
			Assert::AreEqual(string("x"), results[3]);
			Assert::AreEqual(string("y"), results[4]);
			Assert::AreEqual(string("z"), results[5]);
		}



		TEST_METHOD(Processor_Uses) {
			vector<string> code = {
				"procedure First{",
				"x=3;",				// 1
				"y=2;",				// 2
				"z=1;",				// 3
				"a=x+y;",			// 4
				"b=x+y+z+a+5;",		// 5
				"c=a+b;",			// 6
				"call Second;",		// 7
				"while y{",			// 8
				"y=y-1;",			// 9
				"z=y+x;",			// 10
				"b=y+z;}",			// 11
				"while x{",			// 12
				"x=x-1;}}", 		// 13
				"procedure Second{",
				"x=4+x;",			// 14
				"y=1+y;",			// 15
				"a=a-1;",			// 16
				"while a{",			// 17
				"z=x-1;",			// 18
				"call Third;}}",	// 19
				"procedure Third{",
				"z=x+1;",			// 20
				"k=x+y;",			// 21
				"while i{",			// 22
				"while k{",			// 23
				"z=i-1;",			// 24
				"j=z+1;",			// 25
				"x=1+1;}}}",		// 26
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
		  	 *	assign a1;
			 *	Select a1 such that Uses(a1, "x");
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("assign a1");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("a1");
			tree1->setResult(selections1);

			vector<string> relations1;
			relations1.push_back("Uses(a1, \"x\")");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);

			results = handler.queryRec(tree1);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(8, int(results.size()));
			Assert::AreEqual(string("4"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("10"), results[2]);
			Assert::AreEqual(string("13"), results[3]);
			Assert::AreEqual(string("14"), results[4]);
			Assert::AreEqual(string("18"), results[5]);
			Assert::AreEqual(string("20"), results[6]);
			Assert::AreEqual(string("21"), results[7]);



			/**
			 *	variable v1;
			 *	Select v1 such that Uses(5, v1);
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("variable v1");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("v1");
			tree2->setResult(selections2);

			vector<string> relations2;
			relations2.push_back("Uses(5, v1)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);

			results = handler.queryRec(tree2);
			sort(results.begin(), results.end());
			Assert::AreEqual(4, int(results.size()));
			Assert::AreEqual(string("a"), results[0]);
			Assert::AreEqual(string("x"), results[1]);
			Assert::AreEqual(string("y"), results[2]);
			Assert::AreEqual(string("z"), results[3]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Uses(s1, \"z\");
			 *
			 *	single + nested while-stmt, and nested procedure calls
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("stmt s1");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree3->setResult(selections3);

			vector<string> relations3;
			relations3.push_back("Uses(s1, \"z\")");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);

			results = handler.queryRec(tree3);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(9, int(results.size()));
			Assert::AreEqual(string("5"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("8"), results[2]);
			Assert::AreEqual(string("11"), results[3]);
			Assert::AreEqual(string("17"), results[4]);
			Assert::AreEqual(string("19"), results[5]);
			Assert::AreEqual(string("22"), results[6]);
			Assert::AreEqual(string("23"), results[7]);
			Assert::AreEqual(string("25"), results[8]);



			/**
			 *	stmt s1;
			 *	Select s1 such that Uses(s1, \"k\");
			 *
			 *	Check control variable on stmt# 23
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("stmt s1");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree4->setResult(selections4);

			vector<string> relations4;
			relations4.push_back(" Uses(s1, \"k\")");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);

			// For Tue
			// Control variables for while are not caught
			// See test5 for possible reason
			results = handler.queryRec(tree4);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(5, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("17"), results[1]);
			Assert::AreEqual(string("19"), results[2]);
			Assert::AreEqual(string("22"), results[3]);
			Assert::AreEqual(string("23"), results[4]);



			/**
			 *	variable v1;
			 *	Select v1 such that Uses(17, v1)
			 *
			 *	while stmt with nested-call with nested-while
			 */

			QueryTree* tree5 = new QueryTree();

			vector<string> declarations5;
			declarations5.push_back("variable v1");
			tree5->setSymbolTable(declarations5);

			vector<string> selections5;
			selections5.push_back("v1");
			tree5->setResult(selections5);

			vector<string> relations5;
			relations5.push_back("Uses(17, v1)");
			tree5->setSuchThat(relations5);

			tree5->setPattern(emptyVector);

			// For Tue
			// Expected:  "a",  "i",  "k", "x", "y", "z"
			// Results:  " a", " i", " k", "i", "x", "y", "z"
			//
			// Control variables are not trimmed, so "while x" is
			// stored as " x" instead of "x"
			results = handler.queryRec(tree5);
			sort(results.begin(), results.end());
			Assert::AreEqual(7, int(results.size()));
			Assert::AreEqual(string("a"), results[0]);
			Assert::AreEqual(string("i"), results[1]);
			Assert::AreEqual(string("k"), results[2]);
			Assert::AreEqual(string("x"), results[3]);
			Assert::AreEqual(string("y"), results[4]);
			Assert::AreEqual(string("z"), results[5]);
		}



		TEST_METHOD(Processor_Calls) {
            vector<string> code = {
                "procedureAA{",
                "a=1;",             // 1
                "b=2;",             // 2
                "call AB;",         // 3
                "call BA;}",        // 4

                "procedureAB{",
                "call AC;}",        // 5

                "procedureAC{",
                "call AD;}",        // 6

                "procedureAD{",
                "call AE;}",        // 7

                "procedureAE{",
                "call AF;}",        // 8

                "procedureAF{",
                "b=10;}",           // 9

                "procedureBA{",
                "if a then{",       // 10
                "call BB;}",        // 11
                "else{",
                "call BC;}",        // 12
                "while b{",         // 13
                "call BD;}}",       // 14

                "procedureBB{",
                "c=a-1;}",          // 15

                "procedureBC{",
                "d=b-1;}",          // 11

                "procedureBD{",
                "while a{",         // 17
                "call BE;}}",       // 18

                "procedureBE{",
                "if a then{",       // 19
                "a=a-1;}",          // 20
                "else{",
                "call BF;}}",       // 21

                "procedureBF{",
                "f=a+b+c+d+e;",     // 22
                "call AE;}"         // 23
            };

            DesignExtractor ext = DesignExtractor(code);

            // Variables that will be reused in different queries
            QueryHandler handler;
            vector<string> results;
            vector<string> emptyVector;



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls(p1, p2);
             */
            QueryTree* tree1 = new QueryTree();

            vector<string> declarations1;
            declarations1.push_back("procedure p1,p2");
            tree1->setSymbolTable(declarations1);

            vector<string> selections1;
            selections1.push_back("p1");
            tree1->setResult(selections1);

            vector<string> relations1;
            relations1.push_back("Calls(p1, p2)");
            tree1->setSuchThat(relations1);

            tree1->setPattern(emptyVector);

            results = handler.queryRec(tree1);
            sort(results.begin(), results.end());
            Assert::AreEqual(9, int(results.size()));
            Assert::AreEqual(string("AA"), results[0]);
            Assert::AreEqual(string("AB"), results[1]);
            Assert::AreEqual(string("AC"), results[2]);
            Assert::AreEqual(string("AD"), results[3]);
            Assert::AreEqual(string("AE"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BD"), results[6]);
            Assert::AreEqual(string("BE"), results[7]);
            Assert::AreEqual(string("BF"), results[8]);



            /**
             *  procedure p1, p2;
             *  Select p2 such that Calls(p1, p2);
             */
            QueryTree* tree2 = new QueryTree();

            vector<string> declarations2;
            declarations2.push_back("procedure p1,p2");
            tree2->setSymbolTable(declarations2);

            vector<string> selections2;
            selections2.push_back("p2");
            tree1->setResult(selections2);

            vector<string> relations2;
            relations2.push_back("Calls(p1, p2)");
            tree2->setSuchThat(relations2);

            tree2->setPattern(emptyVector);

            results = handler.queryRec(tree2);
            sort(results.begin(), results.end());
            /*Assert::AreEqual(11, int(results.size()));
            Assert::AreEqual(string("AB"), results[0]);
            Assert::AreEqual(string("AC"), results[1]);
            Assert::AreEqual(string("AD"), results[2]);
            Assert::AreEqual(string("AE"), results[3]);
            Assert::AreEqual(string("AF"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BB"), results[3]);
            Assert::AreEqual(string("BC"), results[7]);
            Assert::AreEqual(string("BD"), results[8]);
            Assert::AreEqual(string("BE"), results[9]);
            Assert::AreEqual(string("BF"), results[10]);*/



            /**
             *  procedure p1;
             *  Select p2 such that Calls(p1, "AB");
             *
             *  Basic case
             */
             QueryTree* tree3 = new QueryTree();

             vector<string> declarations3;
             declarations3.push_back("procedure p1,p2");
             tree3->setSymbolTable(declarations3);

             vector<string> selections3;
             selections3.push_back("p1");
             tree1->setResult(selections3);

             vector<string> relations3;
             relations3.push_back("Calls(p1, \"AB\")");
             tree3->setSuchThat(relations3);

             tree3->setPattern(emptyVector);

             results = handler.queryRec(tree3);
             sort(results.begin(), results.end());
             Assert::AreEqual(1, int(results.size()));
             Assert::AreEqual(string("AA"), results[0]);



            /**
             *  procedure p2;
             *  Select p2 such that Calls("AA", p2);
             *
             *  Basic case
             */
            QueryTree* tree4 = new QueryTree();

            vector<string> declarations4;
            declarations4.push_back("procedure p1,p2");
            tree4->setSymbolTable(declarations4);

            vector<string> selections4;
            selections4.push_back("p2");
            tree1->setResult(selections4);

            vector<string> relations4;
            relations4.push_back("Calls(\"AA\", p2)");
            tree4->setSuchThat(relations4);

            tree4->setPattern(emptyVector);

            results = handler.queryRec(tree4);
            sort(results.begin(), results.end());
            Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string("AB"), results[0]);
            Assert::AreEqual(string("BA"), results[1]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls(p1, "AE");
             *
             *  2 separate branches of call-stmts calls "AE"
             */
            QueryTree* tree5 = new QueryTree();

            vector<string> declarations5;
            declarations5.push_back("procedure p1,p2");
            tree5->setSymbolTable(declarations5);

            vector<string> selections5;
            selections5.push_back("p1");
            tree1->setResult(selections5);

            vector<string> relations5;
            relations5.push_back("Calls(p1, \"AE\")");
            tree5->setSuchThat(relations5);

            tree5->setPattern(emptyVector);

            results = handler.queryRec(tree5);
            sort(results.begin(), results.end());
            Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string("AD"), results[0]);
            Assert::AreEqual(string("BF"), results[1]);



            /**
             *  procedure p1, p2;
             *  Select p2 such that Calls("Seventh", p2);
             *
             *  "BA" has 3 call-stmts inside containers
             */
            QueryTree* tree6 = new QueryTree();

            vector<string> declarations6;
            declarations6.push_back("procedure p1,p2");
            tree6->setSymbolTable(declarations6);

            vector<string> selections6;
            selections6.push_back("p2");
            tree1->setResult(selections6);

            vector<string> relations6;
            relations6.push_back("Calls(\"BA\", p2)");
            tree6->setSuchThat(relations6);

            tree6->setPattern(emptyVector);

            results = handler.queryRec(tree6);
            sort(results.begin(), results.end());
            Assert::AreEqual(3, int(results.size()));
            Assert::AreEqual(string("BB"), results[0]);
            Assert::AreEqual(string("BC"), results[1]);
            Assert::AreEqual(string("BD"), results[2]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls(p1, "Sixth");
             *
             *  Deeply nested call-stmts
             */
            QueryTree* tree7 = new QueryTree();

            vector<string> declarations7;
            declarations7.push_back("procedure p1,p2");
            tree7->setSymbolTable(declarations7);

            vector<string> selections7;
            selections7.push_back("p1");
            tree1->setResult(selections7);

            vector<string> relations7;
            relations7.push_back("Calls(p1, \"AF\")");
            tree7->setSuchThat(relations7);

            tree7->setPattern(emptyVector);

            results = handler.queryRec(tree7);
            sort(results.begin(), results.end());
            Assert::AreEqual(1, int(results.size()));
            Assert::AreEqual(string("AE"), results[0]);
        }



        TEST_METHOD(Processor_CallsStar) {
        	vector<string> code = {
        	    "procedure AA{",
        	    "a=1;",             // 1
        	    "b=2;",             // 2
        	    "call AB;",         // 3
        	    "call BA;}",        // 4

        	    "procedure AB{",
        	    "call AC;}",        // 5

        	    "procedure AC{",
        	    "call AD;}",        // 1

        	    "procedure AD{",
        	    "call AE;}",        // 7

        	    "procedure AE{",
        	    "call AF;}",        // 8

        	    "procedure AF{",
        	    "b=10;}",           // 9

        	    "procedure BA{",
        	    "if a then{",       // 10
        	    "call BB;}",        // 11
        	    "else{",
        	    "call BC;}",        // 12
        	    "while b{",         // 13
        	    "call BD;}}",       // 14

        	    "procedure BB{",
        	    "c=a-1;}",          // 15

        	    "procedure BC{",
        	    "d=b-1;}",          // 11

        	    "procedure BD{",
        	    "while a{",         // 17
        	    "call BE;}}",        // 18

        	    "procedure BE{",
        	    "if a then{",       // 19
        	    "a=a-1;}",          // 20
        	    "else{",
        	    "call BF;}}",       // 21

        	    "procedure BF{",
        	    "f=a+b+c+d+e;",     // 22
        	    "call AE;}"         // 23
        	};

        	DesignExtractor ext = DesignExtractor(code);

        	// Variables that will be reused in different queries
        	QueryHandler handler;
        	vector<string> results;
        	vector<string> emptyVector;



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls*(p1, p2);
             *
             *	Basic case
             */
            QueryTree* tree1 = new QueryTree();

            vector<string> declarations1;
            declarations1.push_back("procedure p1,p2");
            tree1->setSymbolTable(declarations1);

            vector<string> selections1;
            selections1.push_back("p1");
            tree1->setResult(selections1);

            vector<string> relations1;
            relations1.push_back("Calls*(p1, p2)");
            tree1->setSuchThat(relations1);

            tree1->setPattern(emptyVector);

            results = handler.queryRec(tree1);
            sort(results.begin(), results.end());
            Assert::AreEqual(9, int(results.size()));
            Assert::AreEqual(string("AA"), results[0]);
            Assert::AreEqual(string("AB"), results[1]);
            Assert::AreEqual(string("AC"), results[2]);
            Assert::AreEqual(string("AD"), results[3]);
            Assert::AreEqual(string("AE"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BD"), results[6]);
            Assert::AreEqual(string("BE"), results[7]);
            Assert::AreEqual(string("BF"), results[8]);



            /**
             *  procedure p1, p2;
             *  Select p2 such that Calls*(p1, p2);
             *
             *	Basic case
             */
            QueryTree* tree2 = new QueryTree();

            vector<string> declarations2;
            declarations2.push_back("procedure p1,p2");
            tree2->setSymbolTable(declarations2);

            vector<string> selections2;
            selections2.push_back("p2");
            tree1->setResult(selections2);

            vector<string> relations2;
            relations2.push_back("Calls*(p1, p2)");
            tree2->setSuchThat(relations2);

            tree2->setPattern(emptyVector);

            results = handler.queryRec(tree2);
            sort(results.begin(), results.end());
            Assert::AreEqual(11, int(results.size()));
            Assert::AreEqual(string("AB"), results[0]);
            Assert::AreEqual(string("AC"), results[1]);
            Assert::AreEqual(string("AD"), results[2]);
            Assert::AreEqual(string("AE"), results[3]);
            Assert::AreEqual(string("AF"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BB"), results[6]);
            Assert::AreEqual(string("BC"), results[7]);
            Assert::AreEqual(string("BD"), results[8]);
            Assert::AreEqual(string("BE"), results[9]);
            Assert::AreEqual(string("BF"), results[10]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls*("AB", p2);
             *
             *	Basic case on nested call-stmts
             */
            QueryTree* tree3 = new QueryTree();

            vector<string> declarations3;
            declarations3.push_back("procedure p1,p2");
            tree3->setSymbolTable(declarations3);

            vector<string> selections3;
            selections3.push_back("p2");
            tree1->setResult(selections3);

            vector<string> relations3;
            relations3.push_back("Calls*(\"AB\", p2)");
            tree3->setSuchThat(relations3);

            tree3->setPattern(emptyVector);

            results = handler.queryRec(tree3);
            sort(results.begin(), results.end());
            Assert::AreEqual(4, int(results.size()));
            Assert::AreEqual(string("AC"), results[0]);
            Assert::AreEqual(string("AD"), results[1]);
            Assert::AreEqual(string("AE"), results[2]);
            Assert::AreEqual(string("AF"), results[3]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls*("BA", p2);
             *
             *	Basic case on nested container stmts
             */
            QueryTree* tree4 = new QueryTree();

            vector<string> declarations4;
            declarations4.push_back("procedure p1,p2");
            tree4->setSymbolTable(declarations4);

            vector<string> selections4;
            selections4.push_back("p2");
            tree1->setResult(selections4);

            vector<string> relations4;
            relations4.push_back("Calls*(\"BA\", p2)");
            tree4->setSuchThat(relations4);

            tree4->setPattern(emptyVector);

            results = handler.queryRec(tree4);
            sort(results.begin(), results.end());
            Assert::AreEqual(7, int(results.size()));
            Assert::AreEqual(string("BB"), results[0]);
            Assert::AreEqual(string("BC"), results[1]);
            Assert::AreEqual(string("BD"), results[2]);
            Assert::AreEqual(string("BE"), results[3]);
            Assert::AreEqual(string("BF"), results[4]);
            Assert::AreEqual(string("AE"), results[5]);
            Assert::AreEqual(string("AF"), results[6]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls*("AA", p2);
             *
             * 	Entire code tree
             */
            QueryTree* tree5 = new QueryTree();

            vector<string> declarations5;
            declarations5.push_back("procedure p1,p2");
            tree5->setSymbolTable(declarations5);

            vector<string> selections5;
            selections5.push_back("p2");
            tree1->setResult(selections5);

            vector<string> relations5;
            relations5.push_back("Calls*(\"AA\", p2)");
            tree5->setSuchThat(relations5);

            tree5->setPattern(emptyVector);

            results = handler.queryRec(tree5);
            sort(results.begin(), results.end());
            Assert::AreEqual(11, int(results.size()));
            Assert::AreEqual(string("AB"), results[0]);
            Assert::AreEqual(string("AC"), results[1]);
            Assert::AreEqual(string("AD"), results[2]);
            Assert::AreEqual(string("AE"), results[3]);
            Assert::AreEqual(string("AF"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BB"), results[6]);
            Assert::AreEqual(string("BC"), results[7]);
            Assert::AreEqual(string("BD"), results[8]);
            Assert::AreEqual(string("BE"), results[9]);
            Assert::AreEqual(string("BF"), results[10]);



            /**
             *  procedure p1, p2;
             *  Select p1 such that Calls*(p1, "AF");
             *
             *	Entire code tree from the other side
             */
            QueryTree* tree6 = new QueryTree();

            vector<string> declarations6;
            declarations6.push_back("procedure p1,p2");
            tree6->setSymbolTable(declarations6);

            vector<string> selections6;
            selections6.push_back("p1");
            tree1->setResult(selections6);

            vector<string> relations6;
            relations6.push_back("Calls*(p1, \"AF\")");
            tree6->setSuchThat(relations6);

            tree6->setPattern(emptyVector);

            results = handler.queryRec(tree6);
            sort(results.begin(), results.end());
            Assert::AreEqual(11, int(results.size()));
            Assert::AreEqual(string("AA"), results[0]);
            Assert::AreEqual(string("AB"), results[1]);
            Assert::AreEqual(string("AC"), results[2]);
            Assert::AreEqual(string("AD"), results[3]);
            Assert::AreEqual(string("AE"), results[4]);
            Assert::AreEqual(string("BA"), results[5]);
            Assert::AreEqual(string("BB"), results[6]);
            Assert::AreEqual(string("BC"), results[7]);
            Assert::AreEqual(string("BD"), results[8]);
            Assert::AreEqual(string("BE"), results[9]);
            Assert::AreEqual(string("BF"), results[10]);
        }



        TEST_METHOD(Processor_Next) {
			vector<string> code = {
				"procedure First{",
				"a=1;",				// 1
				"b=1;",				// 2
				"if a then{",		// 3
				"a=a-1;}",			// 4
				"else{",
				"a=2;}",			// 5
				"c=3;",				// 6
				"while c{",			// 7
				"a=1;",				// 8
				"while c{",			// 9
				"while c{",			// 10
				"c=c-1;",			// 11
				"call Second;}}}",	// 12
				"a=a+c;",			// 13
				"Procedure Second{",
				"a=a+1;}"			// 14
			};


            DesignExtractor ext = DesignExtractor(code);

            // Variables that will be reused in different queries
            QueryHandler handler;
            vector<string> results;
            vector<string> emptyVector;



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(n1, n2);
             *
             *	Basic case
             */
            QueryTree* tree1 = new QueryTree();

            vector<string> declarations1;
            declarations1.push_back("prog_line n1,n2");
            tree1->setSymbolTable(declarations1);

            vector<string> selections1;
            selections1.push_back("n1");
            tree1->setResult(selections1);

            vector<string> relations1;
            relations1.push_back("Next(n1, n2)");
            tree1->setSuchThat(relations1);

            tree1->setPattern(emptyVector);

            results = handler.queryRec(tree1);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(12, int(results.size()));
            Assert::AreEqual(string("1"), results[0]);
            Assert::AreEqual(string("2"), results[1]);
            Assert::AreEqual(string("3"), results[2]);
            Assert::AreEqual(string("4"), results[3]);
            Assert::AreEqual(string("5"), results[4]);
            Assert::AreEqual(string("6"), results[5]);
            Assert::AreEqual(string("7"), results[6]);
            Assert::AreEqual(string("8"), results[7]);
            Assert::AreEqual(string("9"), results[8]);
            Assert::AreEqual(string("10"), results[9]);
            Assert::AreEqual(string("11"), results[10]);
            Assert::AreEqual(string("12"), results[11]);



            /**
             *  prog_line n1, n2;
             *  Select n2 such that Next(n1, n2);
             *
             *	Basic case
             */
            QueryTree* tree2 = new QueryTree();

            vector<string> declarations2;
            declarations2.push_back("prog_line n1,n2");
            tree2->setSymbolTable(declarations2);

            vector<string> selections2;
            selections2.push_back("n2");
            tree1->setResult(selections2);

            vector<string> relations2;
            relations2.push_back("Next(n1, n2)");
            tree2->setSuchThat(relations2);

            tree2->setPattern(emptyVector);

            results = handler.queryRec(tree2);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            /*Assert::AreEqual(12, int(results.size()));
            Assert::AreEqual(string("2"), results[0]);
            Assert::AreEqual(string("3"), results[1]);
            Assert::AreEqual(string("4"), results[2]);
            Assert::AreEqual(string("5"), results[3]);
            Assert::AreEqual(string("6"), results[4]);
            Assert::AreEqual(string("7"), results[5]);
            Assert::AreEqual(string("8"), results[6]);
            Assert::AreEqual(string("9"), results[7]);
            Assert::AreEqual(string("10"), results[8]);
            Assert::AreEqual(string("11"), results[9]);
            Assert::AreEqual(string("12"), results[10]);
            Assert::AreEqual(string("13"), results[11]);*/



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(3, n2);
             *
             *	stmt# 3 is a if-stmt
             */
            QueryTree* tree3 = new QueryTree();

            vector<string> declarations3;
            declarations3.push_back("prog_line n1,n2");
            tree3->setSymbolTable(declarations3);

            vector<string> selections3;
            selections3.push_back("n2");
            tree1->setResult(selections3);

            vector<string> relations3;
            relations3.push_back("Next(3, n2)");
            tree3->setSuchThat(relations3);

            tree3->setPattern(emptyVector);

            results = handler.queryRec(tree3);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            /*Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string("4"), results[0]);
            Assert::AreEqual(string("5"), results[1]);*/



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(n1, 6);
             *
             *	stmt# 6 is the first stmt after a if-else
             */
            QueryTree* tree4 = new QueryTree();

            vector<string> declarations4;
            declarations4.push_back("prog_line n1,n2");
            tree4->setSymbolTable(declarations4);

            vector<string> selections4;
            selections4.push_back("n1");
            tree1->setResult(selections4);

            vector<string> relations4;
            relations4.push_back("Next(n1, 6)");
            tree4->setSuchThat(relations4);

            tree4->setPattern(emptyVector);

            results = handler.queryRec(tree4);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string("4"), results[0]);
            Assert::AreEqual(string("5"), results[1]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(7, n2);
             *
             *	stmt# 7 is the first of a series of nested-whiles
             */
            QueryTree* tree5 = new QueryTree();

            vector<string> declarations5;
            declarations5.push_back("prog_line n1,n2");
            tree5->setSymbolTable(declarations5);

            vector<string> selections5;
            selections5.push_back("n1");
            tree1->setResult(selections5);

            vector<string> relations5;
            relations5.push_back("Next(7, n2)");
            tree5->setSuchThat(relations5);

            tree5->setPattern(emptyVector);

            results = handler.queryRec(tree5);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(1, int(results.size()));
            Assert::AreEqual(string("8"), results[0]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(12, n2);
             *
             *	stmt# 12 is a call-stmt + last stmt in a nested while-container
             */
            QueryTree* tree6 = new QueryTree();

            vector<string> declarations6;
            declarations6.push_back("prog_line n1,n2");
            tree6->setSymbolTable(declarations6);

            vector<string> selections6;
            selections6.push_back("n2");
            tree1->setResult(selections6);

            vector<string> relations6;
            relations6.push_back("Next(12, n2)");
            tree6->setSuchThat(relations6);

            tree6->setPattern(emptyVector);

            results = handler.queryRec(tree6);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string("10"), results[0]);
            Assert::AreEqual(string("13"), results[1]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(13, n2);
             *
             *	stmt# 13 is the last stmt of the procedure
             */
            QueryTree* tree7 = new QueryTree();

            vector<string> declarations7;
            declarations7.push_back("prog_line n1,n2");
            tree7->setSymbolTable(declarations7);

            vector<string> selections7;
            selections7.push_back("n2");
            tree1->setResult(selections7);

            vector<string> relations7;
            relations7.push_back("Next(13, n2)");
            tree7->setSuchThat(relations7);

            tree7->setPattern(emptyVector);

            results = handler.queryRec(tree7);
            Assert::AreEqual(0, int(results.size()));
		}



        TEST_METHOD(Processor_NextStar) {
			vector<string> code = {
				"procedure First{",
				"a=1;",				// 1
				"b=1;",				// 2
				"if a then{",		// 3
				"a=a-1;}",			// 4
				"else{",
				"a=2;}",			// 5
				"c=3;",				// 6
				"while c{",			// 7
				"a=1;",				// 8
				"while c{",			// 9
				"while c{",			// 10
				"c=c-1;",			// 11
				"call Second;}}}",	// 12
				"a=a+c;}",			// 13
				"Procedure Second{",
				"a=a+1;}"			// 14
			};

			DesignExtractor ext = DesignExtractor(code);

            // Variables that will be reused in different queries
            QueryHandler handler;
            vector<string> results;
            vector<string> emptyVector;



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next*(n1, n2);
             *
             *	Basic case
             */
            QueryTree* tree1 = new QueryTree();

            vector<string> declarations1;
            declarations1.push_back("prog_line n1,n2");
            tree1->setSymbolTable(declarations1);

            vector<string> selections1;
            selections1.push_back("n1");
            tree1->setResult(selections1);

            vector<string> relations1;
            relations1.push_back("Next*(n1, n2)");
            tree1->setSuchThat(relations1);

            tree1->setPattern(emptyVector);

            results = handler.queryRec(tree1);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(12, int(results.size()));
            Assert::AreEqual(string("1"), results[0]);
            Assert::AreEqual(string("2"), results[1]);
            Assert::AreEqual(string("3"), results[2]);
            Assert::AreEqual(string("4"), results[3]);
            Assert::AreEqual(string("5"), results[4]);
            Assert::AreEqual(string("6"), results[5]);
            Assert::AreEqual(string("7"), results[6]);
            Assert::AreEqual(string("8"), results[7]);
            Assert::AreEqual(string("9"), results[8]);
            Assert::AreEqual(string("10"), results[9]);
            Assert::AreEqual(string("11"), results[10]);
            Assert::AreEqual(string("12"), results[11]);



            /**
             *  prog_line n1, n2;
             *  Select n2 such that Next*(n1, n2);
             *
             *	Basic case
             */
            QueryTree* tree2 = new QueryTree();

            vector<string> declarations2;
            declarations2.push_back("prog_line n1,n2");
            tree2->setSymbolTable(declarations2);

            vector<string> selections2;
            selections2.push_back("n2");
            tree1->setResult(selections2);

            vector<string> relations2;
            relations2.push_back("Next(n1, n2)");
            tree2->setSuchThat(relations2);

            tree2->setPattern(emptyVector);

            results = handler.queryRec(tree2);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(12, int(results.size()));
            Assert::AreEqual(string("2"), results[0]);
            Assert::AreEqual(string("3"), results[1]);
            Assert::AreEqual(string("4"), results[2]);
            Assert::AreEqual(string("5"), results[3]);
            Assert::AreEqual(string("6"), results[4]);
            Assert::AreEqual(string("7"), results[5]);
            Assert::AreEqual(string("8"), results[6]);
            Assert::AreEqual(string("9"), results[7]);
            Assert::AreEqual(string("10"), results[8]);
            Assert::AreEqual(string("11"), results[9]);
            Assert::AreEqual(string("12"), results[10]);
            Assert::AreEqual(string("13"), results[11]);


            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next*(3, n2);
             *
             *	stmt# 3 is a if-stmt
             */
            QueryTree* tree3 = new QueryTree();

            vector<string> declarations3;
            declarations3.push_back("prog_line n1,n2");
            tree3->setSymbolTable(declarations3);

            vector<string> selections3;
            selections3.push_back("n2");
            tree1->setResult(selections3);

            vector<string> relations3;
            relations3.push_back("Next*(3, n2)");
            tree3->setSuchThat(relations3);

            tree3->setPattern(emptyVector);

            results = handler.queryRec(tree3);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(11, int(results.size()));
            Assert::AreEqual(string("4"), results[0]);
            Assert::AreEqual(string("5"), results[1]);
            Assert::AreEqual(string("6"), results[2]);
            Assert::AreEqual(string("7"), results[3]);
            Assert::AreEqual(string("8"), results[4]);
            Assert::AreEqual(string("9"), results[5]);
            Assert::AreEqual(string("10"), results[6]);
            Assert::AreEqual(string("11"), results[7]);
            Assert::AreEqual(string("12"), results[8]);
            Assert::AreEqual(string("13"), results[9]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next*(n1, 6);
             *
             *	stmt# 6 is the first stmt after a if-else
             */
            QueryTree* tree4 = new QueryTree();

            vector<string> declarations4;
            declarations4.push_back("prog_line n1,n2");
            tree4->setSymbolTable(declarations4);

            vector<string> selections4;
            selections4.push_back("n1");
            tree1->setResult(selections4);

            vector<string> relations4;
            relations4.push_back("Next*(n1, 6)");
            tree4->setSuchThat(relations4);

            tree4->setPattern(emptyVector);

            results = handler.queryRec(tree4);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(5, int(results.size()));
            Assert::AreEqual(string("1"), results[0]);
            Assert::AreEqual(string("2"), results[1]);
            Assert::AreEqual(string("3"), results[2]);
            Assert::AreEqual(string("4"), results[3]);
            Assert::AreEqual(string("5"), results[4]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next*(7, n2);
             *
             *	stmt# 7 is the first of a series of nested-whiles
             */
            QueryTree* tree5 = new QueryTree();

            vector<string> declarations5;
            declarations5.push_back("prog_line n1,n2");
            tree5->setSymbolTable(declarations5);

            vector<string> selections5;
            selections5.push_back("n1");
            tree1->setResult(selections5);

            vector<string> relations5;
            relations5.push_back("Next*(7, n2)");
            tree5->setSuchThat(relations5);

            tree5->setPattern(emptyVector);

            results = handler.queryRec(tree5);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(6, int(results.size()));
            Assert::AreEqual(string("8"), results[0]);
            Assert::AreEqual(string("9"), results[1]);
            Assert::AreEqual(string("10"), results[2]);
            Assert::AreEqual(string("11"), results[3]);
            Assert::AreEqual(string("12"), results[4]);
            Assert::AreEqual(string("13"), results[5]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next(11, n2);
             *
             *	stmt# 12 is a call-stmt + last stmt in a nested while-container
             */
            QueryTree* tree6 = new QueryTree();

            vector<string> declarations6;
            declarations6.push_back("prog_line n1,n2");
            tree6->setSymbolTable(declarations6);

            vector<string> selections6;
            selections6.push_back("n2");
            tree1->setResult(selections6);

            vector<string> relations6;
            relations6.push_back("Next*(12, n2)");
            tree6->setSuchThat(relations6);

            tree6->setPattern(emptyVector);

            results = handler.queryRec(tree6);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(6, int(results.size()));
            Assert::AreEqual(string("7"), results[0]);
            Assert::AreEqual(string("8"), results[1]);
            Assert::AreEqual(string("9"), results[2]);
            Assert::AreEqual(string("10"), results[3]);
            Assert::AreEqual(string("11"), results[4]);
            Assert::AreEqual(string("13"), results[5]);



            /**
             *  prog_line n1, n2;
             *  Select n1 such that Next*(13, n2);
             *
             *	stmt# 13 is the last stmt of the procedure
             */
            QueryTree* tree7 = new QueryTree();

            vector<string> declarations7;
            declarations7.push_back("prog_line n1,n2");
            tree7->setSymbolTable(declarations7);

            vector<string> selections7;
            selections7.push_back("n2");
            tree1->setResult(selections7);

            vector<string> relations7;
            relations7.push_back("Next*(13, n2)");
            tree7->setSuchThat(relations7);

            tree7->setPattern(emptyVector);

            results = handler.queryRec(tree7);
            Assert::AreEqual(0, int(results.size()));
        }



        TEST_METHOD(Processor_Affects) {
        	vector<string> code = {
        		"procedureFirst{",
        		"a=1;",				// 1
        		"b=1;",				// 2
        		"c=1;",				// 3
        		"x=a+1;",			// 4	mod x, use a
        		"a=b+2;",			// 5	mod a, use b
        		"x=a+b;",			// 6	mod x, use a/b
        		"procedureSecond{",
        		"x=a;}"				// 7
        	};

        	DesignExtractor ext = DesignExtractor(code);

        	// Variables that will be reused in different queries
        	QueryHandler handler;
        	vector<string> results;
        	vector<string> emptyVector;



        	/**
             *	assign a1, a2
             *  Select a1 such that Affects(a1, a2);
             *
             *
             */
            QueryTree* tree1 = new QueryTree();

            vector<string> declarations1;
            declarations1.push_back("assign a1, a2");
            tree1->setSymbolTable(declarations1);

            vector<string> selections1;
            selections1.push_back("a1");
            tree1->setResult(selections1);

            vector<string> relations1;
            relations1.push_back("Affects(a1, a2)");
            tree1->setSuchThat(relations1);

            tree1->setPattern(emptyVector);

            results = handler.queryRec(tree1);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(7, int(results.size()));
            Assert::AreEqual(string("1"), results[0]);
            Assert::AreEqual(string("2"), results[1]);
            Assert::AreEqual(string("3"), results[2]);
            Assert::AreEqual(string("4"), results[3]);
            Assert::AreEqual(string("5"), results[4]);
			Assert::AreEqual(string("6"), results[5]);
			Assert::AreEqual(string("7"), results[6]);



            /**
             *	assign a1, a2
             *  Select a2 such that Affects(a1, a2);
             *
             *
             */
            QueryTree* tree2 = new QueryTree();

            vector<string> declarations2;
            declarations2.push_back("assign a1,a2");
            tree2->setSymbolTable(declarations2);

            vector<string> selections2;
            selections2.push_back("a2");
            tree1->setResult(selections2);

            vector<string> relations2;
            relations2.push_back("Affects(a1, a2)");
            tree2->setSuchThat(relations2);

            tree2->setPattern(emptyVector);

            results = handler.queryRec(tree2);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            /*Assert::AreEqual(3, int(results.size()));
            Assert::AreEqual(string("4"), results[0]);
            Assert::AreEqual(string("5"), results[1]);
            Assert::AreEqual(string("6"), results[1]);*/



            /**
             *	assign a1, a2
             *  Select a1 such that Affects(1, a2);
             *
             *
             */
            QueryTree* tree3 = new QueryTree();

            vector<string> declarations3;
            declarations3.push_back("assign a1,a2");
            tree3->setSymbolTable(declarations3);

            vector<string> selections3;
            selections3.push_back("a2");
            tree1->setResult(selections3);

            vector<string> relations3;
            relations3.push_back("Affects(1, a2)");
            tree3->setSuchThat(relations3);

            tree3->setPattern(emptyVector);

            results = handler.queryRec(tree3);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(1, int(results.size()));
            Assert::AreEqual(string("4"), results[0]);



            /**
             *	assign a1, a2
             *  Select a1 such that Affects(a1, a2);
             *
             *
             */
            QueryTree* tree4 = new QueryTree();

            vector<string> declarations4;
            declarations4.push_back("assign a1,a2");
            tree4->setSymbolTable(declarations4);

            vector<string> selections4;
            selections4.push_back("");
            tree1->setResult(selections4);

            vector<string> relations4;
            relations4.push_back("Affects(, )");
            tree4->setSuchThat(relations4);

            tree4->setPattern(emptyVector);

            results = handler.queryRec(tree4);
            sort(results.begin(), results.end(), compareStringsAsNumbers);
            Assert::AreEqual(2, int(results.size()));
            Assert::AreEqual(string(""), results[0]);
            Assert::AreEqual(string(""), results[1]);


        }



		TEST_METHOD(Processor_Pattern) {
			vector<string> code = {
				"procedure First{",
				"x=3;",					// 1
				"y=2;",					// 2
				"z=1;",					// 3

				"a=x;",					// 4
				"b=x;",					// 5

				"a=x*y;",				// 6
				"b=x*y;",				// 7

				// TODO: DongWei remember to remove brackets
				"a=a+(x*y);",			// 8
				"b=a+(x*y);",			// 9

				"a=(x*y)+a;",			// 10
				"b=(x*y)+a;",			// 11

				"a=a+(x*y)+a;",			// 12
				"b=a+(x*y)+a;",			// 13

				"a=(x*y)+(x*y);",		// 14
				"b=(x*y)+(x*y);}",		// 15
			};

			DesignExtractor ext = DesignExtractor(code);

			// Variables that will be reused in different queries
			QueryHandler handler;
			vector<string> results;
			vector<string> emptyVector;



			/**
			 *	assign a;
			 *	Select a pattern a("b", _);
			 */
			QueryTree* tree1 = new QueryTree();

			vector<string> declarations1;
			declarations1.push_back("assign a");
			tree1->setSymbolTable(declarations1);

			vector<string> selections1;
			selections1.push_back("a");
			tree1->setResult(selections1);

			tree1->setSuchThat(emptyVector);

			vector<string> patterns1;
			patterns1.push_back("a(\"b\", _)");
			tree1->setPattern(patterns1);

			results = handler.queryRec(tree1);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("5"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("9"), results[2]);
			Assert::AreEqual(string("11"), results[3]);
			Assert::AreEqual(string("13"), results[4]);
			Assert::AreEqual(string("15"), results[5]);



			/**
			 *	assign a;
			 *	Select a pattern a("b", "x*y");
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("assign a");
			tree2->setSymbolTable(declarations2);

			vector<string> selections2;
			selections2.push_back("a");
			tree2->setResult(selections2);

			tree2->setSuchThat(emptyVector);

			vector<string> patterns2;
			patterns2.push_back("a(\"b\", \"x*y\")");
			tree2->setPattern(patterns2);

			results = handler.queryRec(tree2);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);



			/**
			 *	assign a;
			 *	Select a pattern a("b", _"x*y");
			 */
			QueryTree* tree3 = new QueryTree();

			vector<string> declarations3;
			declarations3.push_back("assign a");
			tree3->setSymbolTable(declarations3);

			vector<string> selections3;
			selections3.push_back("a");
			tree3->setResult(selections3);

			tree3->setSuchThat(emptyVector);

			vector<string> patterns3;
			patterns3.push_back("a(\"b\", _\"x*y\")");
			tree3->setPattern(patterns3);

			results = handler.queryRec(tree3);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(2, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("9"), results[1]);
			Assert::AreEqual(string("15"), results[2]);



			/**
			 *	assign a;
			 *	Select a pattern a("b", "x*y"_);
			 */
			QueryTree* tree4 = new QueryTree();

			vector<string> declarations4;
			declarations4.push_back("assign a");
			tree4->setSymbolTable(declarations4);

			vector<string> selections4;
			selections4.push_back("a");
			tree4->setResult(selections4);

			tree4->setSuchThat(emptyVector);

			vector<string> patterns4;
			patterns4.push_back("a(\"b\", \"x*y\"_)");
			tree4->setPattern(patterns4);

			results = handler.queryRec(tree4);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(2, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("11"), results[1]);
			Assert::AreEqual(string("15"), results[2]);



			/**
			 *	assign a;
			 *	Select a pattern a("b", _"x*y"_);
			 */
			QueryTree* tree5 = new QueryTree();

			vector<string> declarations5;
			declarations5.push_back("assign a");
			tree5->setSymbolTable(declarations5);

			vector<string> selections5;
			selections5.push_back("a");
			tree5->setResult(selections5);

			tree5->setSuchThat(emptyVector);

			vector<string> patterns5;
			patterns5.push_back("a(\"b\", _\"x*y\"_)");
			tree5->setPattern(patterns5);

			results = handler.queryRec(tree5);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(5, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("9"), results[1]);
			Assert::AreEqual(string("11"), results[2]);
			Assert::AreEqual(string("13"), results[3]);
			Assert::AreEqual(string("15"), results[4]);



			/**
			 *	assign a;
			 *	Select a pattern a(_, "x*y");
			 */
			QueryTree* tree6 = new QueryTree();

			vector<string> declarations6;
			declarations6.push_back("assign a");
			tree6->setSymbolTable(declarations6);

			vector<string> selections6;
			selections6.push_back("a");
			tree6->setResult(selections6);

			/*vector<string> relations6;
			relations6.push_back("");*/
			tree6->setSuchThat(emptyVector);

			vector<string> patterns6;
			patterns6.push_back("a(_, \"x*y\")");
			tree6->setPattern(patterns6);

			results = handler.queryRec(tree6);
			sort(results.begin(), results.end());
			Assert::AreEqual(2, int(results.size()));
			Assert::AreEqual(string("6"), results[0]);
			Assert::AreEqual(string("7"), results[1]);



			/**
			 *	assign a;
			 *	Select a pattern a(_, _"x*y");
			 */
			QueryTree* tree7 = new QueryTree();

			vector<string> declarations7;
			declarations7.push_back("assign a");
			tree7->setSymbolTable(declarations7);

			vector<string> selections7;
			selections7.push_back("a");
			tree7->setResult(selections7);

			tree7->setSuchThat(emptyVector);

			vector<string> patterns7;
			patterns7.push_back("a(_, _\"x*y\")");
			tree7->setPattern(patterns7);

			results = handler.queryRec(tree7);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("6"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("8"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("14"), results[4]);
			Assert::AreEqual(string("15"), results[5]);



			/**
			 *	assign a;
			 *	Select a pattern a(_, "x*y"_);
			 */
			QueryTree* tree8 = new QueryTree();

			vector<string> declarations8;
			declarations8.push_back("assign a");
			tree8->setSymbolTable(declarations8);

			vector<string> selections8;
			selections8.push_back("a");
			tree8->setResult(selections8);

			tree8->setSuchThat(emptyVector);

			vector<string> patterns8;
			patterns8.push_back(" a(_, \"x*y\"_)");
			tree8->setPattern(patterns8);

			results = handler.queryRec(tree8);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("6"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("10"), results[2]);
			Assert::AreEqual(string("11"), results[3]);
			Assert::AreEqual(string("14"), results[4]);
			Assert::AreEqual(string("15"), results[5]);



			/**
			 *	assign a;
			 *	Select a pattern a(_, _"x*y"_);
			 */
			QueryTree* tree9 = new QueryTree();

			vector<string> declarations9;
			declarations9.push_back("assign a");
			tree9->setSymbolTable(declarations9);

			vector<string> selections9;
			selections9.push_back("a");
			tree9->setResult(selections9);

			tree9->setSuchThat(emptyVector);

			vector<string> patterns9;
			patterns9.push_back("");
			tree9->setPattern(patterns9);

			results = handler.queryRec(tree9);
			sort(results.begin(), results.end(), compareStringsAsNumbers);
			Assert::AreEqual(10, int(results.size()));
			Assert::AreEqual(string("6"), results[0]);
			Assert::AreEqual(string("7"), results[1]);
			Assert::AreEqual(string("8"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("10"), results[4]);
			Assert::AreEqual(string("11"), results[5]);
			Assert::AreEqual(string("12"), results[6]);
			Assert::AreEqual(string("13"), results[7]);
			Assert::AreEqual(string("14"), results[8]);
			Assert::AreEqual(string("15"), results[9]);
		}


		private:
		static bool compareStringsAsNumbers(const string& s1, const string& s2) {
			int i1, i2;

			stringstream convert1(s1), convert2(s2);
			convert1 >> i1;
			convert2 >> i2;

			if (strtol(s1.c_str(), NULL, 10) > strtol(s2.c_str(), NULL, 10)) {
				return false;
			} else {
				return true;
			}

		}
	};

}