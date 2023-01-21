#include "../include/Automaton.hpp"
using namespace std;

LRItem::LRItem(ItemRule rule) {
  this->rule = rule;
  position = 0;
}

string LRItem::nextSymbol() {
  if (rule.second.size() < position) {
    return charToStr(rule.second[position]);
  }
  return "";
}

LRItem LRItem::readSymbol() {
  LRItem newItem = LRItem(this->rule);
  newItem.position = this->position + 1;
  return newItem;
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
  if (find(items.begin(), items.end(), item) == items.end()) {
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

State* Automaton::createState(LRItem item) {
  State* state = new State;
  state->addItem(item);

  string nextSymbol = item.nextSymbol();
  if (isVariable(nextSymbol)) {
    vector<string> rules = grammar->getRules(nextSymbol);
    for (string rule : rules) {
      state->addItem(LRItem(ItemRule(nextSymbol, rule)));
    }
  }
}

