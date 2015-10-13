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
		TEST_METHOD(Processor_Trial_New) {
			// QueryTree for "stmt s1,s2;", "Select s1 such that Parent(s1, s2);"
			QueryTree* tree = new QueryTree();

			vector<string> declarations;
			declarations.push_back("stmt s1,s2");
			tree->setSymbolTable(declarations);

			vector<string> relations;
			relations.push_back("Parent(s1, s2)");
			tree->setSuchThat(relations);

			vector<string> selections;
			selections.push_back("s1");
			tree->setResult(selections);

			vector<string> empty;
			tree->setPattern(empty);
			
			vector <string> code = {
				"procedureFirst{",
				"x=2;",
				"z=3;}",
				"procedureSecond{",
				"x=0;",
				"i=5;",
				"whilei{",
				"x=x+2+y;",
				"i=i+1;}",
				"z=z+x+i;",
				"y=z+2;",
				"x=x+y+z;}",
				"procedureThird{",
				"z=5;",
				"v=z;}"
			};

			DesignExtractor ext = DesignExtractor(code);
			// PKB::getParentTable();

			QueryHandler handler0;
			vector<string> results = handler0.queryRec(tree);

			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("5"), results[0]);

			Assert::AreEqual(1, 2);

		}

		TEST_METHOD(Processor_Parent) {
			vector<string> code = {
				"procedureFirst{",
				"x=1;",			// 1
				"z=3;",			// 2
				"whilex{",		// 3  parent
				"x=x-1;",		// 4
				"whilex{",		// 5  p
				"y=x-1;",		// 6
				"whiley{",		// 7  p
				"x=x+2;",		// 8
				"whiley{",		// 9  p
				"y=1+1;",		// 10
				"whilex{",		// 11 p
				"x=2;}}}}}}",	// 12
				"procedureSecond{",	
				"a=1;",			// 13
				"whilea{",		// 14 p
				"b=1+a;",		// 15
				"whilea{",		// 16 p
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

			vector<string> relations1;
			relations1.push_back("Parent(s1, s2)");
			tree1->setSuchThat(relations1);

			vector<string> selections1;
			selections1.push_back("s1");
			tree1->setResult(selections1);

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

			vector<string> relations2;
			relations2.push_back("Parent(8, s1)");
			tree2->setSuchThat(relations2);

			vector<string> selections2;
			selections2.push_back("s1");
			tree1->setResult(selections2);

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

			vector<string> relations3;
			relations3.push_back("Parent(7, s1)");
			tree3->setSuchThat(relations3);

			vector<string> selections3;
			selections3.push_back("s1");
			tree1->setResult(selections3);

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

			vector<string> relations4;
			relations4.push_back("Parent(s1, 8)");
			tree4->setSuchThat(relations4);

			vector<string> selections4;
			selections4.push_back("s1");
			tree1->setResult(selections4);

			tree4->setPattern(emptyVector);

			results = handler.queryRec(tree4);
			Assert::AreEqual(1, int(results.size()));
			Assert::AreEqual(string("7"), results[0]);
		}

		TEST_METHOD(Processor_ParentStar) {
			vector <string> code = {
				"TestCode{",	// 0
				"x=1;",			// 1 
				"z=3;",			// 2
				"whilex{",		// 3 parent
				"x=x-1;",		// 4
				"whilex{",		// 5 p
				"y=x-1;",		// 6
				"whiley{",		// 7 p
				"x=x+2;",		// 8
				"whiley{",		// 9 p
				"y=1+1;",		// 10
				"whilex{",		// 11 p
				"x=2;}}}}}}"	// 12
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

			vector<string> relations1;
			relations1.push_back("Parent*(s1, s2)");
			tree1->setSuchThat(relations1);

			tree1->setPattern(emptyVector);
			tree1->setResult(emptyVector);

			results = handler.queryRec(tree1);
			Assert::AreEqual(5, int(results.size()));
			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("7"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("11"), results[4]);



			/**
			 *	stmt s1; 
			 *	Select s1 such that Parent*(s1, 8);
			 */
			QueryTree* tree2 = new QueryTree();

			vector<string> declarations2;
			declarations2.push_back("stmt s1");
			tree2->setSymbolTable(declarations2);

			vector<string> relations2;
			relations2.push_back("Parent*(s1, 8)");
			tree2->setSuchThat(relations2);

			tree2->setPattern(emptyVector);
			tree2->setResult(emptyVector);

			results = handler.queryRec(tree2);

			Assert::AreEqual(3, int(results.size()));
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

			vector<string> relations3;
			relations3.push_back("Parent*(7, s1)");
			tree3->setSuchThat(relations3);

			tree3->setPattern(emptyVector);
			tree3->setResult(emptyVector);

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

			vector<string> relations4;
			relations4.push_back("Parent*(7, s1)");
			tree4->setSuchThat(relations4);

			tree4->setPattern(emptyVector);
			tree4->setResult(emptyVector);

			results = handler.queryRec(tree4);

			Assert::AreEqual(5, int(results.size()));
			Assert::AreEqual(string("8"), results[0]);
			Assert::AreEqual(string("9"), results[1]);
			Assert::AreEqual(string("10"), results[2]);
			Assert::AreEqual(string("11"), results[3]);
			Assert::AreEqual(string("12"), results[4]);
		}

		TEST_METHOD(Processor_Pattern) {
			vector <string> code = {
				"procedureFirst{",
				"x=3;",
				"y=2",
				"z=1;",
				"b=x*y+x*y+z;",
				"a=y+x*y*y;"
				"a=y+x*y;}"

			};

			DesignExtractor ext = DesignExtractor(code);
			QueryPreprocessor qpp;
			QueryTree* tree = qpp.startProcess("assign a;", "Select a pattern a(_, _\"y + x * y\"_)");

			QueryHandler handler0;
			vector<string> results = handler0.queryRec(tree);

			Assert::AreEqual(string("6"), results[0]);
		}

		
	};
}