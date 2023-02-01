#include "../include/Automaton.hpp"
using namespace std;

LRItem::LRItem(ItemRule rule) {
  this->rule = rule;
  lookAhead.clear();
  position = 0;
}

string LRItem::nextSymbol() {
  if (position < rule.second.size()) {
    return charToStr(rule.second[position]);
  }
  return "";
}

LRItem* LRItem::readSymbol() {
  if (this->position < rule.second.size()) {
    LRItem* newItem = new LRItem(this->rule);
    newItem->position = this->position + 1;
    return newItem;
  }
  return NULL;
}

bool LRItem::reducible() {
  return position == rule.second.size();
}

void LRItem::setLookAhead(std::vector<std::string> lookAhead) {
  this->lookAhead = lookAhead;
}

State::State(int id) {
  this->id = id == -1 ? 0 : id;
}

void State::addItem(LRItem item) {
  if (find(items.begin(), items.end(), item) == items.end()) { //adiciona somente se não exisre ainda
    items.push_back(item);
  }
}

void State::addTransition(std::string c, State* state) {
  transitions[c] = state;
}

Automaton::Automaton(GLC* grammar) {
  this->grammar = grammar;
}

Automaton::Automaton(char* filename) {
  this->grammar = new GLC(filename);
}

State* Automaton::generate() {
  cout << *this->grammar << endl;
  queue<State*> statesQueue;
  State* firstState = createState(LRItem(grammar->getInitialRule()));
  states.push_back(firstState);
  statesQueue.push(firstState);

  while (!statesQueue.empty()) {
    State* actual = statesQueue.front(); statesQueue.pop();
    if (find(states.begin(), states.end(), actual) == states.end()) continue;
    for (LRItem item: actual->items) {
      string symbol = item.nextSymbol();
      LRItem* i = item.readSymbol();
      if (i != NULL) {
        State* state = NULL;
        if (actual->transitions.find(symbol) == actual->transitions.end()) {
          state = createState(*i);
          actual->addTransition(symbol, state);
          statesQueue.push(state);
          states.push_back(state);
        } else {
          state = actual->transitions.at(symbol);
          state->addItem(*i);
          int newItems = 0;
          int pos = 0;
          do {
            item = state->items[pos];
            string nextSymbol = item.nextSymbol();
            newItems = 0;
            if (isVariable(nextSymbol)) {
              vector<string> rules = grammar->getRules(nextSymbol);
              for (string rule : rules) {
                state->addItem(LRItem(ItemRule(nextSymbol, rule)));
                newItems++;
              }
            }
            pos++;
          } while (newItems != 0 and pos < state->items.size());
        }
        for (State* s: states) {
          if (*s == *state && s != state) {
            actual->transitions.at(symbol) = s;
            states.erase(find(states.begin(), states.end(), state));
          }
        }
      }
    }
  }
  return firstState;
}

void Automaton::print() {
  for (State* s: this->states) {
    cout << *s << endl;
  }
}

State* Automaton::createState(LRItem item) {
  State* state = new State(states.size());
  state->addItem(item);

  int newItems = 0;
  int pos = 0;
  do {
    item = state->items[pos];
    string nextSymbol = item.nextSymbol();
    newItems = 0;
    if (isVariable(nextSymbol)) {
      vector<string> rules = grammar->getRules(nextSymbol);
      for (string rule : rules) {
        state->addItem(LRItem(ItemRule(nextSymbol, rule)));
        newItems++;
      }
    }
    pos++;
  } while (newItems != 0 and pos < state->items.size());
  return state;
}

