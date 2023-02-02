#include <iostream>
#include "../include/Automaton.hpp"
#include "../include/ParsingTable.hpp"
using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "TOO FEW ARGUMENTS\n";
    return 1;
  }

  bool verbose = false;
  if (argc >= 4) {
    verbose = argv[3][0] == 'v';
  }

  char* input_file = argv[1];
  char* output_file = argv[2];

  
  Automaton autom(input_file);
  autom.generate();
  autom.print();

  ParsingTable table(&autom);
  table.print(&autom);
  return 0;
}