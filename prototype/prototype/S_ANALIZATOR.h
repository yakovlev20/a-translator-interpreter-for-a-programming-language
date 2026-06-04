#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <memory>

#include <C:\репозитории\a-translator-interpreter-for-a-programming-language\COMBILYATOR\Token.h>


class SemanticAnalyzer {
private:
    std::stack<int> labelStack;
    std::vector<std::string> rpn;
    std::unordered_map<std::string, std::vector<int>> arrays;
    bool arrayMode = false;

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
            // Разбиваем строку на токены (упрощённая реализация)
            auto tokens = tokenize(codeLine);

            if (tokens.empty()) return true; // Пустая строка — корректна

            // Анализируем последовательность токенов
            return validateSyntax(tokens);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка анализа: " << e.what() << "\n";
            return false;
        }
    }

private:
    // Упрощённый токенизатор для демонстрации
    std::vector<std::unique_ptr<Token>> tokenize(const std::string& line) {
        std::vector<std::unique_ptr<Token>> tokens;

        // Здесь должна быть полноценная реализация токенизатора,
        // которая разбирает строку на объекты Token разных типов
        // Для примера — заглушка:

        // Пример логики:
        // - если строка начинается с "array", создаём IdentificatorToken
        // - числа преобразуем в IntegerToken/FloatToken
        // - операторы — в соответствующие токены

        // В реальной реализации нужно использовать конечный автомат
        // или библиотеку для парсинга

        return tokens;
    }

    // Проверка синтаксиса последовательности токенов
    bool validateSyntax(const std::vector<std::unique_ptr<Token>>& tokens) {
        if (tokens.empty()) return true;

        const auto& firstToken = tokens[0];

        // Проверка объявления массива
        if (dynamic_cast<IdentificatorToken*>(firstToken.get())) {
            auto identToken = static_cast<IdentificatorToken*>(firstToken.get());
            if (identToken->getValue() == "array") {
                return handleArrayDeclaration(tokens);
            }
        }

        // Проверка операций с массивами
        if (arrayMode) {
            return handleArrayOperations(tokens);
        }

        // Общая проверка синтаксиса
        return checkGeneralSyntax(tokens);
    }

    bool handleArrayDeclaration(const std::vector<std::unique_ptr<Token>>& tokens) {
        if (tokens.size() < 3) {
            std::cerr << "Ошибка: Неверное объявление массива\n";
            return false;
        }

        auto arrayNameToken = dynamic_cast<IdentificatorToken*>(tokens[1].get());
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
            if (auto intToken = dynamic_cast<IntegerToken*>(tokens[i].get())) {
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

    bool handleArrayOperations(const std::vector<std::unique_ptr<Token>>& tokens) {
        // Логика обработки операций с массивами (доступ по индексу, присваивание и т.д.)
        // Проверки границ, типов и т.п.
        return true; // Заглушка
    }

    bool checkGeneralSyntax(const std::vector<std::unique_ptr<Token>>& tokens) {
        // Общие проверки синтаксиса: соответствие скобок, операторов и т.д.
        return true; // Заглушка
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
        arrayMode = false;
    }
};