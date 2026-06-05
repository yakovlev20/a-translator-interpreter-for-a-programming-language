#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\Token.h"
#include "C:\репозитории\a-translator-interpreter-for-a-programming-language\prototype\prototype\LEXER.h"

class SemanticAnalyzer {
private:
    std::stack<int> labelStack;
    std::vector<std::string> rpn;
    std::unordered_map<std::string, std::vector<int>> arrays;

    // Вспомогательная функция для проверки синтаксиса идентификатора
    bool isValidIdentifier(const std::string& name) const {
        if (name.empty()) return false;
        if (!std::isalpha(name[0]) && name[0] != '_') return false;
        for (size_t i = 1; i < name.size(); ++i) {
            if (!std::isalnum(name[i]) && name[i] != '_') return false;
        }
        return true;
    }

public:
    // Основной метод: принимает строку кода и проверяет её корректность
    bool analyze(const std::string& codeLine) {
        try {
            // Используем готовый лексер вместо заглушки
            Lexer lexer(codeLine);
            auto tokens = lexer.tokenize();
            printf_s('OK')
                if (tokens.empty()) return true; // Пустая строка — корректна

            // Анализируем последовательность токенов
            bool result = validateSyntax(tokens);

            // Освобождаем память от токенов
            for (auto* token : tokens) {
                delete token;
            }

            return result;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка анализа: " << e.what() << "\n";
            return false;
        }
    }

    // Функция для чтения файла и преобразования в одну строку
    std::string readFileToSingleLine(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл: " + filename);
        }

        std::stringstream buffer;
        std::string line;
        bool first = true;

        while (std::getline(file, line)) {
            if (!first) {
                buffer << "\\n";  // добавляем \n между строками
            }
            buffer << line;
            first = false;
        }

        file.close();
        return buffer.str();
    }

private:
    // Проверка синтаксиса последовательности токенов
    bool validateSyntax(const std::vector<Token*>& tokens) {
        if (tokens.empty()) return true;

        const auto& firstToken = tokens[0];

        // Проверка объявления массива
        if (auto identToken = dynamic_cast<IdentificatorToken*>(firstToken)) {
            if (identToken->getValue() == "array") {
                return handleArrayDeclaration(tokens);
            }
        }

        // Общая проверка синтаксиса
        return checkGeneralSyntax(tokens);
    }

    bool handleArrayDeclaration(const std::vector<Token*>& tokens) {
        if (tokens.size() < 4) { // array ID [ INT ]
            std::cerr << "Ошибка: Неверное объявление массива\n";
            return false;
        }

        if (tokens.size() < 3) {
            std::cerr << "Ошибка: Неверное объявление массива\n";
            return false;
        }

        auto arrayNameToken = dynamic_cast<IdentificatorToken*>(tokens[1]);
        if (!arrayNameToken) {
            std::cerr << "Ошибка: Ожидалось имя массива\n";
            return false;
        }

        std::string arrayName = arrayNameToken->getValue();
        if (!isValidIdentifier(arrayName)) {
            std::cerr << "Ошибка: Некорректное имя массива '" << arrayName << "'\n";
            return false;
        }

        if (arrays.find(arrayName) != arrays.end()) {
            std::cerr << "Ошибка: Массив '" << arrayName << "' уже объявлен\n";
            return false;
        }

        // Поиск размера массива (после '[' и до ']')
        bool foundSize = false;
        int size = 0;
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (auto intToken = dynamic_cast<IntegerToken*>(tokens[i])) {
                size = intToken->getValue();
                foundSize = true;
                break;
            }
        }

        if (!foundSize) {
            std::cerr << "Ошибка: Не указан размер массива\n";
            return false;
        }

        arrays[arrayName] = std::vector<int>(size, 0);
        rpn.push_back("ARRAY_" + arrayName);
        rpn.push_back("ALLOC_" + arrayName + "_" + std::to_string(size));
        return true;
    }

    bool checkGeneralSyntax(const std::vector<Token*>& tokens) {
        // Общие проверки синтаксиса: соответствие скобок, операторов и т.д.
        std::stack<TokenType> bracketStack;

        for (const auto* token : tokens) {
            if (auto elseToken = dynamic_cast<ElseToken*>(token)) {
                std::string value = elseToken->getValue();
                if (value == "(" || value == "[" || value == "{") {
                    bracketStack.push(token->type);
                }
                else if (value == ")") {
                    if (bracketStack.empty() || bracketStack.top() != TokenType::ELSE) {
                        std::cerr << "Ошибка: Несбалансированные скобки\n";
                        return false;
                    }
                    bracketStack.pop();
                }
                // Аналогично для других типов скобок
            }
        }

        if (!bracketStack.empty()) {
            std::cerr << "Ошибка: Несбалансированные скобки\n";
            return false;
        }

        return true;
    }

    void updateLabel(int oldLabel, int newLabel) {
        std::string oldLabelStr = "LABEL_" + std::to_string(oldLabel);
        std::string newLabelStr = "LABEL_" + std::to_string(newLabel);

        for (auto& item : rpn) {
            if (item == oldLabelStr) {
                item = newLabelStr;
            }
        }
    }

public:
    void printRPN() const {
        std::cout << "ОПЗ:\n";
        for (const auto& item : rpn) {
            std::cout << item << "\n";
        }
    }

    void reset() {
        while (!labelStack.empty()) labelStack.pop();
        rpn.clear();
        arrays.clear();
    }
};