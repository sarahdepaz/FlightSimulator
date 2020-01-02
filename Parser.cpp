#include <fstream>
#include "Parser.h"
#include "OpenServerCommand.h"
#include "ConnectControlClient.h"
#include "DefineVarCommand.h"
#include "AssignCommand.h"
#include "PrintCommand.h"
#include "CommandExpression.h"
#include "SleepCommand.h"
#include "IfCommand.h"
#include "LoopCommand.h"
#include "StringHelper.h"
#include "LRarrow.h"
#include "RLarrow.h"
#include "Evaluator.h"
#include "Lexer.h"
#define DELIM "\t (),"
#define BLOCK_COMMANDS ",while,if,"

using namespace std;

Parser::Parser() {
  _expressionsMap = new map<string, Expression*>();
  _symbolTable = new map<string, double>();
  _pathToVar = new map<string, string>();
  _varToPath = new map<string, string>();
  _bindedVarTable = new map<string, vector<string>>();
  _commandNames = new vector<string>();
  _stream = &cin;
  setExpressionsMap();
}

unsigned int Parser::parser(vector<string> line, int index) {
  unsigned int linesC = 1;
  for (unsigned int i = index; i < line.size();) {
    Expression* expression;
    string argument = StringHelper::getArgument(line);
    StringHelper::addSpaces(argument);
    line = StringHelper::split(argument, " ");
    try {
      if (_symbolTable->find(line[i]) != _symbolTable->end()) {
        ++i;
        continue;
      }
      // cout << "line at i is '" << line[i] << "'" << endl;
      expression = _expressionsMap->at(line[i]);
      line = parseBlock(line, &i, linesC);
    } catch (...) {
      // cout << "Could not resolve '" << line[i] << "'" << endl;
      i++;
      break;
    }
    try {
      i = (int) expression->calculate(line, i);
      // cout << " returned i: " << i << endl;
    } catch (string e) {
      cout << e << endl;
      break;
    } catch (char const* e) {
      cout << e << endl;
      break;
    } catch (...) {
      cout << "Unknown Error" << endl;
      exit(1);
    }
  }
  return linesC;
}

vector<string> Parser::parseBlock(vector<string>& line, unsigned int* i, unsigned int& linesC) {
  string seperator = ",";
  string blockCommands = string(BLOCK_COMMANDS);
  if (blockCommands.find(seperator + line[*i] + seperator) != string::npos) {
    string untilBracket = "";
    string current = StringHelper::getArgument(line);
    int indentation = 0;
    // we're in a block!
    linesC = 0;
    do {
      ++linesC;
      untilBracket += current + " ";
      if (current.find("{") != string::npos)
        ++indentation;
      if (current.find("}") != string::npos)
        if (--indentation == 0)
          break;
    } while (getline(*_stream, current));
    line = Lexer::lexer(untilBracket);
    string argument = StringHelper::getArgument(line);
    StringHelper::addSpaces(argument);
    line = StringHelper::split(argument, " ");
    *i += StringHelper::nextIndexOf(line.begin() + *i, "\n", line.end());
    // cout << "something"<<endl;
  }
  return line;
}

void Parser::setExpressionsMap() {
  ConnectControlClient* connect = new ConnectControlClient(_symbolTable);
  _modifier = new Modifier(_bindedVarTable, _symbolTable, _varToPath, connect);
  _expressionsMap->operator[]("connectControlClient") = new CommandExpression(connect);
  _expressionsMap->operator[]("openDataServer") = new CommandExpression(new OpenServerCommand(_symbolTable, _pathToVar, _modifier));
  _expressionsMap->operator[]("var") = new CommandExpression(new DefineVarCommand(_symbolTable, _expressionsMap));
  _expressionsMap->operator[]("=") = new CommandExpression(new AssignCommand(_symbolTable, _pathToVar, _bindedVarTable, _modifier));
  _expressionsMap->operator[]("Print") = new CommandExpression(new PrintCommand(_symbolTable));
  _expressionsMap->operator[]("Sleep") = new CommandExpression(new SleepCommand(_symbolTable));
  _expressionsMap->operator[]("if") = new CommandExpression(new IfCommand(_symbolTable, _expressionsMap, this));
  _expressionsMap->operator[]("while") = new CommandExpression(new LoopCommand(_symbolTable, _expressionsMap, this));
  _expressionsMap->operator[]("->") = new CommandExpression(new LRarrow(_varToPath));
  _expressionsMap->operator[]("<-") = new CommandExpression(new RLarrow(_pathToVar));
  for (auto it = _expressionsMap->begin(); it != _expressionsMap->end(); it++ )
    _commandNames->push_back(it->first);
  Evaluator::commandNames = this->_commandNames;
}

bool Parser::isScriptFile(string& line) {
  bool flag;
  vector<string> words = StringHelper::split(line, DELIM);
  if (words.size() != 2)
    return false;
  if (words[0].compare("run") != 0)
    return false;
  line = words[1].substr(1, words[1].length() - 2);
  ifstream file(line);
  flag = (bool) file;
  file.close();
  return flag;
}

void Parser::setStream(istream& stream) {
  _stream = &stream;
}

Parser::~Parser() {
  delete _expressionsMap;
  delete _symbolTable;
  delete _pathToVar;
  delete _bindedVarTable;
  delete _varToPath;
  delete _modifier;
}