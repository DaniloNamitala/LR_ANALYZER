#include <iostream>
#include <vector>
#include "types.hpp"
#include "GLC.hpp"

#ifndef _AUTOMATON_STATE
#define _AUTOMATON_STATE

class LRItem {
private:
  ItemRule rule;
  int position;
public: 
  LRItem(ItemRule rule);

  // return the next symbol
  std::string nextSymbol();
  
  // return rule after read the symbol
  LRItem readSymbol();

  // true if item can be reduced
  bool reducible();

  bool operator==(const LRItem& second);
};

class State {
private:
  int id;
  std::vector<LRItem> items;
  std::map<std::string, State*> transitions;
public:
  State(int id = -1);
  void addItem(LRItem item);
  void addTransition(std::string c, State* state);
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

    State* createState(LRItem item);
};

#endif