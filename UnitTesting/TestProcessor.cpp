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
		void initializeEnvironment() {
			
		}

		TEST_METHOD(Processor_Parent) {
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
			QueryPreprocessor qpp;
			QueryTree* tree = qpp.startProcess("stmt s1,s2;", "Select s1 such that Parent(s1, s2);");

			QueryHandler handler0;
			vector<string> results = handler0.queryRec(tree);

			Assert::AreEqual(string("5"), results[0]);
		}

		TEST_METHOD(Processor_ParentStar) {
			vector <string> code = {
				"procedureFirst{",
				"x=1;",
				"z=3;",
				"whilex{",
				"x=x-1;",
				"whilex{",
				"y=x-1;",
				"whiley{",
				"x=x+2;",
				"whiley{",
				"y=1+1;",
				"whilex{",
				"x=2;}}}}}}"
			};

			DesignExtractor ext = DesignExtractor(code);
			QueryPreprocessor qpp;
			QueryTree* tree = qpp.startProcess("stmt s1,s2;", "Select s1 such that Parent(s1, s2);");

			QueryHandler handler0;
			vector<string> results = handler0.queryRec(tree);

			Assert::AreEqual(string("3"), results[0]);
			Assert::AreEqual(string("5"), results[1]);
			Assert::AreEqual(string("7"), results[2]);
			Assert::AreEqual(string("9"), results[3]);
			Assert::AreEqual(string("11"), results[4]);
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