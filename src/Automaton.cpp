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
    newItem->lookAhead = this->lookAhead;
    newItem->position = this->position + 1;
    return newItem;
  }
  return NULL;
}

bool LRItem::reducible() {
  return position == rule.second.size();
}

void LRItem::setLookAhead(vector<string> lookAhead) {
  if (isCLR1)
    this->lookAhead = lookAhead;
}

vector<string> LRItem::calculateLookAheadForNext(GLC* grammar) {
  if (position >= rule.second.size() - 1) {
    return lookAhead;
  } else {
    string symbol = charToStr(rule.second[position + 1]);
    if (isTerminal(symbol)) {
      return vector<string>({symbol});
    } else {
      return grammar->getSetOfFirst(symbol);
    }
  }
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
  isCLR1 = false;
  this->grammar = grammar;
}

Automaton::Automaton(char* filename) {
  isCLR1 = false;
  this->grammar = new GLC(filename);
}

void Automaton::setClr1(bool value) {
  isCLR1 = value;
}

 bool Automaton::isClr1() {
  return isCLR1;
}

State* Automaton::generate() {
  cout << *this->grammar << endl;
  queue<State*> statesQueue;
  LRItem firstItem = LRItem(grammar->getInitialRule());
  firstItem.setLookAhead(vector<string>({"$"}));
  State* firstState = createState(firstItem);
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
          populateState(state);
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

State* Automaton::populateState(State* state) {
  int newItems = 0;
  int pos = 0;
  do {
    LRItem item = state->items[pos];
    string nextSymbol = item.nextSymbol();
    vector<string> lookAhead = item.calculateLookAheadForNext(grammar);
    newItems = 0;
    if (isVariable(nextSymbol)) {
      vector<string> rules = grammar->getRules(nextSymbol);
      for (string rule : rules) {
        LRItem newItem = LRItem(ItemRule(nextSymbol, rule));
        newItem.setLookAhead(lookAhead);
        state->addItem(newItem);
        newItems++;
      }
    }
    pos++;
  } while (newItems != 0 and pos < state->items.size());
  return state;
}

State* Automaton::createState(LRItem item) {
  State* state = new State(states.size());
  state->addItem(item);
  return populateState(state);
}