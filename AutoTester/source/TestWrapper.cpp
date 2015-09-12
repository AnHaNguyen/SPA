#include "TestWrapper.h"
#include "../../source/Preprocessor/QueryPreprocessor.h"
#include "../../source/Parser.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	Parser par;
	par.parseInput(filename);
  // ...rest of your code...
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.

	try {
		list<string> resultList;

		vector<string> oneResult;
		QueryPreprocessor qpp;
		oneResult = qpp.start(query);
		string oneResultString = "";
		for (int i = 0; i < oneResult.size()-1; i++) {
			oneResultString = oneResultString + ",";
		}
		oneResultString = oneResultString + oneResult[oneResult.size() - 1];
		resultList.push_back(oneResultString);
	}
	catch (const exception& error) {
		cout << error.what() << endl;
	}
}
