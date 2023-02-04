#include <iostream>
#include "../include/Automaton.hpp"
#include "../include/ParsingTable.hpp"
using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "TOO FEW ARGUMENTS\n";
    return 1;
  }
  string type = "LR0";
  if (argc > 2) {
    type = argv[2];
  }
  char* input_file = argv[1];

  Automaton autom(input_file, type);
  autom.create();
  autom.print();

  ParsingTable table(&autom);
  table.print(&autom);
  return 0;
}