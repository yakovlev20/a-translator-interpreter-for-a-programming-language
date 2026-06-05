#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

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
        Token(TokenType type) {
            this->type = type;
        }

        virtual ~Token() = default;

        virtual void print(ostream& os) const {
            os << std::left << std::setw(25) << "Token: ?" << "Value: ?\n";
        }

        friend ostream& operator<<(ostream& os, const Token& t) {
            t.print(os);
            return os;
        }
};

class IdentificatorToken : public Token {
    public:
        string value;
        IdentificatorToken(string value) : Token(TokenType::IDENTIFICATOR) {
            this->value = value;
        }

        string getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: IDENTIFICATOR" << "Value: "<< value << endl;
        }
};

class IntegerToken : public Token {
    public:
        int value;
        IntegerToken(int value) : Token(TokenType::INTERGER) {
            this->value = value;
        }

        int getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: INTEGER" << "Value: "<< value << endl;
        }
};

class FloatToken : public Token {
    public:
        double value;
        FloatToken(double value) : Token(TokenType::FLOAT) {
            this->value = value;
        }

        double getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: FLOAT" << "Value: "<< value << endl;
        }
};

class AssignToken : public Token {
    public:
        string value;
        AssignToken(string value) : Token(TokenType::ASSIGN) {
            this->value = value;
        }

        string getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: ASSIGN" << "Value: "<< value << endl;
        }
};

class ElseToken : public Token {
    public:
        string value;
        ElseToken(string value) : Token(TokenType::ELSE) {
            this->value = value;
        }

        string getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: ELSE" << "Value: "<< value << endl;
        }
};

class DelimiterToken : public Token {
    public:
        string value;
        DelimiterToken(string value) : Token(TokenType::DELIMITER) {
            this->value = value;
        }

        string getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: DELIMITER" << "Value: "<< value << endl;
        }
};

class NewlineToken : public Token {
    public:
        string value;
        NewlineToken(string value) : Token(TokenType::NEWLINE) {
            this->value = value;
        }

        string getValue() {
            return value;
        }

        void print(ostream& os) const override {
            os << std::left << std::setw(25) << "Token: NEWLINE" << "Value: NULL" << endl;
        }
};

