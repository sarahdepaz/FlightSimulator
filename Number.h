#ifndef NUMBER_H
#define NUMBER_H

#include "Expression.h"

using namespace std;
/**
 * Number class
*  define the value as class number.
* */
class Number : public Expression {
 private:
  double _value;
 public:
  Number(double value) {
    _value = value;
  }

  virtual double calculate(vector<string> &, int) {
    return _value;
  }
};

#endif    //	!NUMBER_H