#include <iostream>
#include <vector>
#include <map>
#include <regex>

#ifndef TYPES
#define TYPES

/**
 * string -> Vector<string>
 * V -> {regras}
 */

typedef std::map<std::string, std::vector<std::string>> _GLC; // GLC data, rules and variables

typedef std::pair<std::string, std::string> ItemRule;

std::string charToStr(char v);

bool isVariable(std::string str);

bool isValidRule(std::string str);

bool isTerminal(std::string str);

enum {
  LR0 = 0,
  SLR1,
  CLR1,
  LALR1
};

#endif