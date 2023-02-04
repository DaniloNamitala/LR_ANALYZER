#include "../include/Automaton.hpp"
#include "../include/types.hpp"
using namespace std;

LRItem::LRItem(ItemRule rule, bool isCLR1) {
  this->rule = rule;
  this->isCLR1 = isCLR1;
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
    LRItem* newItem = new LRItem(this->rule, this->isCLR1);
    newItem->lookAhead = this->lookAhead;
    newItem->position = this->position + 1;
    return newItem;
  }
  return NULL;
}

bool LRItem::reducible() {
  return position == rule.second.size();
}

void LRItem::setLookAhead(vector<string> lookAhead, int type) {
  if (type == CLR1)
    this->lookAhead = lookAhead;
}

vector<string> LRItem::getLookAhead() {
  return this->lookAhead;
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
  type = LR0;
  this->grammar = grammar;
}

Automaton::Automaton(char* filename) {
  type = LR0;
  this->grammar = new GLC(filename);
}

void Automaton::setType(int value) {
  type = value;
}

State* Automaton::generate() {
  cout << *this->grammar << endl;
  queue<State*> statesQueue;
  LRItem firstItem = LRItem(grammar->getInitialRule(), type == CLR1);
  firstItem.setLookAhead(vector<string>({"$"}), type);
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
        verifyState(state);
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
        LRItem newItem = LRItem(ItemRule(nextSymbol, rule), type == CLR1);
        newItem.setLookAhead(lookAhead, this->type);
        state->addItem(newItem);
        newItems++;
      }
    }
    pos++;
  } while (newItems != 0 and pos < state->items.size());
  return state;
}

void Automaton::verifyState(State* state ) {
  bool reducible = false;
  bool shift = false;

  if (type == LR0) {
    for (LRItem item: state->items) {
      if (item.reducible() && state->items.size() > 1) {
        setType(SLR1);
      }
    }
  } else {
    bool error = false;
    vector<LRItem> reducibles = vector<LRItem>();
    for (vector<LRItem>::iterator i = state->items.begin(); i < state->items.end() && !error; i++) {
      if (i->reducible()) {
        if (reducibles.size() > 0) {
          for (vector<LRItem>::iterator item = reducibles.begin(); item < reducibles.end() && !error; item++) {
            vector<string> f1 = (type == SLR1)? grammar->getSetOfFollow(i->rule.first) : i->getLookAhead();
            vector<string> f2 = (type == SLR1)? grammar->getSetOfFollow(item->rule.first) : item->getLookAhead();
            for (string r: f1) {
              error = find(f2.begin(), f2.end(), r) != f2.end();
            }
          }
        }
        reducibles.push_back(*i);
      } else {
        string next = i->nextSymbol();
        for (vector<LRItem>::iterator item = reducibles.begin(); item < reducibles.end() && !error; item++) {
          vector<string> f1 = (type == SLR1)? grammar->getSetOfFollow(item->rule.first) : item->getLookAhead();
          error = find(f1.begin(), f1.end(), next) != f1.end();
        }
      }
    }
    if (error) {
      setType(type+1);
    }
  }
}

State* Automaton::createState(LRItem item) {
  State* state = new State(states.size());
  state->addItem(item);
  return populateState(state);
}