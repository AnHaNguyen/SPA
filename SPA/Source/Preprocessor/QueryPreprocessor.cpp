#include <string>
#include <algorithm>
#include <iostream>
#include "QueryTree.h"
#include "QueryPreprocessor.h"

string designEntity[] = {"assign","stmt","while","variable","constant","prog_line"};

QueryPreprocessor::QueryPreprocessor(){
}

QueryPreprocessor::~QueryPreprocessor(){
}

QueryTree* QueryPreprocessor::startProcess(string input){
    input = trim(input);
    input = removeMultipleSpace(input);
	//cout << input << endl;

    if(isValidDeclaration(input) && isValidSuchThat(input) &&
       isValidPattern(input) && isValidSelection(input)){

        setDeclarationTable(input);
	//	cout << "-----declaration table----" << endl;
	//	printTable(declarations);

        setSuchThatTable(input);
	//	cout << "-----suchThat table----" << endl;
	//	printTable(relations);

        setPatternTable(input);
	//	cout << "-----pattern table----" << endl;
	//	printTable(patterns);

        setResultTable(input);
	//	cout << "-----result table----" << endl;
	//	printTable(selections);

        QueryTree* tree =  new QueryTree();
        tree->setSymbolTable(declarations);
        tree->setPattern(patterns);
        tree->setResult(selections);
        tree->setSuchThat(relations);
		return tree;
    }
    else{
        QueryTree* emptyTree =  new QueryTree();
        emptyTree->setValidity(false);
        return emptyTree;
    }
}

bool QueryPreprocessor::isValidDeclaration(string input){
	
    string str = input;
    if(str.find(";")==string::npos && str.find("boolean")==string::npos){
        return false;
    }

    vector<string> temp = stringToVector(str, ";");
    if(temp.size()<2){
        return false;
    }

    for(int i=0; i<temp.size()-1; i++){
        temp[i] = trim(temp[i]);
        int numWords = countWords(temp[i], " ");
        if(numWords<2){
            return false;
        }
        string firstWord = getFirstToken(temp[i]);
        firstWord = trim(firstWord);
        firstWord = toLowerCase(firstWord);
        string remainWord = removeFirstToken(temp[i]);
        remainWord = trim(remainWord);
        if(!containWord(firstWord, designEntity, 6)){
            return false;
        }
        if(remainWord.find(",")!=string::npos){
            if(countWords(remainWord, ",")<2){
                return false;
            }
            if(remainWord.at(remainWord.size()-1)==','){
                return false;
            }
        }
        else{
            if(countWords(remainWord, " ")>1){
                return false;
            }
        }
    }
    return true;
}

bool QueryPreprocessor::isValidSelection(string input){
	//to be implemented.....
	return true;
}

bool QueryPreprocessor::isValidSuchThat(string input){
	//to be implemented.....
	return true;

}

bool QueryPreprocessor::isValidPattern(string input){
	//to be implemented.....
	return true;

}


void QueryPreprocessor::setDeclarationTable(string input){


    string str = removeMultipleSpace(input);
    str = toLowerCase(str);

    declarations = stringToVector(str, ";");
	
	for (int i = 0; i < declarations.size(); i++) {
		declarations[i] = trim(declarations[i]);
	}

    declarations.pop_back();
}

void QueryPreprocessor::setSuchThatTable(string input){

    string str = removeMultipleSpace(input);
    str = toLowerCase(str);

    string result;
    if(str.find("such that ")!=string::npos){
        vector<string> words = stringToVector(str, "such that ");
        string stringAfterThat = words[1];
        vector<string> wordsAfterThat = stringToVector(stringAfterThat, ")");
        result = wordsAfterThat[0] + ")";
		result = trim(result);
		relations.push_back(result);
    }
}

void QueryPreprocessor::setPatternTable(string input){

    string str = removeMultipleSpace(input);
    str = toLowerCase(str);

    string result;
    if(str.find("pattern ")!=string::npos){
        vector<string> words = stringToVector(str, "pattern ");
        string stringAfterPattern = words[1];
        vector<string> wordsAfterPattern = stringToVector(stringAfterPattern, ")");
        result = wordsAfterPattern[0] + ")";
		result = trim(result);
		patterns.push_back(result);
    }
}

void QueryPreprocessor::setResultTable(string input){

    string str = removeMultipleSpace(input);
    str = toLowerCase(str);

    string result;
    if(str.find("select")!=string::npos){
        vector<string> words = stringToVector(str, "select ");
        string stringAfterSelect = words[1];
        vector<string> wordsAfterSelect = stringToVector(stringAfterSelect, " ");
        result = wordsAfterSelect[0];
    }
    else{
        result = "";
    }
    result = trim(result);
    selections.push_back(result);
}



vector<string> QueryPreprocessor::stringToVector(string original, string delimiter){

    string s = original;
    vector<string> words;

    int position = 0;
    string token;
    while ((position = s.find(delimiter)) != string::npos) {
        token = s.substr(0, position);
        words.push_back(token);
        s.erase(0, position + delimiter.length());
    }
    words.push_back(s);
    return words;
}

string QueryPreprocessor::toLowerCase(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int QueryPreprocessor::countWords(string str, string delimiter){
    vector<string> words = stringToVector(str,delimiter);
    return words.size();
}

string QueryPreprocessor::trim(string str){
    if (str==""){
        return str;
    }

	int i=0;
	int j=0;
	for (i=0;i<str.size();i++){
		if (str[i]!=' ')
			break;
	}
	for (j=str.size()-1;j>=i;j--){
		if (str[j]!=' ')
			break;
	}
	return str.substr(i,j-i+1);
}

string QueryPreprocessor::removeMultipleSpace(string str){

    string result = "";
    for(int i=0; i<str.size(); i++){
        if(!(str.at(i)==' ' && str.at(i+1)==' ')){
            result = result + str.at(i);
        }
    }
    return result;
}

string QueryPreprocessor::getFirstToken(string str){
    string result = "";
    for(int i=0; i<str.size(); i++){
        if(str.at(i) != ' ')
            result = result + str.at(i);
        else
            break;
    }
    return result;
}

string QueryPreprocessor::removeFirstToken(string str){
    str = trim(str);
    string result = "";
    int index=0;

    for(int i=0; i<str.size(); i++){
        if(str.at(i) == ' '){
           index=i;
           break;
        }
    }

    for(int j=index+1; j<str.size(); j++){
        result = result + str.at(j);
    }

    return result;
}

bool QueryPreprocessor::containWord(string str, string arr[], int size){
    for(int i=0; i<size; i++){
        if(arr[i]==str){
            return true;
        }
    }
    return false;
}

void QueryPreprocessor::printTable(vector<string> table) {
	for (int i = 0; i<table.size(); i++) {
		cout << table[i] << endl;
	}
}