#include "../include/ParsingTable.hpp" 
using namespace std;

ParsingTable::ParsingTable(Automaton* automaton) {
  for (State* state: automaton->states) {
    for (LRItem item: state->items) {
      if (item.reducible()) {
        if(item.rule.first == "S'"){
          table[state->id]["$"] = "acc";
        }
        else{
          if(isVariable(item.rule.first)){
            table[state->id][item.rule.first] = item.position;
          }else{
            table[state->id][item.rule.first] = "r" + item.rule.first;
          }
        }
      }
      else if (state->transitions.find(item.nextSymbol()) != state->transitions.end()){
        table[state->id][item.nextSymbol()] = "s" + to_string(state->transitions[item.nextSymbol()]->id);
      }
    }
  }
}

int ParsingTable::getIndex(vector<string> v, string K)
{
  auto it = find(v.begin(), v.end(), K);
  if (it != v.end()){
    int index = it - v.begin();
    return index;
  } else {
    return -1;
  }
}

void ParsingTable::print() {
  vector<string> v; 
  vector<string> columns;

  for(auto i: table){
    for(auto j: i.second){
      if(find(columns.begin(),columns.end(),j.first) == columns.end()){
        if(isVariable(j.first)){
          columns.push_back(j.first);
        }else{
          columns.insert(columns.begin(),j.first);
        }
      }
    }
  }

  for(auto column: columns){
    cout << "     " << column;
    v.push_back("     |");
  }

  cout << endl;
  for(auto i: table){
    cout << i.first << "    ";
    for(auto j: i.second){
      int index = getIndex(columns,j.first);
      if(j.second == "acc"){
        v[index] = "acc  |";
      }else{
        v[index] = j.second + "   |";
      }
    }
    for(int k = 0; k < columns.size(); k++){
      cout << v[k];
      v[k] = "     |";
    }
    cout << endl;
  }
}
