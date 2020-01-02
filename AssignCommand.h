#ifndef ASSIGN_COMMAND_H
#define ASSIGN_COMMAND_H

#include "Modifier.h"
#include "Command.h"

using namespace std;
/**
 * AssignCommand class
* assign the command into the maps.
* */
class AssignCommand : public Command {
  map<string, vector<string>> *_bindedVarTable;
  map<string, string> *_pathToVar;
  Modifier *_modifier;

 public:
  AssignCommand(map<string, double> *symbolTable,
                map<string, string> *pathToVar,
                map<string, vector<string>> *bindedVarTable,
                Modifier *modifier);

  /**
  * execute ->
  * assigns variable it's requested value.
  */
  virtual int execute(vector<string> &arguments, unsigned int index);
};

#endif //	!ASSIGN_COMMAND_H