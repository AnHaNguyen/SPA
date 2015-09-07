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

		TEST_METHOD(NoSuchNoPattern)
		{
			QueryPreprocessor pro0;
			string declare0 = "assign a1, a2;";
			string input0 = "Select a1";
			QueryTree* tree0 = pro0.startProcess(declare0, input0);

			Assert::AreEqual(tree0->getResult()->getResult(), (string)"a1");
			return;
		}
	};
}