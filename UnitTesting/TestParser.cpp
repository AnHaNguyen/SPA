#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestPreprocessor
{
	TEST_CLASS(TestStartPreprocessor)
	{
	public:

		TEST_METHOD(TestInteger)
		{
			Parser p;
			string sInteger = "123";
			Assert::AreEqual(true, p.isInteger(sInteger));

		}

		
	};
}