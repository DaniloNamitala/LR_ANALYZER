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
  std::vector<std::string> terminals;
  std::vector<std::string> variables;
  void readDataFromFile(char* fileName);
  void calcTerminalsAndVariables();
  void extend();
public:
  GLC(char* fileName); // create GLC from file

  std::pair<std::string, std::string> getInitialRule();

  std::vector<std::string> getRules(std::string variable);

  friend std::ostream& operator<<(std::ostream& out, GLC &g);

  std::vector<std::string> getSetOfFirst(std::string var);

  std::vector<std::string> getSetOfFollow(std::string var);

  std::vector<std::string> getTerminals();

  std::vector<std::string> getVariables();
};
#endif