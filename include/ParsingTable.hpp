#include "./Automaton.hpp"

class ParsingTable {
  friend class Automaton;
  friend class State;
  private:
    std::map<int, std::map<std::string, std::string>> table;
  public:
    ParsingTable(Automaton* automaton);
    void print(Automaton* automaton);
    int getIndex(std::vector<std::string> v, std::string K);
    std::vector<std::string> getReducibleColumns(GLC* grammar,std::vector<std::string> v, std::vector <std::string> columns, std::string j, int index, bool isSrl1, bool isCrl1);
};