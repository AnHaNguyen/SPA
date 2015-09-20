#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "../Preprocessor/QueryPreprocessor.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ASTTest)
	{
	public:

		TEST_METHOD(TestBig)
		{
			// TODO: Your test code here
			string filename = "simple.txt";
			Parser par;
			par.parseInput(filename);


			//string query = "";
			//QueryPreprocessor qpp;
			//list<string> result;
			//result = qpp.start(query);


		}
	};
}