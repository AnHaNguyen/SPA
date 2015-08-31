#include"QueryTree.h"
#include<iostream>

QueryTree::QueryTree(){

    isValid = true;
    result = new PreResultNode();
    suchThat = new PreSuchThatNode();
    pattern = new PrePatternNode();
}

QueryTree::~QueryTree(){
}


//getter
bool QueryTree::getValidity(){
    return isValid;
}

PreResultNode* QueryTree::getResult(){
    return result;
}

PreSuchThatNode* QueryTree::getSuchThat(){
    return suchThat;
}
PrePatternNode* QueryTree::getPattern(){
    return pattern;
}
vector< vector<string> > QueryTree::getSymbolTable(){
    return symbolTable;
}


//setter
void QueryTree::setValidity(bool boolean){
    isValid = boolean;
}

void QueryTree::setResult(vector<string> terms){
    if(terms.size()==0) return;

    string str = terms[0];
    str = removeSpace(str);
    result->setResult(str);
}

void QueryTree::setSuchThat(vector<string> terms){
    if(terms.size()==0) return;

    string str = terms[0];
    str = removeSpace(str);

    vector<string> words = stringToVector(str, "(");
    words[0] = trim(words[0]);
    suchThat->setSynonym(words[0]);


    vector<string> attributes = stringToVector(words[1], ",");
    attributes[0] = trim(attributes[0]);
    suchThat->setFirstAttr(attributes[0]);

    vector<string> remain = stringToVector(attributes[1], ")");
    remain[0] = trim(remain[0]);
    suchThat->setSecondAttr(remain[0]);
}

void QueryTree::setPattern(vector<string> terms){
    if(terms.size()==0) return;

    string str = terms[0];
    str = removeSpace(str);

    vector<string> words = stringToVector(str, "(");
    words[0] = trim(words[0]);
    pattern->setSynonym(words[0]);

    vector<string> attributes = stringToVector(words[1], ",");
    attributes[0] = trim(attributes[0]);
    pattern->setFirstAttr(attributes[0]);

    vector<string> remain = stringToVector(attributes[1], ")");
    remain[0] = trim(remain[0]);
    pattern->setSecondAttr(remain[0]);
}

void QueryTree::setSymbolTable(vector<string> terms){
    if(terms.size()==0) return;

    for(int i=0; i<terms.size(); i++){
        if(terms[i].find(",") == string::npos){
            string firstWord = getFirstToken(terms[i]);
            string remainWord = removeFirstToken(terms[i]);
            remainWord = trim(remainWord);
            int index = getSymbolIndex(symbolTable, firstWord);
            if( index != -1){
                remainWord = trim(remainWord);
                symbolTable[index].push_back(remainWord);
            }
            else{
                vector<string> newType;
                newType.push_back(firstWord);
                remainWord = trim(remainWord);
                newType.push_back(remainWord);
                symbolTable.push_back(newType);
            }
        }
        else{
            string firstWord = getFirstToken(terms[i]);
            string remainWord = removeFirstToken(terms[i]);
            remainWord = trim(remainWord);
            vector<string> synonyms = stringToVector(remainWord, ",");
            int index = getSymbolIndex(symbolTable, firstWord);
            if( index != -1){
                for(int j=0; j<synonyms.size(); j++){
                    synonyms[j] = trim(synonyms[j]);
                    symbolTable[index].push_back(synonyms[j]);
                }
            }
            else{
                vector<string> newType;
                newType.push_back(firstWord);
                for(int k=0; k<synonyms.size(); k++){
                    synonyms[k] = trim(synonyms[k]);
                    newType.push_back(synonyms[k]);
                }
                symbolTable.push_back(newType);
            }
        }
    }
}

int QueryTree::getSymbolIndex(vector< vector<string> > table, string str){
    for(int i=0; i<table.size(); i++){
        if(table[i][0] == str){
            return i;
        }
    }
    return -1;
}

vector<string> QueryTree::stringToVector(string original, string delimiter){

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

string QueryTree::removeSpace(string str){
    string result = "";
    for(int i=0; i<str.size(); i++){
        if(str.at(i) != ' '){
            result = result + str.at(i);
        }
    }
    return result;
}

string QueryTree::getFirstToken(string str){
    string result = "";
    for(int i=0; i<str.size(); i++){
        if(str.at(i) != ' ')
            result = result + str.at(i);
        else
            break;
    }
    return result;
}

string QueryTree::trim(string str){
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

string QueryTree::removeFirstToken(string str){
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
