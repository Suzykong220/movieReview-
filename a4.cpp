
#include "a4.hpp"
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
	vector<string> v;  // the vector that is used to store ratings from file
	vector<string> words;
	vector<string> vv;// initializing???
	if (inFile.is_open()){
		string line;
		// check if the file has next line
		while (inFile.peek()!=EOF){
			getline(inFile,line);
			// append the line into the vector
			v.push_back(line);
		}
		vector<string> newVec;
		for (int i = 0; i < v.size(); ++i){
			replaceHyphensWithSpaces(v[i]);
			//   2. Split the line of text into individual words.
			splitLine(v[i], newVec);
		}
		//   3. Remove the punctuation marks from the words.
		vector<string> vv;
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
			cout << vv[i] << "\n";
			outFile << vv[i] << endl;
	}
}
}
void fillDictionary(std::ifstream &newInFile,
		std::unordered_map<std::string, std::pair<long, long>> &dict) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	if (newInFile.is_open()){
		string line;
		int num;
		while (getline(newInFile, line)){
			num = atoi(&(line.at(0)));
			line.erase(0,2);
			string token;
			string delimiter = " ";
			int pos = 0;
			while ((pos = line.find(delimiter)) != string::npos){
				token = line.substr(0,pos);
				auto word = dict.find(token);
				if (word != dict.end()){
					(*word).second.second++;
					(*word).second.first += num;
				}
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
		stopwords.insert(word);
	}
}

void rateReviews(std::ifstream &testFile,
		std::unordered_map<std::string, std::pair<long, long> > &dict,
		std::ofstream &ratingsFile) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 20
	string line;
	while (testFile.peek()!=EOF){
		getline(testFile,line);
		double rate = 0;
		if (line.length()==0){
			rate = 2;
		}
		else {
			size_t pos = 0;
			string word;
			string delimiter = " ";
			int count = 0;
			while ((pos = line.find(delimiter)) != std::string::npos){
				word = line.substr(0,pos);
				auto it = dict.find(word);
				if (it==dict.end()){
					rate += 2;
				}
				else {
					rate += (*it).second.first / (*it).second.second;
				}
				count++;
			}
			rate = rate / count;
		}
		ratingsFile << fixed << setprecision(2) << rate << endl;
	}
}
void removeEmptyWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	for (auto it = tokens.begin(); it != tokens.end(); ++it){
		if ((*it).empty()){
			tokens.erase(it);
		}
	}
}
void removePunctuation(std::vector<std::string> &inTokens,
		std::vector<std::string> &outTokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 10
	int punct_count=0;
	for (auto word:inTokens){
		for(string::size_type ix = 0 ;ix < word.size();++ix)
		{
			if(ispunct(word[ix]))
			{
				++punct_count;
				word.erase(ix,1);
				ix--;
			}
		}
		outTokens.push_back(word);
	}
}
void removeSingleLetterWords(std::vector<std::string> &tokens) {
	// TODO: Implement this method.
	// approximate # of lines of code in Gerald's implementation: < 5
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].length() == 1){
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
	while (getline(ss, item, delim)) {
		words.push_back(item);
	}
}

	
	
