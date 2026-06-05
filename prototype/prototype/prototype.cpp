

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <stack>
#include <fstream>

#include "LEXER.h"
#include "PARSER.h"
#include "Token.h"
#include "ParserToken.h"

//#include "C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\Token.h"
//#include "C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\LEXER.h"

//#include <C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\PARSER.h>
//#include <C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\S_ANALIZATOR1.h>

using namespace std;

#define SHOW_CODE 1
#define SHOW_LEXER 1
#define SHOW_PARSER 1

static ifstream codeFile("inout.txt");


int main() {
    
    setlocale(LC_ALL, "Russian");

    string code = "var a = 10;\n var b = 11.5;\n var c = a + b * d;\n EY$@!*$)@#!*$#!%";
    Lexer l(code);
    vector<Token*> token = l.tokenize();

    for (auto& i : token) {
        cout << *i;
    }

    string code, line;
    if (codeFile.is_open()) {
        while (!codeFile.eof()) {
            getline(codeFile, line);
            code += line + "\n";
        }

        string code1 = "begin { array a[10]; a[0] := 10; a[1] := 1 + 2;}  end ";

        if (SHOW_CODE) cout << "Code: \n" << code1 << endl << endl << endl;
        Lexer l(code1);
        vector<Token> tokens = l.tokenize();

        if (SHOW_LEXER) for (auto& i : tokens) cout << i;

        if (SHOW_PARSER) cout << "----- RPN output -----" << endl;
        Parser parser(tokens);
        vector<Poken> rpn = parser.parse();
        if (SHOW_PARSER) for (auto& p : rpn) cout << p;

    }
    else {
        cout << "File is dead lmao";
    }

    std::cout << "\nСемантический анализатор: \n";
    SemanticAnalyzer analyzer;

    // Примеры строк для анализа
    std::vector<std::string> testLines = {
        "array myArray[10];",
        "myArray[0] = 5;",
        "invalid code here"
    };

    for (const auto& line : testLines) {
        std::cout << "Анализируем: " << line << "\n";
        bool isValid = analyzer.analyze(line);
        std::cout << (isValid ? "КОРРЕКТНО" : "ОШИБКА") << "\n\n";
    }

    // Путь к файлу с кодом для анализа
    std::string filename = "C:\репозитории\a-translator-interpreter-for-a-programming-language\TESTS\inout.txt";

    try {
        // Читаем файл и преобразуем в одну строку через метод анализатора
        std::string singleLineCode = analyzer.readFileToSingleLine(filename);

        std::cout << "Исходный код (в одной строке):\n" << singleLineCode << "\n\n";

        // Анализируем всю строку
        std::cout << "Анализируем: " << singleLineCode << "\n";
        bool isValid = analyzer.analyze(singleLineCode);
        std::cout << (isValid ? "КОРРЕКТНО" : "ОШИБКА") << "\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
