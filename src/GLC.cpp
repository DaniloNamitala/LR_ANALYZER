#include "../include/GLC.hpp"
using namespace std;

GLC::GLC(char* fileName) {
  initialSymbol = "S";
  readDataFromFile(fileName);
}

void GLC::readDataFromFile(char* fileName) {
  ifstream file(fileName);
  
  if (!file.is_open()) {
    throw runtime_error("FILE NOT FOUND\n");
  }

  string data;
  while(getline(file, data)) {
    stringstream line(data);
    string var;
    bool before_arrow = true;
    while (getline(line, data, ' ')) {
      before_arrow = before_arrow? data != "->" : before_arrow; 

      if (before_arrow) {
        var = data;
      } else if (isValidRule(data)) {
        dataSet[var].push_back(data);
      }
    }
  }
  file.close();
}

bool GLC::isVariable(string str) {
  return regex_match(str, regex("[A-Z][1-9]*"));
}

bool GLC::isValidRule(string str) {
  return regex_match(str, regex("(([A-Z][1-9]*)|[a-z])*|\\."));
}

bool GLC::isTerminal(string str) {
  return regex_match(str, regex("[a-z]|\\."));
}

void GLC::extend(){
  string newInitial = "S'";
  dataSet[newInitial].push_back(initialSymbol);
  initialSymbol = newInitial;
}