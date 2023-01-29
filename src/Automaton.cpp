#include "../include/Automaton.hpp"
using namespace std;

LRItem::LRItem(ItemRule rule) {
  this->rule = rule;
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

bool LRItem::operator==(const LRItem& second) {
  return this->position == second.position && this->rule == second.rule;
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

ostream& operator<<(ostream& out, State &s) {
  out << "State ----------- " << s.id << "\n";
  out << "ITEMS:\n";
  for (LRItem i: s.items) {
    out << "<" << i << ">\n";
  }
  out << "TRANSITIONS:\n";
  for (pair<string, State*> t: s.transitions) {
    out << t.first << " -> " << t.second->id << "\n";
  }
  return out;
}

ostream& operator<<(ostream& out, LRItem &i) {
  out << i.rule.first << " -> ";
  for (int k = 0; k < i.rule.second.size(); k++) {
    if (k == i.position) {
      out << POINTER;
    }
    out << i.rule.second[k];
  }
  if (i.position == i.rule.second.size()) {
    out << POINTER;
  }
  return out;
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
    for (LRItem item: actual->items) {
      string symbol = item.nextSymbol();
      LRItem* i = item.readSymbol();
      if (i != NULL) {
        if (actual->transitions.find(symbol) == actual->transitions.end()) {
          State* state = createState(*i);
          actual->addTransition(symbol, state);
          statesQueue.push(state);
          states.push_back(state);
        } else {
          State* state = actual->transitions.at(symbol);
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

