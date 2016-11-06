#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>
using namespace std;

map<string, string> createKeywordList(ifstream& config);
void fixKey(string& key);
string removeDoubleBrace(string& s);
string divideString(string& s, map<string, string>& keywordsList, stack<string>& keywords);
string printKeyword(const string& s, map<string, string>& keywordsList);
void fixValue(string& value);

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

int main(int argc, char const *argv[]) {

  string fileName;

  if (argc > 2) {
    cerr << "Usage: {configuration file}";
    return 1;
  }

  if (argc == 1) {
    fileName = "config";
  } else if (argc == 2) {
    fileName = argv[1];
  }

  ifstream config(fileName);

  if (!config) {
    cerr << "unable to open file: " << fileName<< endl;
    return 1;
  }
  map <string, string> keywordsList = createKeywordList(config);
  stack<string> keywords;
  string sampleText;

  while (getline(cin, sampleText)) {
    sampleText += '\n';
    sampleText = removeDoubleBrace(sampleText);
    cout << divideString(sampleText, keywordsList, keywords);
  }
}

map<string, string> createKeywordList(ifstream& config) {
  string line;
  map<string, string> keywords;
  while (getline(config, line)) {
    istringstream iss(line);
    string key, escapeValue;

    if((iss >> key >> escapeValue)) {
      fixKey(key);
      fixValue(escapeValue);
      keywords.insert(pair<string, string>(key, escapeValue));
    }
  }
  return keywords;
}

void fixKey(string& key) {
  for (auto it = key.begin() ; it != key.end() ; ++it) {
    if (*it == '(') {
      *it = '\001';
    } else if (*it == ')') {
      *it = '\002';
    }
  }
}

void fixValue(string& value) {
  string fixedCommand = "\033";
  for (auto it = value.begin() + 2 ; it != value.end() ; ++it) {
    fixedCommand += *it;
  }
  value = fixedCommand;
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

string divideString(string& s, map<string, string>& keywordsList, stack<string>& keywords) {
  string newString;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '(') {
      string newKeyword;
      //skip whitespace
      while (isspace(*++it)) {
        newString += *it;
      }
      //reads the keyword
      while (!isspace(*it) && (*it != '\n')) {
        newKeyword += *it++;
      }
      //throws it onto the stack
      keywords.push(newKeyword);
      //prints the code to change
      newString += printKeyword(keywords.top(), keywordsList);
      //skip whitespace
      if (isspace(*it)) {
        while (isspace(*++it)) {}
      }
      if (*it == '\n') {
        return newString;
      }
      --it;
    } else if (*it == ')') {
      keywords.pop();
      if (keywords.empty()) {
          break;
      }
      newString += printKeyword(keywords.top(), keywordsList);
    } else {
      newString += *it;
    }
  }
  return newString;
}

string printKeyword(const string& s, map<string, string>& keywordsList) {
  return keywordsList.find(s)->second;
}
