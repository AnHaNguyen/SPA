#include "stdafx.h"
#include "CppUnitTest.h"

#include "QueryPreprocessor.h"
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

			results = handler.queryRec(tree1);
			Assert::AreEqual(9, int(results.size()));
			Assert::AreEqual(string("2"), results[0]);
			Assert::AreEqual(string("3"), results[1]);
			Assert::AreEqual(string("5"), results[2]);
			Assert::AreEqual(string("7"), results[3]);
			Assert::AreEqual(string("9"), results[4]);
			Assert::AreEqual(string("11"), results[5]);
			Assert::AreEqual(string("13"), results[6]);
			Assert::AreEqual(string("15"), results[7]);
			Assert::AreEqual(string("17"), results[8]);



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

			results = handler.queryRec(tree1);
			Assert::AreEqual(10, int(results.size()));
			/*Assert::AreEqual(string("2"), results[0]);
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
			Assert::AreEqual(string("7"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("3"), results[2]);



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
				"b=x+y+z+a+b;",		// 5
				"c=a+b;",			// 6
				"call Second;",		// 7
				"while y{",			// 8
				"y=y-1;",			// 9
				"z=y+x;",			// 10
				"b=y+z;}",			// 11
				"while x{",			// 12
				"x=x-1;}}", };		// 13
			//	"procedure Second{",
			//	"x=4+x;",			// 14
			//	"y=1+y;",			// 15
			//	"z=z-1;",			// 16
			//	"while x{",			// 17
			//	"x=x-1;"			// 18
			//	"call Third;}}",	// 19
			//	"procedure Third{",
			//	"z=1+1;",			// 20
			//	"i=x+y;",			// 21
			//	"while i{",			// 22
			//	"while x{"			// 23
			//	"i=i-1;"			// 24
			//	"j=1+1;}}}"			// 25
			//};

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
			Assert::AreEqual(6, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);



			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(5, v1);
			 */

			/**
			 *	stmt s1;
			 *	Select s1 such that Modifies(s1, "z");
			 */

			/**
			 *	while w1;
			 *	Select w1 such that Modifies(w1, "x");
			 */

			/**
			 *	stmt s1, while w1, variable v1;
			 *	Select v1 such that Modifies(5, v1);
			 */

			/**
			 *	variable v1;
			 *	Select v1 such that Modifies(8, v1);
			 *	
			 *	stmt# 8 is a while statement
			 */

			/**
			 *	assign a1, variable v1;
			 *	Select v1 such that Modifies(22, v1);
			 *	
			 *	stmt# 22 is a nested-while statement
			 */

			/**
			 *	stmt s1, variable v1;
			 *	Select s1 such that Modifies(s1, "i");
			 *
			 *	"i" is a variable in nested-procedure calls
			 */

			/**
			 *	stmt s1, variable v1;
			 *	Select v1 such that Modifies(7, v1);
			 *
			 *	stmt# 7 is the procedure call
			 */	

		}



		TEST_METHOD(Processor_Pattern) {
			vector<string> code = {
				"procedure First{",
				"x=3;",				// 1
				"y=2;",				// 2
				"z=1;",				// 3
				"a=x;",				// 4
				"a=x*y;",			// 5
				"b=x*y;",			// 6
				"b=a+x*y;",			// 7
				"b=x*y+a;",			// 8
				"b=a+x*y+a;",		// 9
				"c=x*y+x*y;",		// 10
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

			/**
			 *	assign a;
			 *	Select a pattern a("b", "x*y");
			 */

			/**
			 *	assign a;
			 *	Select a pattern a("b", _"x*y");
			 */

			/**
			 *	assign a;
			 *	Select a pattern a("b", "x*y"_);;
			 */

			/**
			 *	assign a;
			 *	Select a pattern a("b", _"x*y"_);;
			 */

			/**
			 *	assign a;
			 *	Select a pattern ;
			 */

			/**
			 *	assign a;
			 *	Select a pattern ;
			 */

			/**
			 *	assign a;
			 *	Select a pattern a(_, "x*y");
			 */

			/**
			 *	assign a;
			 *	Select a pattern a(_, _"x*y");
			 */

			/**
			 *	assign a;
			 *	Select a pattern a(_, "x*y"_);
			 */

			/**
			 *	assign a;
			 *	Select a pattern a(_, _"x*y"_);
			 */
		}


	};
}