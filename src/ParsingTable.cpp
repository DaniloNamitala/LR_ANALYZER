#include "../include/ParsingTable.hpp" 
using namespace std;

map<pair<string, string>, int> ParsingTable::createIndexMap(GLC* grammar) {
  vector<string> var = grammar->getVariables();
  map<pair<string, string>, int> result;
  int index = 1;
  for (string v: var) {
    vector<string> rules = grammar->getRules(v);
    for (string r: rules) {
      result[pair<string, string>(v, r)] = index++;
    }
  }
  return result;
}

void ParsingTable::initializeTable(Automaton* automaton) {
  GLC* grammar = automaton->grammar;
  vector<string> columns;
  for (string s: grammar->getTerminals()) {
    columns.push_back(s);
  }
  columns.push_back("$");
  for (string s: grammar->getVariables()) {
    columns.push_back(s);
  }
  for (int i = 0; i < automaton->states.size(); i++) {
    for (string c: columns) {
      table[i][c] = "";
    }
  }
}

vector<string> ParsingTable::getColumnsForItem(Automaton* autom, LRItem item) {
  vector<string> list;
  if (autom->type == LR0) {
    list = autom->grammar->getTerminals();
    list.push_back("$");  
  } else if (autom->type == SLR1) {
    list = autom->grammar->getSetOfFollow(item.rule.first);
  } else if (autom->type == CLR1) {
    list = item.getLookAhead();
  }
  return list;
}

void ParsingTable::print(Automaton* autom) {
  cout << "Tipo: ";
  switch (autom->type) {
  case LR0:
    cout << "LR(0)\n";
    break;
  case SLR1:
    cout << "SLR(1)\n";
    break;
  case CLR1:
    cout << "CLR(1)\n";
    break;
  default:
    cerr << "TIPO DE GRAMATICA NAO SUPORTADA: LALR(1)\n";
    return;
    break;
  }

  map<pair<string, string>, int> indexMap = createIndexMap(autom->grammar);
  for (auto i: indexMap) {
    printf("(%d): %s -> %s\n", i.second, i.first.first.c_str(), i.first.second.c_str());
  }
  printf("\n");

  printf("%-3s|", "");
  for (auto c: table[0]) {
    if (isTerminal(c.first))
      printf("%-5s|", c.first.c_str());
  }
  for (auto c: table[0]) {
    if (!isTerminal(c.first))
      printf("%-5s|", c.first.c_str());
  }
  printf("\n");
  printf("----");
  for (auto c: table[0]) {
      printf("------");
  }
  printf("\n");
  for (auto l: table) {
    printf("%-3s|", to_string(l.first).c_str());
    for (auto c: l.second) {
      if (isTerminal(c.first))
        printf("%-5s|", c.second.c_str());
    }
    for (auto c: l.second) {
      if (!isTerminal(c.first))
        printf("%-5s|", c.second.c_str());
    }
    printf("\n");
  }
}

void ParsingTable::populateTable(Automaton* automaton) {
  map<pair<string, string>, int> indexMap = createIndexMap(automaton->grammar);
  for (State* state: automaton->states) {
    for (pair<string, State*> t: state->transitions) {
      if (isTerminal(t.first)) {
        table[state->id][t.first] = "s" + to_string(t.second->id);
      } else {
        table[state->id][t.first] = to_string(t.second->id);
      }
    }
    for (LRItem i: state->items) {
      if (i.reducible()) {
        if (i.rule.first == automaton->grammar->getInitialRule().first) {
          table[state->id]["$"] = "acc";
        } else {
          for (string c : getColumnsForItem(automaton, i)) {
            auto rules = automaton->grammar->getRules(i.rule.first);
            int ruleIndex = indexMap[i.rule];
            table[state->id][c] = "r" + to_string(ruleIndex);
          }
        }
      }
    }
  }
}

ParsingTable::ParsingTable(Automaton* automaton) { 
  initializeTable(automaton);
  populateTable(automaton);
}
