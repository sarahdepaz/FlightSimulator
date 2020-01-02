//
// Created by sarah on 02/01/2020.
//

#include "Lexer.h"
#include "StringHelper.h"

#define DELIM "\t (),"

vector<string> Lexer::lexer(string line) {
  return StringHelper::split2(line, DELIM);
}