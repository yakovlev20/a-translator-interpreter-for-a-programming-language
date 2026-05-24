#include <iostream>
#include <vector>
#include <string>
#include "tokentype.h"

using namespace std;

class Token {
public:
    TokenType type;
    string value;
    Token(TokenType t, const string& v) : type(t), value(v) {}

}


class Lexer
{
public:

    vector<string> tokenize(string code)
    {
        do {
            
        } while 
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