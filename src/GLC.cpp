#include "../include/GLC.hpp"
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

ostream& operator<<(ostream& out, GLC &g) {
    for (_GLC::reference v : g.dataSet) {
      out << v.first << " -> ";
      for (int i = 0; i < v.second.size(); i++) {
        out << v.second[i];
        if (i < v.second.size()-1) out << " | ";
      }
      out << "\n";
    }
    return out;
}