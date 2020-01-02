//
// Created by sarah on 18/12/2019.
//

#include "LRarrow.h"
#include <iostream>

using namespace std;

LRarrow::LRarrow(map<string, string>* varToPath) {
  this->_varToPath = varToPath;
}

int LRarrow::execute(vector<string>& arguments, unsigned int index) {
  string path = arguments[index + 3];
  string varName = arguments[index - 1];
  if (path[0] == '\"' && path[path.length() - 1] == '\"') {
    path = path.substr(1, path.length() - 2);
  }
  _varToPath->operator[](varName) = path;
  return index + 5;
}