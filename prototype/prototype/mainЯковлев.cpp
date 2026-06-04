// prototype.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <stack>

#include <C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\PARSER.h>
#include <C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\S_ANALIZATOR.h>


int main() {

    setlocale(LC_ALL, "Russian");

    std::cout << "\nСемантический анализатор: \n";
    SemanticAnalyzer analyzer;

    // Примеры строк для анализа
    std::vector<std::string> testLines = {
        "array myArray[10];",
        "myArray[0] = 5;",
        "invalid code here",
        
    };

    for (const auto& line : testLines) {
        std::cout << "Анализируем: " << line << "\n";
        bool isValid = analyzer.analyze(line);
        std::cout << (isValid ? "КОРРЕКТНО" : "ОШИБКА") << "\n\n";
    }

    analyzer.printRPN();

    std::cout << "\nПарсер: \n";
    Parser analyzer1;

    analyzer1.program4(100);           // Добавляем метку 100 в стек
    analyzer1.program1(200);           // Добавляем метку 200 и условный переход
    analyzer1.program2(300);          // Обновляем метку, добавляем метку 300 и безусловный переход
    analyzer1.program3(400);          // Обновляем верхнюю метку в стеке до 400
    analyzer1.program5(50);           // Вычисляем метку 52 (50+2), обновляем предыдущую, добавляем переход

    analyzer1.program7();              // Переключаемся в режим массивов
    analyzer1.program8("myArray");     // Объявляем массив "myArray"
    analyzer1.program9("myArray", 10); // Выделяем для него 10 элементов

    analyzer1.printRPN();              // Выводим итоговую ОПС
    
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
