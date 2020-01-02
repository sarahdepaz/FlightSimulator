#include "IfCommand.h"
#include "StringHelper.h"

using namespace std;

IfCommand::IfCommand(map<string, double>* symbolTable,
                     map<string, Expression*>* expressionsMap,
                     Parser* interpreter):
    ConditionParser(symbolTable, expressionsMap, interpreter) {
  _argumentsAmount = 5;
}

/*
* extracts it's block of commands and runs it if condition is true.
* */
int IfCommand::execute(vector<string>& arguments, unsigned int index) {
  if ((arguments.size() - 1 - index) < _argumentsAmount)
    throw "Amount of arguments is lower than " + to_string(_argumentsAmount);
  if (this->conditionIsTrue(arguments)) {
    vector<string> argumentsOnly = extractBlock(arguments);
    // cout << "args only: " << endl;
    // for (string s : argumentsOnly)
    // 	cout << "\t '" << s << "'" << endl;
    this->runBlock(argumentsOnly);
  }
  return StringHelper::nextIndexOf(arguments.begin(), "}", arguments.end()) + 1;
}
