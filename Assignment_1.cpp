/**
 * A program that has a specified configuration file
 * on how to print out inputed text. All adjustments
 * are denoted with left or right parentheses as delimiters
 * except for double left or double right parenthesis.
 *
 * @author Albert Fong-Chiaw Chen
 * @date   5/11/2016
 */
#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>
using namespace std;

#ifndef DEFAULT_MODE
#define DEFAULT_MODE "\033[0;37m"
#endif

map<string, string> createKeywordList(ifstream& config);
void fixKey(string& key);
void removeDoubleBrace(string& s);
string divideString(string& s, map<string, string>& keywordsList, stack<string>& keywords, bool& readingKeyword, int& errorType);
string printKeyword(const string& s, map<string, string>& keywordsList);
void fixValue(string& value);
void printError(int line, int errorType);

/**
 * Overloaded << operator that prints out our non-printable characters.
 */
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

/**
 * Main method.
 */
int main(int argc, char const *argv[]) {
  //fileName
  string fileName;

  //checks for correct number of arguements
  if (argc > 2) {
    cerr << "Usage: {configuration file}";
    return 1;
  }

  //sets the appropriate fileName if specified
  if (argc == 1) {
    fileName = "config";
  } else if (argc == 2) {
    fileName = argv[1];
  }

  //opens the file.
  ifstream config(fileName);

  //Incase of failure to open file
  if (!config) {
    cerr << "unable to open file: " << fileName<< endl;
    return 1;
  }

  //map of keywords and their associated escapeValues
  map <string, string> keywordsList = createKeywordList(config);
  //stack of keywords currently being used
  stack<string> keywords;
  //user Input
  string inputText;

  //DEBUG_MODE
  #ifndef DEBUG

  bool  readingKeyword = false;

  int   errorType = 0;

  int line = 0;

  //Recieves user input and prints out with specified configurations
  while (getline(cin, inputText) && (errorType == 0)) {
    line++;
    inputText += '\n';
    if (line == 1 && (*(inputText.begin()) != '(' )) {
      errorType = 4;
      break;
    }
    removeDoubleBrace(inputText);
    cout << divideString(inputText, keywordsList, keywords, readingKeyword, errorType);
  }

  if (errorType == 0 && !keywords.empty()) {
    errorType = 2;
  }

  if (errorType != 0)
    printError(line, errorType);

  #else

  //Displays the configurations
  for (auto it = keywordsList.begin() ; it != keywordsList.end() ; ++it) {
    cout << it->second << '(' << it->first << ')' << DEFAULT_MODE << endl;
  }

  #endif
}

/**
 * Creates a map of keywords and their associated escape characters.
 * @param config configuration file
 * @return map of keywords
 */
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

/**
 * Adjusts the keys paraentheses to be non-printable characters.
 * @param key
 */
void fixKey(string& key) {
  for (auto it = key.begin() ; it != key.end() ; ++it) {
    if (*it == '(') {
      *it = '\001';
    } else if (*it == ')') {
      *it = '\002';
    }
  }
}

/**
 * Corrects the configuration file values to be accepted by standard convention.
 * @param value that needs to be fixed
 */
void fixValue(string& value) {
  string fixedCommand = "\033";
  size_t offset = value.find("[");
  if (offset != string::npos) {
    for (auto it = value.begin() + offset; it != value.end() ; ++it) {
      fixedCommand += *it;
    }
    value = fixedCommand;
  }
}

/**
 * Removes all instances of double parentheses and
 * replaces them with non-printable characters.
 * @param s the input string
 */
void removeDoubleBrace(string& s) {
  string newString;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    if (*it == '(') {
      newString += (*++it == '(') ? '\001' : (--it ,'(');
    } else if (*it == ')') {
      if (++it == s.end()) {
        break;
      }
      newString += (*it == ')') ? '\002' : (--it ,')');
    } else {
      newString += *it;
    }
  }
  s = newString;
}

/**
 * Converts our input into a string that is to be printed.
 * @param s input string.
 * @param keywordsList list of keywords from config file.
 * @param keywords the stack of current keywords.
 * @param readingKeyword boolean for if we are currently searching for a keyword
 */
string divideString(string& s, map<string, string>& keywordsList, stack<string>& keywords, bool& readingKeyword, int& errorType) {
  string newString;
  for (auto it = s.begin() ; it != s.end() ; ++it) {
    //Case for hitting the left delimiter
    if (*it == '(' || readingKeyword) {
      //The key word associated with this delimiter
      string newKeyword;

      if (!readingKeyword) {
        //checks if keyword is on the next line
        if (*++it == '\n') {
          readingKeyword = true;
          newString += '\n';
          return newString;
        }
        //skip whitespace
        while (isspace(*it)) {
          newString += *it++;
        }
      }

      //reads the keyword
      while (!isspace(*it) && (*it != '\n')) {
        newKeyword += *it++;
      }

      if (newKeyword[0] == ')') {
        errorType = 3;
        return newString;
      }

      readingKeyword = false;
      //throws it onto the stack
      keywords.push(newKeyword);
      //prints the code to change
      newString += printKeyword(keywords.top(), keywordsList);
      //skip whitespace
      if (isspace(*it)) {
        while (isspace(*++it)) {}
      }
      //short circuits to next line
      if (*it == '\n') {
        return newString;
      }
      --it;
    } else if (*it == ')') {
      if (!keywords.empty()) {
        keywords.pop();
        if (!keywords.empty()) {
          newString += printKeyword(keywords.top(), keywordsList);
        }
      } else {
        errorType = 1;
        return newString;
      }
    } else {
      if (!keywords.empty()) {
        newString += *it;
      }
    }
  }
  return newString;
}

/**
 * Grabs appropriate escape sequence from our keywordsList
 * @param s current keyword
 * @param keywordsList map of configurations
 * @return the escape sequence associated with that keyword
 */
string printKeyword(const string& s, map<string, string>& keywordsList) {
  if (keywordsList.find(s) == keywordsList.end()) {
    return DEFAULT_MODE;
  }
  return keywordsList.find(s)->second;
}

/**
 * Prints the appropriate error message.
 * @param line number
 * @param errorType identification for what type of error
 */
void printError(int line, int errorType) {
  cerr << DEFAULT_MODE << endl;
  cerr << "Error on line " << line;
  switch (errorType) {
    case 1:
      cerr << ": Extraneous token after end of top-level list" << endl;
      break;
    case 2:
      cerr << ": Unmatched left delimiter" << endl;
      break;
    case 3:
      cerr << ": No command after left delimiter" << endl;
      break;
    case 4:
      cerr << ": Extraneous token before start of top-level list" << endl;
      break;
    default:
      cerr << ": Undefined Error" << endl;
  }
}
