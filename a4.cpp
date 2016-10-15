////////////////////////////////////////////////////////////////////////////////
// File Name:      a2.cpp
//
// Author:         Gerald
// CS email:       gerald@cs.wisc.edu
//
// Description:    The source file for a2.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A2.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions/methods.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
////////////////////////////////////////////////////////////////////////////////
#include "a2.hpp"

#include "trim.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

void cleanData(std::ifstream &inFile, std::ofstream &outFile,
		std::unordered_set<std::string> &stopwords) {
	// TODO: Implement this method.
	// # of lines of code in Gerald's implementation: 13
	// Do the following operations on each review before
	// storing it to the output file:
	//   1. Replace hyphens with spaces.
	// the vector that is used to store ratings from file
	string line;
	// check if the file has next line
	while (getline(inFile,line)){
		// append the line into the vector
		replaceHyphensWithSpaces(line);
		vector<string> newVec;
		//   2. Split the line of text into individual words.
		splitLine(line, newVec);
		vector<string> vv;
		//   3. Remove the punctuation marks from the words
		removePunctuation(newVec,vv);
		//   4. Remove the trailing and the leading whitespaces in each word.
		removeWhiteSpaces(vv);
		//   5. Remove the empty words.
		removeEmptyWords(vv);
		//   6. Remove words with just one character in them. You should NOT remove
		//      numbers in this step because if you do so, you'll lose the ratings.
		removeSingleLetterWords(vv);
		//   7. Remove stopwords.
		removeStopWords(vv,stopwords);
		for (int i = 0; i < vv.size(); i++){
			outFile << vv[i] << ' ';
		}
		outFile << endl;
	}
}
void fillDictionary(std::ifstream &newInFile,
		std::unordered_map<std::string, std::pair<long, long>> &dict) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	string line;
	int rate = 0;
	int count = 0;
	while (getline(newInFile, line)){
		vector<string> vv;
		// store the rating of one line
		splitLine(line,vv);
		// obtain the first element of line
		double num = atol(vv[0].c_str());
		// iterate the loop to obtain the word and save them into dict
		for (int i = 1; i < vv.size(); i++){
			string token = vv[i];
			// look up the dict to find if the word has appeared before
			if (dict.find(token) == dict.end()){
				dict.emplace(token,make_pair(num,1));
			}
			else {
				// make the pair and save the data
				rate = num + dict[token].first;
				count = dict[token].second + 1;
				dict[token] = make_pair(rate,count);
			}
		}
	}
}

void fillStopWords(std::ifstream &inFile,
		std::unordered_set<std::string> &stopwords) {
	// TODO: Implement this methods.
	// approximate # of lines of code in Gerald's implementation: < 5
	string word;
	while (getline(inFile, word)){
		// insert the stop words into the file
		stopwords.insert(word);
	}
}

void rateReviews(std::ifstream &testFile,
		std::unordered_map<std::string, std::pair<long, long> > &dict,
		std::ofstream &ratingsFile) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	string line;
	while (getline(testFile,line)){
		vector<string> token;
		splitLine(line,token);
		double rate = 0;
		// if the dict does not have the word, or the word length is 0, set it to 2
		for (int i = 0; i < token.size(); i++){
			if (dict.find(token[i]) == dict.end() || token[i].length() == 0){
				rate += 2;
			}
			else {
				// update the token's rating
				rate += (double)dict[token[i]].first / (double)dict[token[i]].second;
			}
		}
		ratingsFile << fixed << setprecision(2) << rate/token.size() << endl;
	}
}

void removeEmptyWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	tokens.erase(remove_if(tokens.begin(), tokens.end(), [](std::string& s)
			{return s.empty();}), tokens.end());
}

void removePunctuation(std::vector<std::string> &inTokens,
		std::vector<std::string> &outTokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 10
	int punct_count=0;
	for (auto word:inTokens){
		for(string::size_type ix = 0 ;ix < word.size();++ix)
		{
			if(ispunct(word[ix])) // check if the word contains the puncuation mark
			{
				++punct_count;
				word.erase(ix,1);
				ix--;
			}
		}
		outTokens.push_back(word); // move the word into the vector after erase the puntuation
	}
}
void removeSingleLetterWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	for (int i = 0; i < tokens.size(); i++) {
		// check if the character is only one single letter
		if (tokens[i].length() == 1){
			// check if the character contains number
			if ((tokens[i].find_first_not_of( "0123456789" ) == string::npos) == false) {
				tokens.erase(tokens.begin()+i);
				--i;
			}
		}
	}
}

void removeStopWords(std::vector<std::string> &tokens,
		std::unordered_set<std::string> &stopwords) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	auto lambda_func = [stopwords](string word){return stopwords.find(word)!= stopwords.end();};
	// this word has found in stopwords list before it iterates to the end
	tokens.erase(remove_if(tokens.begin(),tokens.end(),lambda_func), tokens.end());
}

void removeWhiteSpaces(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	// You may want to use the trim() method from the trim.*pp files in a2.
	for (auto it = tokens.begin(); it != tokens.end(); ++it){
		trim(*it);
	}
}
void replaceHyphensWithSpaces(std::string &line) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	replace(line.begin(), line.end(), '-', ' ');
}

void splitLine(std::string &line, std::vector<std::string> &words) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 10
	char delim = ' ';
	stringstream ss(line);
	string item;
	vector<string> tokens;
	// split the line based on the white space token
	while (getline(ss, item, delim)) {
		words.push_back(item);
	}
}
