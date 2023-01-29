#include <ostream>
#include <vector>
#include "types.hpp"
#include "GLC.hpp"
#include <queue>

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
  LRItem* readSymbol();

  // true if item can be reduced
  bool reducible();

  bool operator==(const LRItem& second);
  friend bool operator!=(const LRItem& i, const LRItem& j);

  friend std::ostream& operator<<(std::ostream& out, LRItem &i);
};

class Automaton;
class State {
  friend class Automaton;
private:
  int id;
  std::vector<LRItem> items;
  std::map<std::string, State*> transitions;
public:
  State(int id = -1);
  void addItem(LRItem item);
  void addTransition(std::string c, State* state);
  friend std::ostream& operator<<(std::ostream& out, State &s);
  friend bool operator==(const State &i, const State &j);
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

    // create a new state
    State* createState(LRItem item);

    State* generate();

    void print();
};

#endif