#include <iostream>
#include <vector>
#include <map>

#ifndef TYPES
#define TYPES

/**
 * string -> Vector<string>
 * V -> {regras}
 */
typedef std::map<std::string, std::vector<std::string>> _GLC; // GLC data, rules and variables

typedef std::pair<std::string, std::vector<std::string>> GlcRule;

#endif