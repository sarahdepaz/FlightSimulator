#ifndef OPEN_SERVER_COMMAND_H
#define OPEN_SERVER_COMMAND_H

#include "Command.h"
#include "Modifier.h"
#include <iostream>
#include "DataReaderServer.h"

using namespace std;

class OpenServerCommand : public Command {
  DataReaderServer _dataReaderServer;

 public:
  OpenServerCommand(map<string,double>* symbolTable,
                    map<string, string>* pathToVar,
                    Modifier* modifier):
      _dataReaderServer(DataReaderServer(symbolTable, pathToVar, modifier)) {
    _argumentsAmount = 3;
  }

  /*
  * opens a server that the simulator could connect to.
  * */
  virtual int execute(vector<string>& arguments, unsigned int index) {
    if ((arguments.size() - 1) < _argumentsAmount)
      throw "Amount of arguments is lower than " + to_string(_argumentsAmount);
    return _dataReaderServer.execute(arguments, index + 1);
  }
};

#endif //	!OPEN_SERVER_COMMAND_H