#ifndef LOOP_COMMAND_H
#define LOOP_COMMAND_H

#include "Command.h"
#include "ConditionParser.h"
#include "Parser.h"

using namespace std;
/**
 * LoopCommand class
*checks and handles the command in the loop
* */
class LoopCommand : public ConditionParser {
 public:
  LoopCommand(map<string, double> *symbolTable,
              map<string, Expression *> *expressionsMap,
              Parser *interpreter);
  virtual int execute(vector<string> &arguments, unsigned int index);
};

#endif