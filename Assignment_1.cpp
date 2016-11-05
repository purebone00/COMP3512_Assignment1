#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
using namespace std;

string printKeyword(const string& s);
string divideString(string& s);

int main() {
  //string sampleText = "(normal ((Read)) these instructions (color(brown) carefully). This is a closed-book exam. There are 6 questions with a total of 25 marks. Answer (bold all) questions. Time allowed: (underline 80 minutes).)";
  string sampleText = "(normal    this (bold     is a(italic short)simple) test)";
  //string sampleText = "( normal abc(())def((ghi(italic ((BLUE)))Grey) ";
  //string sampleText = "(underline this (()) is a (((((bold very)) complicated) example)";

  string printedString = divideString(sampleText);

  cout << printedString;

}

string divideString(string& s) {
  string newString;
  stack<string> keywords;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '(') {
      //escape case
      if (*++it == '(') {
        newString += '(';
        ++it;
      } else {
        string newKeyword;
        //skip whitespace
        while (isspace(*it))
          ++it;

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
        newString += *it;
      }
    } else if (*it == ')') {
      if (++it == s.end()) {
        break;
      }
      //escape case
      if (*it == ')') {
        newString += ')';
        it++;
        continue;
      }
      keywords.pop();
      newString += printKeyword(keywords.top());
      newString += *it;
    } else {
      newString += *it;
    }
  }
  return newString;
}

string printKeyword(const string& s) {
  if (s == "normal") {
    return "1";
  } else if (s == "italic") {
    return "3";
  } else if (s == "bold") {
    return "5";
  } else if (s == "underline") {
    return "8";
  } else {
    return "0";
  }
}
