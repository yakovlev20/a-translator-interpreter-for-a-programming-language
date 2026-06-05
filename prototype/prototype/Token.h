#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <variant>

using namespace std;
using Value = std::variant<int, double, string>;

enum TokenType {
    IDENTIFICATOR,
    INTERGER,
    FLOAT,
    ASSIGN,
    ELSE,
    DELIMITER,
    NEWLINE,
    UNKNOWN,
};

class Token {
    const vector<string> TOKENS = {
        "IDENTIFICATOR",
        "INTERGER",
        "FLOAT",
        "ASSIGN",
        "ELSE",
        "DELIMITER",
        "NEWLINE",
        "?",
    };

    public:
        TokenType type;
        Value value;
        Token(TokenType type, Value value) {
            this->type = type;
            this->value = std::move(value);
        }

        virtual ~Token() = default;


        void print(ostream& os) const {
            os << "Token: " << std::left << std::setw(25) << TOKENS[type] << "Value: ";
            switch (value.index()) {
            case 0: os << *get_if<0>(&value); break;
            case 1: os << *get_if<1>(&value); break;
            default: os << *get_if<2>(&value); break;
            }
            os << endl;
        }

        friend ostream& operator<<(ostream& os, const Token& t) {
            t.print(os);
            return os;
        }

        
};

