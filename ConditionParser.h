#ifndef CONDITION_PARSER_H
#define CONDITION_PARSER_H

#include "Command.h"
#include <map>
#include "Expression.h"
#include "Parser.h"

#define PAIR pair<Expression*, vector<string>>;
/**
 * ConditionParser class
* seperate the conditions in the xml .
* */
class ConditionParser : public Command {
  Parser *_interpreter;
  map<string, double> *_symbolTable;
  map<string, Expression *> *_expressionsMap;
  map<string, int> _operatorsMap;
 public:
  ConditionParser(map<string, double> *symbolTable,
                  map<string, Expression *> *expressionsMap,
                  Parser *interpreter);
  virtual int execute(vector<string> &arguments, unsigned int index) = 0;

 protected:

  /**
   * function that calculates whether the condition
   * held by the condition parser is accurate.
   * */
  bool conditionIsTrue(vector<string> &arguments);

  // this function runs the commands in the body's block.
  void runBlock(vector<string> &arguments);

  /** extracts the block of commands from the condition.
   * **/
  vector<string> extractBlock(vector<string> &vec);
};

#endif