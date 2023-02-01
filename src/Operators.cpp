#include "../include/Automaton.hpp"
#include <sstream>
using namespace std;

bool LRItem::operator==(const LRItem& second) {
  return this->position == second.position && this->rule == second.rule;
}

bool operator!=(const LRItem& i, const LRItem& j) {
  if(i.rule == j.rule && i.position == j.position) {
    return false;
  }
  return true;
}

ostream& operator<<(ostream& out, GLC &g) {
  for (_GLC::reference v : g.dataSet) {
    out << v.first << " -> ";
    for (int i = 0; i < v.second.size(); i++) {
      out << v.second[i];
      if (i < v.second.size()-1) out << " | ";
    }
    out << "\n";
  }
  return out;
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

bool operator==(const State &i, const State &j) {
  if (i.items.size() != j.items.size()) {
    return false;
  }
  for (int k = 0; k < i.items.size(); k++) {
    if (i.items[k] != j.items[k]) {
      return false;
    }
  }
  return true;
}