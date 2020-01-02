#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Parser.h"
#include "BinaryExpression.h"
#include "Command.h"
#include "Minus.h"
#include "Plus.h"
#include "Mul.h"
#include "Div.h"
#include "Expression.h"
#include "Lexer.h"

using namespace std;

/*
* main function that runs the interpreter.
* reads the script line by line.
* */
int main(int argc, char *argv[]) {
  Parser interpreter;
  string inputStr;
  // if we have a filename to read from:
  if (argc == 2) {
    ifstream script(argv[1]);
    interpreter.setStream(script);
    while (getline(script, inputStr)) {
      vector<string> line = Lexer::lexer(inputStr);
      interpreter.parser(line, 0);
    }
    return 0;
  } else {
    cout << "No filename provided." << endl;
    return 1;
  }
}
