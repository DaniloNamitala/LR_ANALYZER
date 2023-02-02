#include "../include/ParsingTable.hpp" 
using namespace std;

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

ParsingTable::ParsingTable(Automaton* automaton) {
  for (State* state: automaton->states) {
    for (LRItem item: state->items) {
      if (item.reducible()) {
        if(item.rule.first == "S'"){
          table[state->id]["$"] = "acc";
        }else{
          string rule = "r"; 
          auto rules = automaton->grammar->getRules(item.rule.first);
          int ruleIndex = getIndex(rules, item.rule.second);
          table[state->id][item.rule.first] = "r" + to_string(ruleIndex + 1);
        }
      }else if (state->transitions.find(item.nextSymbol()) !=         state->transitions.end()){
        if(isVariable(item.nextSymbol())){
          table[state->id][item.nextSymbol()] = to_string(state->transitions[item.nextSymbol()]->id);
        }else{
          table[state->id][item.nextSymbol()] = "s" + to_string(state->transitions[item.nextSymbol()]->id);
        }
     }
  }
  }
}

vector<string> ParsingTable::getReducibleColumns(GLC* grammar,vector<string> v, vector<string> columns, string j, int index, bool isSrl1, bool isCrl1 ) {
  vector<string> values = v;
  vector<string> listOfFollow;

  if(isSrl1){
    listOfFollow = grammar->getSetOfFollow(columns[index]);
    for (auto i : listOfFollow)
    {
      int index = getIndex(columns, i);
      if(index != -1){
        values[index] = j;
      }
    }    
  }else{
    for(int k = 0; k < v.size(); k++){
      if(k != index && !isVariable(columns[k])){
        values[k] = j;
      }
    }
  }
  return values;
}

void ParsingTable::print(Automaton* autom) {
  vector<string> v; 
  vector<string> columns;

  cout << "Parsing Table" << endl;
  cout << "SRL1: " << autom->isSLR1 << endl;

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

  sort(columns.begin(), columns.end(), greater<string>());

  for(string column: columns){
    printf("%10s|", column.c_str());
    v.push_back("");
  }

  cout << endl;
  for(auto i: table){
    printf("%d", i.first);

    for(auto j: i.second){
      int index = getIndex(columns,j.first);
      if(j.second == "acc"){
        v[index] = "acc";
      }else if(j.second[0] == 'r' && j.first != "S'"){
        v = getReducibleColumns(autom->grammar, v, columns,j.second, index, autom->isSLR1, autom->isCLR1);
      }else{
        v[index] = j.second;
      }
    }
    for(int k = 0; k < columns.size(); k++){
      printf("%10s|",v[k].c_str());
      v[k] = "";
    }
    cout << endl;
  }
}
