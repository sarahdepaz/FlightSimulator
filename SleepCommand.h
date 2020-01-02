#ifndef SLEEP_COMMAND_H
#define SLEEP_COMMAND_H
#include "Command.h"

/**
 * SleepCommand class
* tells the airplain when to stop or wait fo command
* */
class SleepCommand : public Command {
 public:
  SleepCommand(map<string, double> *symbolTable);
  virtual int execute(vector<string> &arguments, unsigned int index);
};

#endif //    !SLEEP_COMMAND_H