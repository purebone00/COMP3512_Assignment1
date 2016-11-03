#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;

struct Chunk {
  string keyWord;
  string word;
};

void divideString(istringstream& iss, vector<Chunk>& v);
string addSpace(string& s);

int main() {
  string sampleText = "(     normal ((Read)) these instructions (   color(brown) carefully). This is a closed-book exam. There are 6 questions with a total of 25 marks. Answer (bold all) questions. Time allowed: (underline 80 minutes).)";
  //string sampleText = "(normal this (bold is a (italic short)simple) test)";

  sampleText = addSpace(sampleText);

  cout << sampleText << endl;

  vector<Chunk> v;

  istringstream iss(sampleText);

  divideString(iss, v);

  for (Chunk c : v) {
    cout << c.keyWord << '\t' << c.word << endl;
  }
}

string addSpace(string& s) {
  string stringWithSpaces;
  for (auto it = s.begin() ; it != s.end() ; ++it) {

    if (*it == ')') {
      stringWithSpaces += *it;
      stringWithSpaces += " ";
    } else {
      stringWithSpaces += *it;
    }
  }
  return stringWithSpaces;
}


void divideString(istringstream& iss, vector<Chunk>& v) {
  stack<string>  keywords;
  string word;
  while (iss >> word) {
    if (word[0] == '(') {
      if (word.size() != 1) {
        keywords.push(word.substr(1,word.size()));
      } else {
        iss >> word;
        keywords.push(word);
      }
    } else if (word.back() == ')') {
      Chunk newChunk;
      newChunk.keyWord = keywords.top();
      newChunk.word = word.substr(0, word.size() - 1);
      v.push_back(newChunk);
      keywords.pop();
    } else {
      Chunk newChunk;
      newChunk.keyWord = keywords.top();
      newChunk.word = word;
      v.push_back(newChunk);
    }
  }
}


/*
void findingKeyword() {

}
*/
