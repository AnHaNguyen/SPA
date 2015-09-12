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
		DesignExtractor ext = DesignExtractor(code);

		TEST_METHOD(TestCheckAssign)
		{
			// TODO: Your test code here
			vector<int> result = PKB::checkAssign("2", false);
			Assert::AreEqual(result.size(), (unsigned)	1);
			Assert::AreEqual(result.at(0), 1);
			result = PKB::checkAssign("2", true);
			Assert::AreEqual(result.size(), (unsigned)3);
			Assert::AreEqual(result.at(1), 6);
			Assert::AreEqual(result.at(2), 9);
		}
	};
}