#ifndef IF_COMMAND_H
#define IF_COMMAND_H

#include "Command.h"
#include "ConditionParser.h"
#include "Parser.h"

using namespace std;
/**
 * IfCommand class
* handel with command in loops .
* */
class IfCommand : public ConditionParser {
 public:
  IfCommand(map<string, double> *symbolTable,
            map<string, Expression *> *expressionsMap,
            Parser *interpreter);
  virtual int execute(vector<string> &arguments, unsigned int index);
};

#endif