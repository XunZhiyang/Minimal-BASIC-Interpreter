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
using namespace std;

Program::Program() {
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
}

void Program::clear() {
    mp.clear();
}

void Program::addSourceLine(int lineNumber, TokenScanner scanner) {
    map[lineNumber] = convertToStatement(scanner);
}

void Program::removeSourceLine(int lineNumber) {
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
    auto i = mp[lineNumber].find(lineNumber);
    return (++i) -> first;
    return 0;
}

void directlyExcecute(TokenScanner scanner) {
    Statement *p = convertToStatement(scanner);
    p -> execute();
}
