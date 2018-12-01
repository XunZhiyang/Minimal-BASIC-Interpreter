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
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}
Assignment::Assignment(const TokenScanner &_scanner) : scanner(_scanner) {

}

virtual void Assignment::execute(EvalState &state) {
    parseExp(scanner) -> eval(state);
}

Print::Print(const TokenScanner &_scanner) : scanner(_scanner) {}

virtual void Print::execute(EvalState &state) {
    Expression *exp = parseExp(scanner);
    int value = exp->eval(state);
    cout << value << endl;
}

Input::Input(const TokenScanner &_scanner) scanner(_scanner) : {}

virtual void Input::execute(EvalState &state) {
    cout << " ? ";
    string inputString = getLine();
    bool flag = true;
    while (flag) {
        flag = false;
        try {
            scanner.nextToken = stringToInteger(inputString);
        } catch (...) {
            flag = true;
            cerr << "INVALID NUMBER" << endl;
        }
    }
}

Goto::Goto(const TokenScanner &_scanner) : scanner(_scanner) {}

virtual void Goto::execute(EvalState &state) {
    Expression *exp = parseExp(scanner);
    int value = exp->eval(state);
    state.currentLine = value;
}

Conditional::Conditional(const TokenScanner &_scanner) : scanner(_scanner) {}

virtual void Conditional::execute(EvalState &state) {
    TokenScanner tmpScanner;
    string cmp;
    int v1, v2;
    for(;;) {
        string p = scanner.nextToken();
        if(p == "<" || p == "=" || p == ">") {
            v1 = parseExp(tmpScanner) -> eval(state);
            cmp = p;
            break;
        }
        tmpScanner.saveToken(p);
    }
    while(tmpScanner.hasMoreTokens()) tmpScanner.nextToken();
    for(;;) {
        string p = scanner.nextToken();
        if(p == "THEN") {
            v2 = parseExp(tmpScanner) -> eval(state);
            break;
        }
        tmpScanner.saveToken(p);
    }
    bool flag = false;
    switch(p) {
        case "=" : flag = (v1 == v2); break;
        case "<" : flag = (v1 < v2); break;
        case ">" : flag = (v1 > v2); break;
    }
    if(flag) {
        Expression *exp = parseExp(scanner);
        int value = exp->eval(state);
        state.currentLine = value;
    }
}
End::End(const TokenScanner &_scanner) : scanner(_scanner) {}

virtual void End::execute(EvalState &state) {
    state.currentLine = -1;
}

Rem::Rem(const TokenScanner &_scanner) scanner(_scanner) {}

virtual void Rem::execute(EvalState &state) {}

TokenScanner &scannerInit(string line) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    return scanner;
}

StatementType statementClassification(TokenScanner &scanner) {
    string token = scanner.nextToken();
    switch(token) {
        case "LET" : return ASSIGNMENT;
        case "PRINT" : return PRINT;
        case "INPUT" : return INPUT;
        case "GOTO" : return GOTO;
        case "IF" : return CONDITIONAL;
        case "END" : return END;
        case "REM" : return REM;
        case "RUN" : return RUN;
    }
    return ERROR;
}

Statement *convertToStatement(TokenScanner &scanner, bool direct, Program &program = Program()) {
    StatementType type = statementClassification(scanner);
    Statement *p = NULL;
    switch (type) {
        case ASSIGNMENT :
            p = new Assignment(scanner);
            break;
        case PRINT :
            p = new Print(scanner);
            break;
        case INPUT :
            p = new Input(scanner);
            break;
        case GOTO :
            if(direct) error("SYNTAX ERROR");
            p = new Goto(scanner);
            break;
        case CONDITIONAL :
            if(direct) error("SYNTAX ERROR");
            p = new Conditional(scanner);
            break;
        case END :
            if(direct) error("SYNTAX ERROR");
            p = new End;
            break;
        case REM :
            if(direct) error("SYNTAX ERROR");
            p = new Rem;
            break;
        case RUN :
            if(!direct || scanner.hasMoreTokens()) error("SYNTAX ERROR");
            break;
        default :
            error("NO SUCH STATEMENT");
    }
    return p;
}
