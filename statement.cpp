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

virtual void Assignment::execute() {
    TokenScanner scanner = scannerInit(line);

}
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
    }
    return "REM";
}

Statement *convertToStatement(TokenScanner &scanner) {
    StatementType type = statementClassification(token);
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
            p = new Goto(scanner);
            break;
        case CONDITIONAL :
            p = new Conditional(scanner);
            break;
        case END :
            p = new End;
            break;
        default :
            p = new Rem;
    }
    return p;
}
