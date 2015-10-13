#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PKBTest)
	{
	public:
		vector <string> code = { "procedureFirst{", "x=2;", "z=3;}",
			"procedureSecond{", "x=0;",  "i=5;" , "whilei{" ,"x=x+2+y;",
			"i=i+1;}" ,"z=z+x+i;", "y=z+2;", "x=x+y+z;}",
			"procedureThird{", "z=5;", "v=z;}" };

		TEST_METHOD(TestCheckAssign)
		{
			// TODO: Your test code here
			DesignExtractor ext = DesignExtractor(code);
			vector<string> result;
			result = PKB::checkAssign("x+2", false);
			Assert::AreEqual(result.size(), (unsigned)	0);
		//	Assert::AreEqual(result.at(0),(string) "9");
			result = PKB::checkAssign("2+x", true);
			Assert::AreEqual(result.size(), (unsigned)0);
			//Assert::AreEqual(result.at(0), (string) "6");
			//Assert::AreEqual(result.at(2), (string) "9");
		}
		TEST_METHOD(TestCheckIf)
		{
			//vector<string> result = PKB::patternIf()
		}
		TEST_METHOD(TestCheckWhile) 
		{
			DesignExtractor ext = DesignExtractor(code);
			vector<string> result = PKB::patternWhile("_");
			Assert::AreEqual(result.size(), (unsigned)1);
			Assert::AreEqual(result.at(0), (string)"5");
			result = PKB::patternWhile("x");
			Assert::AreEqual(result.size(), (unsigned)0);
			result = PKB::patternWhile("i");
			Assert::AreEqual(result.size(), (unsigned)1);
			Assert::AreEqual(result.at(0), (string)"5");
		}
	};
}