#include <iostream>
#include "../include/GLC.hpp"
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
  return 0;
}