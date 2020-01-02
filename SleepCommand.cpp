#include "SleepCommand.h"
#include "Evaluator.h"
#include <unistd.h>
#define MILLI_IN_MICRO 1000

using namespace std;

SleepCommand::SleepCommand(map<string, double>* symbolTable) {
  _argumentsAmount = 3;
  _symbolTable = symbolTable;
}

int SleepCommand::execute(vector<string>& arguments, unsigned int index) {
  if ((arguments.size() - 1) < _argumentsAmount) {
    throw "Arguments amount is lower than " + to_string(_argumentsAmount);
  }
  index += 2;
  unsigned int argC = index;
  double millisec = Evaluator::evaluate(arguments, &argC, _symbolTable);
  usleep(millisec * MILLI_IN_MICRO);
  return argC;
}