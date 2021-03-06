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

Statement::Statement(TokenScanner *_scanner, int _lineNumber) : lineNumber(_lineNumber){
    line.clear();
    while(_scanner -> hasMoreTokens()){
        line += " ";
        line += _scanner -> nextToken();
    }
}

Statement::Statement(int _lineNumber) : lineNumber(_lineNumber) {}

Statement::~Statement() {
}
Assignment::Assignment(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Assignment::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    parseExp(*scanner) -> eval(state);
    delete scanner;
}

Print::Print(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

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

Input::Input(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

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
            cout << "INVALID NUMBER" << endl;
        }
    }
    delete scanner;
}


Goto::Goto(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Goto::execute(EvalState &state) {
    TokenScanner *scanner = scannerInit(line);
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    state.currentLine = -value;
    delete scanner;
}

Conditional::Conditional(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Conditional::execute(EvalState &state) {
    // cerr << "~~~" << line << endl;
    TokenScanner *scanner = scannerInit(line);
    TokenScanner tmpScanner;
    string cmp;
    int v1, v2;
    // cerr << "start first part" << endl;
    string tmp = "";
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "<" || p == "=" || p == ">") {
            v1 = (tmpScanner.setInput(tmp), parseExp(tmpScanner) -> eval(state));
            cmp = p;
            break;
        }
        tmp += p;
    }
    while(tmpScanner.hasMoreTokens()) tmpScanner.nextToken();
    tmp = "";
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "THEN") {
            v2 = parseExp((tmpScanner.setInput(tmp), tmpScanner)) -> eval(state);
            break;
        }
        tmp += p;
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
End::End(int _lineNumber) : Statement(_lineNumber) {}

void End::execute(EvalState &state) {
    state.currentLine = 0;
}

Rem::Rem(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Rem::execute(EvalState &state) {}

void Assignment::print() const {
    cout << lineNumber << " LET" << line << endl;
}
void Print::print() const {
    cout << lineNumber << " PRINT" << line << endl;
}
void Input::print() const {
    cout << lineNumber << " INPUT" << line << endl;
}
void Goto::print() const {
    cout << lineNumber << " GOT" << line << endl;
}
void Conditional::print() const {
    cout << lineNumber << " IF" << line << endl;
}
void End::print() const {
    cout << lineNumber << " END" << line << endl;
}
void Rem::print() const{
    cout << lineNumber << " REM" << line << endl;
}



TokenScanner *scannerInit(const string &line) {
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
