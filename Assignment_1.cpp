#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cstdio>
using namespace std;

string printKeyword(const string& s);
string divideString(string& s);
string removeDoubleBrace(string& s);

ostream& operator<<(ostream& os, const string& s) {
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '\001') {
      os << '(';
    } else if (*it == '\002') {
      os << ')';
    } else {
      os << *it;
    }
  }
  return os;
}

int main() {
  //string sampleText = "(normal ((Read)) these instructions (color(brown) carefully). This is a closed-book exam. There are 6 questions with a total of 25 marks. Answer (bold all) questions. Time allowed: (underline 80 minutes).)";
  //string sampleText = "(normal    this (bold     is a(italic short)simple) test)";
  string sampleText = "( normal abc(())def((ghi(italic ((BLUE)))Grey)    ";
  //string sampleText = "(underline this (()) is a (((((bold very)) complicated) example)";
  //string sampleText = "(normal this (bold is a(italic short \n)simple) test)";

  sampleText = removeDoubleBrace(sampleText);

  cout << sampleText << endl;

  string printedString = divideString(sampleText);

  cout << printedString;

}

string removeDoubleBrace(string& s) {
  string newString;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '(') {
      if (*++it == '(') {
        newString += '\001';
      } else {
        newString += '(';
        --it;
      }
    } else if (*it == ')') {
      if (++it == s.end()) {
        break;
      }

      if (*it == ')') {
        newString += '\002';
      } else {
        newString += ')';
        --it;
      }
    } else {
      newString += *it;
    }
  }
  return newString;
}

string divideString(string& s) {
  string newString;
  stack<string> keywords;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '(') {
      string newKeyword;
      //skip whitespace
      while (isspace(*++it)) {}

      //reads the keyword
      while (!isspace(*it)) {
        newKeyword += *it++;
      }
      cout << newKeyword <<endl;
      //throws it onto the stack
      keywords.push(newKeyword);
      //prints the code to change
      newString += printKeyword(keywords.top());
      //skip whitespace
      while (isspace(*it))
        ++it;
      --it;
    } else if (*it == ')') {
      keywords.pop();
      if (keywords.empty()) {
          break;
      }
      newString += printKeyword(keywords.top());
    } else {
      newString += *it;
    }
  }
  return newString;
}

string printKeyword(const string& s) {
  if (s == "normal") {
    return "\033[0;37m";
  } else if (s == "italic") {
    return "\033[0;34m";
  } else if (s == "bold") {
    return "\033[0;31m";
  } else if (s == "underline") {
    return "\033[0;32m";
  } else {
    return "0";
  }
}
