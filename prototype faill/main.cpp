#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "tokentype.h"

using namespace std;

enum TokenType {
    ID,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    ASSIGN,
    DELIMITER,
    ELSE
};


class Token {
public:
    TokenType type;
    string value;
    Token(TokenType t, const string& v = "") : type(t), value(v) {}

};


class Lexer
{
public:
    vector<Token> tokenize(string code)
    {

        for (int i = 0; i < code.length(); i++) {
            char c = code[i];
            if (isalpha(c)) {
                
}
            }

        }
    }

private:
    string code;
};

Lexer lexer;

int main()
{
    const string code = "begin { int x = 10; x = x + 1; out(x);}";
    vector<string> tokens = lexer.tokenize(code);
    
}