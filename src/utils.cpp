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