#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <string>

using namespace std;
/**
 * Expression class
* holds all the commands in expression map .
* */
class Expression {
 public:
  virtual double calculate(vector<string> &arguments, int index) = 0;
  virtual ~Expression() {}
};

#endif 