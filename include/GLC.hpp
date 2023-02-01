#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <regex>
#include <string>
#include "types.hpp"

#ifndef __GLC
#define __GLC

#define LAMBDA "."
#define POINTER "*"

class GLC {
private:
  _GLC dataSet;
  std::string initialSymbol;

  void readDataFromFile(char* fileName);
  
  std::vector<std::string> findNullables();
  std::regex createRegex(std::vector<std::string> rules, bool findTerminal = false);
  std::vector<std::string> findChain(std::string var);
  std::vector<std::string> findTerminals();
  std::vector<std::string> findReachables(std::string start);

  void extend();
public:
  GLC(char* fileName); // create GLC from file

  std::pair<std::string, std::string> getInitialRule();

  std::vector<std::string> getRules(std::string variable);

  friend std::ostream& operator<<(std::ostream& out, GLC &g);

  std::vector<std::string> getSetOfFirst(std::string var);

  std::vector<std::string> getSetOfFollowers(std::string var);
};
#endif