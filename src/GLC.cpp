#include "../include/GLC.hpp"
#include <sstream>
#include <unordered_set>
using namespace std;

GLC::GLC(char* fileName) {
  initialSymbol = "S";
  readDataFromFile(fileName);
  extend();
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

void GLC::extend(){
  string newInitial = "S'";
  dataSet[newInitial].push_back(initialSymbol);
  initialSymbol = newInitial;
}

pair<string, string> GLC::getInitialRule() {
  return pair<string, string>(initialSymbol, dataSet.at(initialSymbol).at(0));
}

vector<string> GLC::getRules(string variable) {
  try {
    return dataSet.at(variable);
  } catch (std::out_of_range e) {
    cerr << "CANT FIND VARIABLE " << variable << " RULE\n";
    return vector<string>();
  }
}

vector<string> removeDuplicates(vector<string> v) {
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}

vector<string> GLC::getSetOfFirst(string variable){
  vector<string> firsts;

  for (string r : dataSet[variable]) {
      if (isTerminal(charToStr(r[0]))) {
        firsts.push_back(string(1, r[0]));
        return firsts;
      } else if(isVariable(charToStr(r[0]))) {
        vector<string> firstsOfVar = getSetOfFirst(charToStr(r[0]));
        firsts.insert(firsts.end(), firstsOfVar.begin(), firstsOfVar.end());
      } else {
        throw runtime_error("INVALID RULE\n");
      }
  }
  firsts = removeDuplicates(firsts);
  return firsts;
}
