#include <iostream>
#include <vector>
#include "types.hpp"
#include "GLC.hpp"

#ifndef _AUTOMATON_STATE
#define _AUTOMATON_STATE

class LRItem {
private:
  GlcRule rule;
  int position;
public: 
  LRItem(GlcRule rule);

  // return the next symbol
  std::string nextSymbol();
  
  // return rule after read the symbol
  LRItem readSymbol();

  // true if item can be reduced
  bool reducible();
};

struct State {
  int id;
  std::vector<LRItem> items;
  std::map<std::string, State*> transitions;
};

class Automaton {
  private:
    std::vector<State*> states;
    int statesCount;
    GLC* grammar; 
  public:
    // create LR automaton from grammar
    Automaton(GLC* grammar);

    // create LR automaton from grammar file
    Automaton(char* filename);
};

#endif