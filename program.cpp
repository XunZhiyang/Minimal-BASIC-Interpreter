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
   // Replace this stub with your own code
}

void Program::clear() {
    for(auto i : mp) {
        delete i.second;
    }
    mp.clear();
}

void Program::addSourceLine(int lineNumber, TokenScanner &scanner) {
    Statement *p = convertToStatement(scanner, false);
    if(p != NULL){
        if(mp.count(lineNumber)) delete mp[lineNumber];
        map[lineNumber] = p;
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

int Program::getNextLineNumber(int lineNumber) {
    if(lineNumber == -1) return -1;
    auto i = mp[lineNumber].find(lineNumber);
    if(++i != mp.end()) return i -> first;
    return -1;
}

void Program::run(EvalState &state) {
    state.currentLine = getFirstLineNumber();
    for(int &i = state.currentLine; i != -1; i = getNextLineNumber(i)) {
        mp[i] -> execute(state);
    }
}

void directlyExcecute(TokenScanner &scanner, EvalState &state, Program &program) {
    Statement *p = convertToStatement(scanner, true, program);
    if(p != NULL) {
        p -> execute(state);
        delete p;
    }
    else {
        program.run(state);
    }
}
