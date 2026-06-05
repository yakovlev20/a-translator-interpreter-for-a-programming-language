#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\Token.h"

#define TEST 0

using namespace std;

enum MachineState {
    START, 
    IDN, 
    AQ_INT, 
    WDM, 
    FLT, 
    WASS, 
    WNEQ, 
    WNLN, 
    MIS, 
    ERR,
    END
};

enum DataType {
    STRING,
    AQ_INTEGER,
    AQ_FLOAT,

};

class BufferValue {
    string buffer;
    DataType type;
public:
    BufferValue(DataType type) {
        buffer = "";
        this->type = type;
    }
    DataType getType() { return type; }    
    string getValue() { return buffer; }
    void changeType(DataType type) { this->type = type; }
    void add(char c) { buffer += c; }
    int toInt() {
        if (type != DataType::AQ_INTEGER) {
            throw("ERROR: DIT CON ME MAY INT!");
        } 

        int a = 0;
        for (auto& i : buffer) {
            a *= 10;
            a += i - '0';
        }
        return a;
    }
    double toFloat() {
        if (type != DataType::AQ_FLOAT) {
            throw("ERROR: DIT CON ME MAY FLOAT!");
        } 
        float a = 0;
        int decimal = 0;
        for (auto& i : buffer) {
            if (i == '.') decimal = true;
            else {
                if (!decimal) {
                    a *= 10;
                    a += i - '0';
                } else {
                    a += (i - '0') * pow(10, -decimal);
                    decimal++; 
                }  
            }
        }
        return a;
    }
    string toString() {
        if (type != DataType::STRING) {
            throw("ERROR: DIT CON ME MAY STRING!");
        }
        return buffer;
    }
};

static const vector<char> Miscellaneous({'<', '>', '+', '-', '*', '(', ')', '{', '}', '[', ']', ','});
bool isMiscellaneous(char symbol) {
    for (const auto& i : Miscellaneous) if (symbol == i) return true;
    return false;
}

class Lexer {
    string input;
    vector<Token*> tokens;
    BufferValue* buffer;

    public:
        Lexer(string input) : input(input) { 
            if (input[input.length() - 1] != '┴') input += '┴';
            tokens = vector<Token*>(); 
            buffer = nullptr; 
        }

        vector<Token*> tokenize() {
            MachineState currentState = MachineState::START;
            int i = 0;
            do {
                if (TEST) { cout << i << " " << currentState << endl; }
                int curr = input[i];
                if (currentState == MachineState::START) {
                    if (isalpha(curr)) {
                        currentState = MachineState::IDN;
                        buffer = new BufferValue(DataType::STRING);
                        i--;
                    } else if (isdigit(curr)) {
                        currentState = MachineState::AQ_INT;
                        buffer = new BufferValue(DataType::AQ_INTEGER);
                        i--;
                    } else if (curr == '=') {
                        tokens.push_back(new ElseToken("="));
                    } else if (curr == '\\') {
                        currentState = MachineState::WNLN;
                    } else if (curr == '\n') {
                        tokens.push_back(new NewlineToken("\n"));
                    } else if (curr == ':') {
                        currentState = MachineState::WASS;
                    } else if (curr == '!') {
                        currentState = MachineState::WNEQ;
                    } else if (curr == ' ') {
  
                    } else if (curr == ';') {
                        tokens.push_back(new DelimiterToken(";"));
                    } else if (isMiscellaneous(curr)) {
                        tokens.push_back(new ElseToken(to_string(curr)));
                    } else if (curr == '┴') {
                        currentState = MachineState::END;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::IDN) {
                    if (isalnum(curr)) {
                        buffer->add(curr);
                    } else if (curr == ' ' || curr == ';' || curr == ':' || curr == '!' || curr == '=' || isMiscellaneous(curr)) {
                        tokens.push_back(new IdentificatorToken(buffer->toString()));
                        i--;
                        delete buffer;
                        currentState = MachineState::START;
                    } else if (curr == '┴') {
                        tokens.push_back(new IdentificatorToken(buffer->toString()));
                        i--;
                        delete buffer;
                        currentState = MachineState::END;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::AQ_INT) {
                    if (isdigit(curr)) {
                        buffer->add(curr);
                    } else if (curr == '.') {
                        buffer->add(curr);
                        currentState = MachineState::FLT;
                        buffer->changeType(DataType::AQ_FLOAT);
                    } else if (curr == ' ' || curr == ';' || curr == ':' || curr == '!' || curr == '=' || curr == '\\' || isMiscellaneous(curr)) {
                        tokens.push_back(new IntegerToken(buffer->toInt()));
                        i--;
                        delete buffer;
                        currentState = MachineState::START;
                    } else if (curr == '\n') {
                        tokens.push_back(new NewlineToken("\n"));
                    } else if (curr == '┴') {
                        tokens.push_back(new IntegerToken(buffer->toInt()));
                        i--;
                        delete buffer;
                        currentState = MachineState::END;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::WDM) {
                    if (isdigit(curr)) {
                        buffer->add(curr);
                        currentState = MachineState::FLT;
                    } if (curr == '┴') {
                        currentState = MachineState::ERR;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::FLT) {
                    if (isdigit(curr)) {
                        buffer->add(curr);
                    } else if (curr == ' ' || curr == ';' || curr == ':' || curr == '!' || curr == '=' || curr == '\\' || isMiscellaneous(curr)) {
                        tokens.push_back(new FloatToken(buffer->toFloat()));
                        i--;
                        delete buffer;
                        currentState = MachineState::START;
                    } else if (curr == '┴') {
                        tokens.push_back(new FloatToken(buffer->toFloat()));
                        i--;
                        delete buffer;
                        currentState = MachineState::END;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::WASS) {
                    if (curr == '=') {
                        tokens.push_back(new AssignToken(":="));
                        currentState = MachineState::START;
                    } if (curr == '┴') {
                        currentState = MachineState::ERR;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::WNEQ) {
                    if (curr == '=') {
                        tokens.push_back(new ElseToken("!="));
                        currentState = MachineState::START;
                    } if (curr == '┴') {
                        currentState = MachineState::ERR;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::WNLN) {
                    if (curr == 'n') {
                        tokens.push_back(new NewlineToken("\n"));
                        currentState = MachineState::START;
                    } if (curr == '┴') {
                        currentState = MachineState::ERR;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::ERR) {         // ERROR CODE INCOMPLETE
                    cout << "ERROR AT i = " << i-1 << "(" << input[i-1] << ")" << endl;
                    break;
                } else if (currentState == MachineState::END) {
                    break;
                }
                i++;
            } while (i < input.length());
            //delete buffer;
            return tokens;
        }            
};