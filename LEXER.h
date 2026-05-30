#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

enum MachineState {
    START, 
    IDN, 
    INT, 
    MTS, 
    FLT, 
    WASS, 
    WNEQ, 
    WNLN, 
    MIS, 
    ERR
};

enum TokenType {
    IDENTIFICATOR,
    INTERGER,
    FLOAT,
    ASSIGN,
    ELSE,
    DELIMITER,
    NEWLINE
};

class Token {
    public:
        TokenType type;
        string value;
        Token(TokenType type, string value) {
            this->type = type;
            this->value = value;
        }
};




enum DataType {
    STRING,
    INT,
    FLOAT,

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
    void add(char c) { buffer += c; }
    int toInt() {
        if (type != DataType::INT) {
            throw("ERROR: DIT CON ME MAY INT!");
        } 

        int a = 0;
        for (auto& i : buffer) {
            a += i - '0';
            a *= 10;
        }
    }
    double toFloat() {
        if (type != DataType::INT) {
            throw("ERROR: DIT CON ME MAY FLOAT!");
        } 
        float a = 0;
        int decimal = 0;
        for (auto& i : buffer) {
            if (i == '.') decimal = true;
            else {
                if (!decimal) {
                    a += i - '0';
                    a *= 10;
                } else {
                    a += pow((i - '0'), -decimal);
                    decimal++; 
                }  
            }
        }
    }
    string toString() {
        if (type != DataType::INT) {
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
    vector<Token> tokens;
    BufferValue* buffer;

    public:
        Lexer(string input) : input(input) { tokens = vector<Token>();}
        vector<Token> tokenize() {
            MachineState currentState = MachineState::START;
            for (int i = 0; i < input.length(); i++) {
                int curr = input[i];
                if (currentState == MachineState::START) {
                    if (isalpha(curr)) {
                        currentState = MachineState::IDN;
                        buffer = new BufferValue(DataType::STRING);
                    } else if (isdigit(curr)) {
                        currentState = MachineState::INT;
                        buffer = new BufferValue(DataType::INT);
                    } else if (curr == '=') {
                        tokens.push_back(Token(TokenType::ELSE, "="));
                    } else if (curr == '\\') {
                        currentState = MachineState::WNLN;
                    } else if (curr == ':') {
                        currentState = MachineState::WASS;
                    } else if (curr == '!') {
                        currentState = MachineState::WNEQ;
                    } else if (curr == ' ') {
                        continue;
                    } else if (curr == ';') {
                        tokens.push_back(Token(TokenType::DELIMITER, ";"));
                    } else if (isMiscellaneous(curr)) {
                        tokens.push_back(Token(TokenType::ELSE, to_string(curr)));
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::IDN) {
                    if (isalnum(curr)) {
                        buffer->add(curr);
                    } else if (curr == ' ' || curr == ';' || curr == ':' || curr == '!' || curr == '=' || curr == '\\' || isMiscellaneous(curr)) {
                        tokens.push_back(Token(TokenType::IDENTIFICATOR, buffer->toString()));
                        i--;
                        delete buffer;
                    } else {
                        currentState = MachineState::ERR;
                    }
                } else if (currentState == MachineState::INT) {
                    if (isdigit(curr) || curr == '.') {
                        buffer->add(curr);
                    } else if (curr == ' ' || curr == ';' || curr == ':' || curr == '!' || curr == '=' || curr == '\\' || isMiscellaneous(curr)) {
                        if (buffer->getType() == DataType::INT) tokens.push_back(Token(TokenType::INTERGER, buffer->toInt()));
                    }
                }
            
                    case MachineState::INT: 
                    case MachineState::MTS: 
                    case MachineState::FLT: 
                    case MachineState::WASS: 
                    case MachineState::WNEQ: 
                    case MachineState::WNLN: 
                    case MachineState::ERR:

                }
            }

            return tokens;
        }
};