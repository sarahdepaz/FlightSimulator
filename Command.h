#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <map>
#include "Evaluator.h"
//hfhbf
using namespace std;

/*
 * Command - Abstract class.
 * Represents commands that are enabled in our interpreter.
 * */
class Command {
 protected:
  unsigned int _argumentsAmount;
  map<string, double>* _symbolTable;

 public:
  virtual int execute(vector<string>& arguments, unsigned int index) = 0;

  virtual ~Command() {}
};

#endif //	!COMMAND_H