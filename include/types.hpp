#include <iostream>
#include <vector>
#include <map>

#ifndef TYPES
#define TYPES

/**
 * string -> Vector<string>
 * V -> {regras}
 */
typedef std::map<std::string, std::vector<std::string>> _GLC; // GLC data, rules and variables

typedef std::pair<std::string, std::string> ItemRule;

std::string charToStr(char v) {
  std::string aux = "";
  aux += v;
  return aux;
}


bool isVariable(string str) {
  return regex_match(str, regex("[A-Z][1-9]*"));
}

bool isValidRule(string str) {
  return regex_match(str, regex("(([A-Z][1-9]*)|[a-z])*|\\."));
}

bool isTerminal(string str) {
  return regex_match(str, regex("[a-z]|\\."));
}

#endif