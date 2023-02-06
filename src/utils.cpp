#include "../include/types.hpp"

std::string charToStr(char v) {
  return std::string(1, v);
}

bool isVariable(std::string str) {
  return std::regex_match(str, std::regex("[A-Z][1-9]*"));
}

bool isValidRule(std::string str) {
  return std::regex_match(str, std::regex("(([A-Z][1-9]*)|[a-z])*|\\."));
}

bool isTerminal(std::string str) {
  return std::regex_match(str, std::regex("[a-z]|\\.|$"));
}

int stringToType(std::string strType) {
  if (strType == "LR0") {
    return LR0;
  } else if (strType == "SLR1") {
    return SLR1;
  } else if (strType == "CLR1") {
    return CLR1;
  } else if (strType == "LALR1") {
    return LALR1;
  }
  return LR0;
}