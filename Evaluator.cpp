#include "Evaluator.h"
#include "StringHelper.h"
#include "Mul.h"
#include "Minus.h"
#include "Div.h"
#include "Plus.h"
#include <math.h>
#include <stack>
#include <algorithm>

using namespace std;

map<string, int> Evaluator::_precedenceMap = {{"+", 0}, {"-", 0}, {"*", 1}, {"/", 1}};
vector<string> *Evaluator::commandNames = nullptr;

double Evaluator::evaluate(vector<string> arguments, unsigned int *index, map<string, double> *symbolTable) {
  vector<string> postfix = toPostfix(arguments, index, symbolTable);
  *index = lastIndex(arguments, index) + 1;
  return calculatePostfix(postfix);
}

/*
* assigns variables value by a given map.
* */
vector<string> Evaluator::assignVars(vector<string> arguments,
                                     unsigned int *index,
                                     map<string, double> *symbolTable) {
  unsigned int indexCopy = *index;
  map<string, int> operands(_precedenceMap);
  operands[")"] = 0;
  operands["("] = 0;
  unsigned int size = lastIndex(arguments, index);
  for (unsigned int i = indexCopy; i <= size; i++) {
    try {
      stod(arguments[i]);
    } catch (...) {
      if (operands.find(arguments[i]) != operands.end())
        continue;
      // if (StringHelper::startsWith(arguments[i], "\n")) {
      // 	arguments.erase(arguments.begin() + i);
      // 	continue;
      // }
      if (symbolTable->find(arguments[i]) == symbolTable->end())
        throw "The variable " + arguments[i] + " is not defined";
      arguments[i] = to_string(symbolTable->at(arguments[i]));
    }
  }
  // if we have -NUMBER instead of 0-NUMBER, we add 0
  if (arguments[indexCopy] == "-")
    arguments.insert(arguments.begin() + indexCopy, "0");
  for (unsigned int i = indexCopy; i < arguments.size() - 1; i++) {
    if (operands.find(arguments[i]) != operands.end() &&
        operands.find(arguments[i + 1]) != operands.end() &&
        arguments[i + 1] == "-") {
      arguments.insert(arguments.begin() + i + 1, "0");
    }
  }
  return arguments;
}

/*
* returns the last index of the expression.
* */
unsigned int Evaluator::lastIndex(vector<string> arguments,
                                  unsigned int *index) {
  bool firstIsOperand;
  bool secondIsOperand;
  map<string, int> operands(_precedenceMap);
  operands[")"] = 0;
  operands["("] = 0;
  // reads untill found two strings who aren't operands.
  for (unsigned int i = *index; i < arguments.size() - 1; i++) {
    firstIsOperand = (operands.find(arguments[i]) != operands.end());
    secondIsOperand = (operands.find(arguments[i + 1]) != operands.end());
    if ((firstIsOperand || secondIsOperand)
        && (std::find(commandNames->begin(), commandNames->end(), arguments[i]) == commandNames->end()))
      continue;
    if (std::find(commandNames->begin(), commandNames->end(), arguments[i]) != commandNames->end())
      return i - 1;
    else
      return i;
  }
  return arguments.size() - 1;
}

/*
* turns an infix vector to postfix.
* */
vector<string> Evaluator::toPostfix(vector<string> arguments,
                                    unsigned int *index,
                                    map<string, double> *symbolTable) {
  vector<string> afterAssign = assignVars(arguments, index, symbolTable);
  unsigned int assignedLastIndex = lastIndex(afterAssign, index) + 1;
  stack<string> operands;
  vector<string> postfix;
  auto end = afterAssign.begin() + assignedLastIndex;
  for (auto it = afterAssign.begin() + *index; it != end; ++it) {
    string token = *it;
    try {
      stod(token);
      postfix.push_back(token);
    } catch (...) {
      if (_precedenceMap.find(token) != _precedenceMap.end()) {
        // checks if stack top is operand
        while (!operands.empty() && _precedenceMap.find(operands.top()) != _precedenceMap.end())
          if (_precedenceMap[operands.top()] >= _precedenceMap[token]) {
            postfix.push_back(operands.top());
            operands.pop();
          } else
            break;
        operands.push(token);
      } else if (token == "(") {
        operands.push(token);
      } else {
        string top;
        while (!operands.empty() && (top = operands.top()).compare("(") != 0) {
          postfix.push_back(top);
          operands.pop();
        }
        if (!operands.empty())
          operands.pop();
      }
    }
  }
  while (!operands.empty()) {
    postfix.push_back(operands.top());
    operands.pop();
  }
  return postfix;
}

/*
* calculates a postfix vector representing mathematica expression.
* */
double Evaluator::calculatePostfix(vector<string> arguments) {
  stack<Number> operands;
  while (!arguments.empty()) {
    try {
      double value = stod(arguments.front());
      arguments.erase(arguments.begin());
      operands.push(Number(value));
    } catch (...) {
      string operant = arguments.front();
      arguments.erase(arguments.begin());
      Number leftNumber = operands.top();
      operands.pop();
      Number rightNumber = operands.top();
      operands.pop();
      operands.push(calculateNumber(operant, rightNumber, leftNumber));
    }
  }
  return operands.top().calculate(arguments, 0);
}

/*
* returns a number expression by activating the operand on two numbers.
* */
Number Evaluator::calculateNumber(string operant, Number leftNumber, Number rightNumber) {
  vector<string> dummy;
  if (operant.compare("+") == 0) {
    return Number(Plus(leftNumber, rightNumber).calculate(dummy, 0));
  }
  if (operant.compare("-") == 0) {
    return Number(Minus(leftNumber, rightNumber).calculate(dummy, 0));
  }
  if (operant.compare("*") == 0) {
    return Number(Mul(leftNumber, rightNumber).calculate(dummy, 0));
  }
  if (operant.compare("/") == 0) {
    return Number(Div(leftNumber, rightNumber).calculate(dummy, 0));
  }
  return Number(-1);
}
