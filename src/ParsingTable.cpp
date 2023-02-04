#include "../include/ParsingTable.hpp" 
using namespace std;

int ParsingTable::getIndex(vector<string> v, string K) {
  auto it = find(v.begin(), v.end(), K);
  if (it != v.end()){
    int index = it - v.begin();
    return index;
  } else {
    return -1;
  }
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
            int ruleIndex = getIndex(rules, i.rule.second);
            table[state->id][c] = "r" + to_string(ruleIndex + 1);
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
