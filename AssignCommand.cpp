#include "AssignCommand.h"
#include "StringHelper.h"
#include "Evaluator.h"
#include <stdio.h>
// Assignement of command
AssignCommand::AssignCommand(map<string, double> *symbolTable,
                             map<string, string> *pathToVar,
                             map<string, vector<string>> *bindedVarTable,
                             Modifier *modifier) {
    _symbolTable = symbolTable;
    _pathToVar = pathToVar;
    _bindedVarTable = bindedVarTable;
    _modifier = modifier;
    _argumentsAmount = 2;
}

/*
* execute ->
* assigns variable it's requested value.
*/
int AssignCommand::execute(vector<string> &arguments, unsigned int index) {
    if ((arguments.size() - 1) < _argumentsAmount)
        throw "Arguments amount is lower than " + to_string(_argumentsAmount);
    // if variable doesn't exist, sends an exception.
    if (_symbolTable->find(arguments[index - 1]) == _symbolTable->end())
        throw "The variable " + arguments[index - 1] + " is not defined";
    string argument = StringHelper::getArgument(arguments);
    int varIndex = index - 1;
    double value = Evaluator::evaluate(arguments, &(++index), _symbolTable);
    // calls the modifier who is responsible to managing the variables.
    _modifier->setVariableValue(arguments[varIndex], value);
    _modifier->updateSimulator(arguments[varIndex]);
    return index;
}