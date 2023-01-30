#include "./Automaton.hpp"

class ParsingTable {
  friend class Automaton;
  friend class State;
  private:
    std::map<int, std::map<std::string, std::string>> table;
  public:
    ParsingTable(Automaton* automaton);
    void print();
    int getIndex(std::vector<std::string> v, std::string K);
};