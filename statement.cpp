/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "StanfordCPPLib/error.h"
#include "StanfordCPPLib/tokenscanner.h"
#include "StanfordCPPLib/strlib.h"
#include "StanfordCPPLib/simpio.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement(TokenScanner *_scanner) {
    line.clear();
    while(_scanner -> hasMoreTokens())
    line += _scanner -> nextToken();
    line += ' ';
}

Statement::~Statement() {
   // if(scanner != nullptr) delete scanner;
}
Assignment::Assignment(TokenScanner *_scanner) : Statement(_scanner) {}

void Assignment::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    parseExp(*scanner) -> eval(state);
    delete scanner;
}

Print::Print(TokenScanner *_scanner) : Statement(_scanner) {}

void Print::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    // cerr << "I'm printing!!!" << endl;
    // while (scanner.hasMoreTokens()) {
    //     cerr <<"___" << scanner.nextToken() << "___" <<endl;
    // }
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    cout << value << endl;
    delete scanner;
    // cerr << "I'm printing!!!" << endl;
}

Input::Input(TokenScanner *_scanner) : Statement(_scanner) {}

void Input::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    bool flag = true;
    while (flag) {
        flag = false;
        try {
            cout << " ? ";
            string inputString = getLine();
            state.setValue(scanner -> nextToken(), stringToInteger(inputString));
        } catch (...) {
            flag = true;
            // cerr << "INVALID NUMBER" << endl;
        }
    }
    delete scanner;
}


Goto::Goto(TokenScanner *_scanner) : Statement(_scanner) {}

void Goto::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    state.currentLine = -value;
    delete scanner;
}

Conditional::Conditional(TokenScanner *_scanner) : Statement(_scanner) {}

void Conditional::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    TokenScanner tmpScanner;
    string cmp;
    int v1, v2;
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "<" || p == "=" || p == ">") {
            v1 = parseExp(tmpScanner) -> eval(state);
            cmp = p;
            break;
        }
        tmpScanner.saveToken(p);
    }
    while(tmpScanner.hasMoreTokens()) tmpScanner.nextToken();
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "THEN") {
            v2 = parseExp(tmpScanner) -> eval(state);
            break;
        }
        tmpScanner.saveToken(p);
    }
    bool flag = false;
    if(cmp == "=") flag = (v1 == v2);
    if(cmp == "<") flag = (v1 < v2);
    if(cmp == ">") flag = (v1 > v2);
    if(flag) {
        Expression *exp = parseExp(*scanner);
        int value = exp->eval(state);
        state.currentLine = -value;
    }
    delete scanner;
}
// End::End(TokenScanner *_scanner) : Statement(_scanner) {}

void End::execute(EvalState &state) {
    state.currentLine = -1;
}

Rem::Rem(TokenScanner *_scanner) : Statement(_scanner) {}

void Rem::execute(EvalState &state) {}

void Assignment::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "LET ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void Print::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "PRINT ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void Input::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "INPUT ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void Goto::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "GOTO ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void Conditional::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "IF ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void End::print() const {
    TokenScanner *scanner = scannerInit(line);
    cout << "END ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}
void Rem::print() const{
    TokenScanner *scanner = scannerInit(line);
    cout << "REM ";
    while(scanner -> hasMoreTokens()) cout << scanner -> nextToken();
    cout << endl;
    delete scanner;
}



TokenScanner *scannerInit(string line) {
    TokenScanner *scanner = new TokenScanner;
    scanner -> scanStrings();
    scanner -> ignoreWhitespace();
    scanner -> scanNumbers();
    scanner -> setInput(line);
    return scanner;
}

StatementType statementClassification(TokenScanner *scanner) {
    // cerr << "Classifying!!!" << endl;
    // cerr<<scanner;
    string token = scanner -> nextToken();
    // cerr << "Classifying!!!" << endl;

    if(token == "LET") return ASSIGNMENT;
    if(token == "PRINT") return PRINT;
    if(token == "INPUT") return INPUT;
    if(token == "GOTO") return GOTO;
    if(token == "IF") return CONDITIONAL;
    if(token == "END") return END;
    if(token == "REM") return REM;
    if(token == "RUN") return RUN;
    if(token == "QUIT") return QUIT;
    if(token == "CLEAR") return CLEAR;
    if(token == "LIST") return LIST;
    // cerr << "Classifying!!!" << endl;
    return ERROR;
}
