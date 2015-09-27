#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UtilityTest)
	{
	public:

		TEST_METHOD(TestUtility) {
			
			vector<int> originInt = { 4,2,6,4,1,2,10,8,1 };
			vector<string> originStr = { "4","2","6","4","1","2","10","8","1" };
			vector<int> expectedInt = { 4,2,6,1,10,8 };
			vector<string> expectedStr = { "4","2","6","1", "10","8" };
			vector<int> resultInt = Utility::removeDuplicate(originInt);
			vector<string> resultStr = Utility::removeDuplicate(originStr);
			for (unsigned i = 0; i < 6; i++) {
				Assert::AreEqual(expectedInt.at(i), resultInt.at(i));
				Assert::AreEqual(expectedStr.at(i), resultStr.at(i));
			}
		}
	};
}