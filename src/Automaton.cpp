#include "../include/Automaton.hpp"
using namespace std;

LRItem::LRItem(GlcRule rule) {
  this->rule = rule;
  position = 0;
}

string LRItem::nextSymbol() {
  if (rule.second.size() < position)
    return rule.second[position];
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

Automaton::Automaton(GLC* grammar) {
  this->grammar = grammar;
}

Automaton::Automaton(char* filename) {
  this->grammar = new GLC(filename);
}

