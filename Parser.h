#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include "Expression.h"
#include "Evaluator.h"
#include <iostream>
#include "Modifier.h"

using namespace std;
/**
 * Parser class
*  Interprets the code into maps of commands .
* */
class Parser {
 private:
  map<string, Expression *> *_expressionsMap;
  map<string, double> *_symbolTable;
  map<string, string> *_pathToVar;
  map<string, string> *_varToPath;
  map<string, vector<string>> *_bindedVarTable;
  Modifier *_modifier;
  istream *_stream;
  vector<string> *_commandNames;

 public:
  Parser();
  static bool isScriptFile(string &line);
  unsigned int parser(vector<string> line, int index);
  vector<string> parseBlock(vector<string> &line, unsigned int *i, unsigned int &linesC);
  void setStream(istream &stream);
  ~Parser();

 private:
  void setExpressionsMap();
};

#endif