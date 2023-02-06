#include "../include/GLC.hpp"
#include <sstream>
#include <unordered_set>
using namespace std;

GLC::GLC(char* fileName) {
  initialSymbol = "";
  readDataFromFile(fileName);
  calcTerminalsAndVariables();
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
        if (dataSet.empty()) {
          initialSymbol = var;
        }
        dataSet[var].push_back(data);
      }
    }
  }
  file.close();
}

void GLC::calcTerminalsAndVariables() {
  regex reg = regex("([A-Z]\\')|([A-Z][1-9]*)|[a-z]"); //regex para separar variaveis e terminais
  for (auto rule : dataSet){
    if (find(variables.begin(), variables.end(), rule.first) == variables.end())
      variables.push_back(rule.first);
    for (vector<string>::iterator r = rule.second.begin(); r < rule.second.end(); r++) {
      regex_iterator<string::iterator> begin(r->begin(), r->end(), reg);
      regex_iterator<string::iterator> end;
      for (regex_iterator<string::iterator> i = begin; distance(i,end) > 0; i++) {
        if (isTerminal(i->str())) {
          if (find(terminals.begin(), terminals.end(), i->str()) == terminals.end())
            terminals.push_back(i->str());
        }
      }
    }
  }
  sort(terminals.begin(), terminals.end());
  sort(variables.begin(), variables.end());
}

void GLC::extend(){
  string newInitial = initialSymbol + "\'";
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

vector<string> GLC::getTerminals() {
  return terminals;
}

vector<string> GLC::getVariables() {
  return variables;
}

vector<string> removeDuplicates(vector<string> v) {
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}

vector<string> GLC::getSetOfFirst(string variable){
  vector<string> firsts;

  for (string r : getRules(variable)) {
    if (isTerminal(charToStr(r[0]))) {
      firsts.push_back(string(1, r[0]));
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

vector<string> removeDot(vector<string> v) {
  for (int i = 0; i < v.size(); i++) {
    v[i].erase(remove(v[i].begin(), v[i].end(), '.'), v[i].end());
  }
  return v;
}

vector<string> GLC::getSetOfFollow(string variable){
  vector<string> follows;
  if (variable == initialSymbol) {
    follows.push_back("$");
  }
  for (_GLC::reference rule : dataSet) {
    for (string r : rule.second) {
      for (int i = 0; i < r.size(); i++) {
        if (charToStr(r[i]) == variable) {
          if (i == r.size()-1) {
            if (rule.first != variable) {
              vector<string> followsOfVar = getSetOfFollow(rule.first);
              follows.insert(follows.end(), followsOfVar.begin(), followsOfVar.end());
            }
          } else {
            if (isTerminal(charToStr(r[i+1])) && r[i+1] != '.') {
              follows.push_back(charToStr(r[i+1]));
            } else if (isVariable(charToStr(r[i+1]))) {
              vector<string> firstsOfVar = removeDot(getSetOfFirst(charToStr(r[i+1])));
              if(find(firstsOfVar.begin(), firstsOfVar.end(), ".") == firstsOfVar.end()) {
                vector<string> followsOfVar = getSetOfFollow(rule.first);
                firstsOfVar.insert(firstsOfVar.end(), followsOfVar.begin(), followsOfVar.end());
              }
              follows.insert(follows.end(), firstsOfVar.begin(), firstsOfVar.end());
            } else {
              throw runtime_error("INVALID RULE\n");
            }
          }
        }
      }
    }
  }
  follows = removeDuplicates(follows);
  return follows;
}