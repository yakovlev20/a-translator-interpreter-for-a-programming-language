#include <iostream>
#include "LEXER.h"
#include "Token.h"
#include <string>
#include <vector>

using namespace std;

//надо добавить скопы 
int main() {
    string code = "var a = 10;\n var b = 11;\n var c = a + b;\n";
    Lexer l(code);
    vector<Token*> token = l.tokenize();
    
    for (auto& i : token) {
        cout << *i;
    }
    return 0;
}