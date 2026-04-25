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

// Типы данных
enum ValueType {
    INTEGER,
    REAL,
    ARRAY
};

// Структура для значения переменной
struct Value {
    ValueType type;
    int intValue;
    double realValue;
    std::vector<double> arrayValue; // Для массива
    int arraySize;

    Value() : type(INTEGER), intValue(0), realValue(0.0), arraySize(0) {}

    Value(int val) : type(INTEGER), intValue(val), realValue(0.0), arraySize(0) {}

    Value(double val) : type(REAL), intValue(0), realValue(val), arraySize(0) {}

    Value(const std::vector<double>& arr, int size)
        : type(ARRAY), intValue(0), realValue(0.0), arrayValue(arr), arraySize(size) {
    }
};

// Класс для управления переменными
class VariableManager {
private:
    std::map<std::string, Value> variables;

public:
    bool exists(const std::string& name) const {
        return variables.find(name) != variables.end();
    }

    Value get(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable '" + name + "' not defined");
    }

    void set(const std::string& name, const Value& val) {
        variables[name] = val;
    }

    void printAll() const {
        for (const auto& pair : variables) {
            std::cout << pair.first << ": ";
            if (pair.second.type == INTEGER) {
                std::cout << pair.second.intValue;
            }
            else if (pair.second.type == REAL) {
                std::cout << pair.second.realValue;
            }
            else if (pair.second.type == ARRAY) {
                std::cout << "[";
                for (size_t i = 0; i < pair.second.arrayValue.size(); ++i) {
                    std::cout << pair.second.arrayValue[i];
                    if (i < pair.second.arrayValue.size() - 1) std::cout << ", ";
                }
                std::cout << "]";
            }
            std::cout << std::endl;
        }
    }
};

// Класс для лексического анализа (разбиения на токены)
class Lexer {
private:
    std::string input;
    size_t position;

public:
    Lexer(const std::string& inp) : input(inp), position(0) {}

    // Типы токенов
    enum TokenType {
        NUMBER,
        IDENTIFIER,
        OPERATOR,
        COMPARISON,
        KEYWORD,
        DELIMITER,
        END
    };

    struct Token {
        TokenType type;
        std::string value;

        Token(TokenType t, const std::string& v) : type(t), value(v) {}
        Token() : type(END), value("") {}
    };

    Token getNextToken() {
        if (position >= input.length()) {
            return Token(END, "");
        }

        // Пропускаем пробелы
        while (position < input.length() && std::isspace(input[position])) {
            position++;
        }

        if (position >= input.length()) {
            return Token(END, "");
        }

        char current = input[position];

        // Числа
        if (std::isdigit(current) || current == '.') {
            std::string num;
            while (position < input.length() &&
                (std::isdigit(input[position]) || input[position] == '.')) {
                num += input[position];
                position++;
            }
            return Token(NUMBER, num);
        }

        // Идентификаторы (имена переменных)
        if (std::isalpha(current) || current == '_') {
            std::string ident;
            while (position < input.length() &&
                (std::isalnum(input[position]) || input[position] == '_')) {
                ident += input[position];
                position++;
            }

            // Проверка на ключевые слова
            if (ident == "if" || ident == "while" || ident == "print" ||
                ident == "input" || ident == "array") {
                return Token(KEYWORD, ident);
            }
            return Token(IDENTIFIER, ident);
        }

        // Операторы и сравнения
        if (current == '+' || current == '-' || current == '*' || current == '/' ||
            current == '(' || current == ')' || current == '=' || current == '[' ||
            current == ']' || current == ';' || current == ',') {
            position++;
            return Token(DELIMITER, std::string(1, current));
        }

        // Операторы сравнения
        if (current == '<' || current == '>' || current == '!') {
            std::string comp;
            comp += current;
            position++;

            if (position < input.length() && input[position] == '=') {
                comp += input[position];
                position++;
            }
            return Token(COMPARISON, comp);
        }

        if (current == '=' && position + 1 < input.length() && input[position + 1] == '=') {
            position += 2;
            return Token(COMPARISON, "==");
        }

        // Ошибка - неизвестный символ
        throw std::runtime_error("Unexpected character: " + std::string(1, current));
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token;
        do {
            token = getNextToken();
            tokens.push_back(token);
        } while (token.type != END);
        return tokens;
    }
};

// Класс для синтаксического анализа и интерпретации
class Interpreter {
private:
    VariableManager vars;
    std::vector<Lexer::Token> tokens;  // Используем правильный тип Token
    size_t currentToken;

    // Получение текущего токена
    Lexer::Token peek() const {
        if (currentToken < tokens.size()) {
            return tokens[currentToken];
        }
        return Lexer::Token(Lexer::END, "");
    }

    Lexer::Token consume() {
        Lexer::Token token = peek();
        if (currentToken < tokens.size()) {
            currentToken++;
        }
        return token;
    }

    // Проверка типа токена
    bool check(Lexer::TokenType type) const {
        return peek().type == type;
    }

    bool check(const std::string& value) const {
        return peek().value == value;
    }

    // Парсинг выражений
    Value parseExpression() {
        return parseAddSub();
    }

    Value parseAddSub() {
        Value left = parseMulDiv();

        while (check(Lexer::DELIMITER) && (peek().value == "+" || peek().value == "-")) {
            std::string op = consume().value;
            Value right = parseMulDiv();

            if (left.type == INTEGER && right.type == INTEGER) {
                if (op == "+") left.intValue += right.intValue;
                else left.intValue -= right.intValue;
            }
            else {
                double leftVal = (left.type == INTEGER) ? left.intValue : left.realValue;
                double rightVal = (right.type == INTEGER) ? right.intValue : right.realValue;
                left.type = REAL;
                if (op == "+") left.realValue = leftVal + rightVal;
                else left.realValue = leftVal - rightVal;
            }
        }

        return left;
    }

    Value parseMulDiv() {
        Value left = parsePrimary();

        while (check(Lexer::DELIMITER) && (peek().value == "*" || peek().value == "/")) {
            std::string op = consume().value;
            Value right = parsePrimary();

            if (left.type == INTEGER && right.type == INTEGER) {
                if (op == "*") left.intValue *= right.intValue;
                else if (right.intValue != 0) left.intValue /= right.intValue;
                else throw std::runtime_error("Division by zero");
            }
            else {
                double leftVal = (left.type == INTEGER) ? left.intValue : left.realValue;
                double rightVal = (right.type == INTEGER) ? right.intValue : right.realValue;
                left.type = REAL;
                if (op == "*") left.realValue = leftVal * rightVal;
                else if (rightVal != 0) left.realValue = leftVal / rightVal;
                else throw std::runtime_error("Division by zero");
            }
        }

        return left;
    }

    Value parsePrimary() {
        Lexer::Token token = peek();

        if (token.type == Lexer::NUMBER) {
            consume();
            // Проверка, целое или вещественное
            if (token.value.find('.') != std::string::npos) {
                return Value(std::stod(token.value));
            }
            else {
                return Value(std::stoi(token.value));
            }
        }

        if (token.type == Lexer::IDENTIFIER) {
            std::string name = consume().value;

            // Проверка на массив
            if (check(Lexer::DELIMITER) && peek().value == "[") {
                consume(); // '['
                Value index = parseExpression();
                if (!check(Lexer::DELIMITER) || peek().value != "]") {
                    throw std::runtime_error("Expected ']' after array index");
                }
                consume(); // ']'

                Value arrayVal = vars.get(name);
                if (arrayVal.type != ARRAY) {
                    throw std::runtime_error("'" + name + "' is not an array");
                }

                int idx = (index.type == INTEGER) ? index.intValue : static_cast<int>(index.realValue);
                if (idx < 0 || idx >= arrayVal.arraySize) {
                    throw std::runtime_error("Array index out of bounds");
                }

                return Value(arrayVal.arrayValue[idx]);
            }

            return vars.get(name);
        }

        if (check(Lexer::DELIMITER) && token.value == "(") {
            consume(); // '('
            Value val = parseExpression();
            if (!check(Lexer::DELIMITER) || peek().value != ")") {
                throw std::runtime_error("Expected ')'");
            }
            consume(); // ')'
            return val;
        }

        throw std::runtime_error("Unexpected token in expression: " + token.value);
    }

    // Парсинг операторов
    void parseStatement() {
        Lexer::Token token = peek();

        if (token.type == Lexer::KEYWORD && token.value == "print") {
            consume();
            Value val = parseExpression();
            if (val.type == INTEGER) {
                std::cout << val.intValue << std::endl;
            }
            else if (val.type == REAL) {
                std::cout << val.realValue << std::endl;
            }
        }
        else if (token.type == Lexer::KEYWORD && token.value == "input") {
            consume();
            std::string varName = consume().value;

            std::string inputStr;
            std::cin >> inputStr;

            if (inputStr.find('.') != std::string::npos) {
                vars.set(varName, Value(std::stod(inputStr)));
            }
            else {
                vars.set(varName, Value(std::stoi(inputStr)));
            }
        }
        else if (token.type == Lexer::IDENTIFIER) {
            // Оператор присваивания
            std::string varName = consume().value;

            // Проверка на массив
            if (check(Lexer::DELIMITER) && peek().value == "[") {
                consume(); // '['
                Value index = parseExpression();
                if (!check(Lexer::DELIMITER) || peek().value != "]") {
                    throw std::runtime_error("Expected ']' after array index");
                }
                consume(); // ']'

                if (!check(Lexer::DELIMITER) || peek().value != "=") {
                    throw std::runtime_error("Expected '='");
                }
                consume(); // '='

                Value exprVal = parseExpression();

                Value arrayVal = vars.get(varName);
                if (arrayVal.type != ARRAY) {
                    throw std::runtime_error("'" + varName + "' is not an array");
                }

                int idx = (index.type == INTEGER) ? index.intValue : static_cast<int>(index.realValue);
                if (idx < 0 || idx >= arrayVal.arraySize) {
                    throw std::runtime_error("Array index out of bounds");
                }

                if (exprVal.type == INTEGER) {
                    arrayVal.arrayValue[idx] = exprVal.intValue;
                }
                else {
                    arrayVal.arrayValue[idx] = exprVal.realValue;
                }
                vars.set(varName, arrayVal);
            }
            else {
                // Обычное присваивание
                if (!check(Lexer::DELIMITER) || peek().value != "=") {
                    throw std::runtime_error("Expected '='");
                }
                consume(); // '='

                Value exprVal = parseExpression();
                vars.set(varName, exprVal);
            }
        }
        else if (token.type == Lexer::KEYWORD && token.value == "array") {
            consume();
            std::string varName = consume().value;

            if (!check(Lexer::DELIMITER) || peek().value != "[") {
                throw std::runtime_error("Expected '[' for array declaration");
            }
            consume(); // '['

            Lexer::Token sizeToken = consume();
            if (sizeToken.type != Lexer::NUMBER) {
                throw std::runtime_error("Expected array size");
            }
            int size = std::stoi(sizeToken.value);

            if (!check(Lexer::DELIMITER) || peek().value != "]") {
                throw std::runtime_error("Expected ']' after array size");
            }
            consume(); // ']'

            std::vector<double> arr(size, 0.0);
            vars.set(varName, Value(arr, size));
        }
        else {
            throw std::runtime_error("Unknown statement: " + token.value);
        }

        // Проверка конца оператора
        if (check(Lexer::DELIMITER) && peek().value == ";") {
            consume(); // ';'
        }
    }

public:
    Interpreter() : currentToken(0) {}

    void execute(const std::string& code) {
        Lexer lexer(code);
        tokens = lexer.tokenize();
        currentToken = 0;

        while (currentToken < tokens.size() && peek().type != Lexer::END) {
            parseStatement();
        }
    }
};

// Пример использования
int main() {
    Interpreter interpreter;

    // Пример программы на созданном языке
    std::string program =
        "array arr[5];\n"
        "arr[0] = 10;\n"
        "arr[1] = 20;\n"
        "x = arr[0] + arr[1];\n"
        "print x;\n"
        "input y;\n"
        "print y + x;\n";

    try {
        interpreter.execute(program);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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
