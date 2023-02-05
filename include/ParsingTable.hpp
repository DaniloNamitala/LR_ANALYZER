#include "./Automaton.hpp"

class ParsingTable {
  friend class Automaton;
  friend class State;
  private:
    std::map<int, std::map<std::string, std::string>> table;
    void initializeTable(Automaton* automaton);
    void populateTable(Automaton* automaton);
    std::vector<std::string> getColumnsForItem (Automaton* autom, LRItem item);
    std::vector<std::string> getReducibleColumns(Automaton* autom,std::vector<std::string> v, std::vector <std::string> columns, std::string j, int index);
    std::map<std::pair<std::string, std::string>, int> createIndexMap(GLC* grammar);
  public:
    ParsingTable(Automaton* automaton);
    void print(Automaton* automaton);
};