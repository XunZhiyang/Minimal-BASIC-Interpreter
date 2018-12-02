/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "StanfordCPPLib/tokenscanner.h"

using namespace std;

// Program::Program() {
//    // Replace this stub with your own code
// }

Program::~Program() {
    clear();
}

void Program::clear() {
    for(auto i : mp) {
        delete i.second;
    }
    mp.clear();
}

void Program::addSourceLine(int lineNumber, TokenScanner *scanner) {
    StatementType type;
    Statement *p = convertToStatement(scanner, false, *this, type);
    // while (scanner -> hasMoreTokens()) {
        // cerr <<"___" << scanner -> nextToken() << "___" <<endl;
    // }
    if(p != NULL){
        if(mp.count(lineNumber)) delete mp[lineNumber];
        mp[lineNumber] = p;
    }
}

void Program::removeSourceLine(int lineNumber) {
    delete mp[lineNumber];
    mp.erase(lineNumber);
}

// string Program::getSourceLine(int lineNumber) {
//    return "";
// }

// void Program::setParsedStatement(int lineNumber, Statement *stmt) {
//
// }

Statement *Program::getParsedStatement(int lineNumber) {
    return mp[lineNumber];
}

int Program::getFirstLineNumber() {
    return mp.begin() -> first;
    return 0;
}

int Program::getPreviousLineNumber(int lineNumber) {
    auto i = mp.find(lineNumber);
    if(i != mp.begin()) return (--i) -> first;
    return -1;
}

int Program::getNextLineNumber(int lineNumber) {
    if(lineNumber == -1) return getFirstLineNumber();
    auto i = mp.find(lineNumber);
    if(++i != mp.end()) return i -> first;
    return -1;
}

void Program::run(EvalState &state) {
    state.currentLine = getFirstLineNumber();
    for(int &i = state.currentLine; i != -1; i = getNextLineNumber(i)) {
        mp[i] -> execute(state);
        if(i < 0) {
            i = getPreviousLineNumber(-i);
        }
    }
}

void Program::list() {
    for(auto i : mp) {
        i.second -> print();
    }
}

void directlyExcecute(TokenScanner *scanner, EvalState &state, Program &program) {
    StatementType type;
    Statement *p = convertToStatement(scanner, true, program, type);
    // cerr << "I'm finishing!!!" << endl;
    if(p != NULL) {
        p -> execute(state);
        // cerr << "I'm finishing!!!" << endl;
        delete p;
        // cerr << "I'm finished!!!" << endl;
    }
    else {
        if(type == RUN) program.run(state);
        else if(type == CLEAR) {
            program.clear();
            state.clear();
        }
        else if(type == LIST) {
            program.list();
        }
    }
}

Statement *convertToStatement(TokenScanner *scanner, bool direct, Program &program, StatementType &type) {
    // cerr << "Converting!!!" << endl;
    type = statementClassification(scanner);
    // cerr << "Classification succeed!!" << endl;

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
            p = new Rem(scanner);
            break;
        case RUN :
            if(!direct || scanner -> hasMoreTokens()) error("SYNTAX ERROR");
            delete scanner;
            break;
        case QUIT :
            if(!direct || scanner -> hasMoreTokens()) error("SYNTAX ERROR");
            delete scanner;
            error("QUIT");
            break;
        case CLEAR :
            if(!direct || scanner -> hasMoreTokens()) error("SYNTAX ERROR");
            delete scanner;
            break;
        case LIST :
            if(!direct || scanner -> hasMoreTokens()) error("SYNTAX ERROR");
            delete scanner;
            break;
        default :
            delete scanner;
            error("SYNTAX ERROR");
    }
    return p;
}
