//
// Created by sarah on 18/12/2019.
//

#include "RLarrow.h"

RLarrow::RLarrow(map<string, string> *pathToVar) {
  this->_pathToVar = pathToVar;
}
/**
* execute
* if the arrow is from right to left we accept the command from the simulator .
*/
int RLarrow::execute(vector<string> &arguments, unsigned int index) {
  string path = arguments[index + 3];
  string varName = arguments[index - 1];
  if (path[0] == '\"' && path[path.length() - 1] == '\"') {
    path = path.substr(1, path.length() - 2);
  }
  this->_pathToVar->operator[](path) = varName;
  return index + 5;
}