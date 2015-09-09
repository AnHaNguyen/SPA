#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{
	TEST_CLASS(ParserTest)
	{
	public:
		TEST_METHOD(TestIsInteger) {
			Parser p ;
			string sInteger = "1123";

			//p.isInteger(sInteger)
			Assert::AreEqual(true, p.isInteger(sInteger));

		}

	};
}
