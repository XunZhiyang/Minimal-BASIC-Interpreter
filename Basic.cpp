/*
 * File: Basic.cpp
 * ---------------
 * Name: Xun Zhiyang
 *
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "StanfordCPPLib/error.h"
#include "StanfordCPPLib/tokenscanner.h"
#include "StanfordCPPLib/strlib.h"
#include "StanfordCPPLib/simpio.h"

using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
       // cerr << "I'm doing!!!" << endl;
      try {
          // cerr << "I'm reading!!!" << endl;
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
          if(ex.getMessage() == "QUIT") break;
          // cerr << "I'm error!!!" << endl;
         cerr << "Error: " << ex.getMessage() << endl;
      }
      // cerr << "I'm aftererror!!!" << endl;
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    // cerr << "gthdhthtrdhrtdh" << endl;
    TokenScanner *scanner = scannerInit(line);
    if(line[0] >= '0' && line[0] <= '9') {
        int tmp = stringToInteger(scanner -> nextToken());
        program.addSourceLine(tmp, scanner);
    }
    else {
        directlyExcecute(scanner, state, program);
    }
    // cerr << "I'm processed!!!" << endl;
    // Expression *exp = parseExp(scanner);
    // int value = exp->eval(state);
    // cout << value << endl;
    // delete exp;
}
