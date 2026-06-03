#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

setlocale(LC_ALL, "Russian");

class SemanticAnalyzer {
private:
    std::stack<int> labelStack;        // Магазин меток
    std::vector<std::string> rpn;     // ОПС (обратная польская запись)
    std::unordered_map<std::string, std::vector<int>> arrays; // Таблица массивов
    bool arrayMode = false;            // Режим записи массивов

public:
    // Программа 1: запись метки и условного перехода
    void program1(int k) {
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER"); // Пустое место для метки
        rpn.push_back("jf");             // Переход при false
    }

    // Программа 2: обновление метки и безусловный переход
    void program2(int k) {
        if (!labelStack.empty()) {
            int topLabel = labelStack.top();
            // Обновляем предыдущую метку
            updateLabel(topLabel, k);
        }
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER");
        rpn.push_back("j"); // Безусловный переход
    }

    // Программа 3: обновление метки через верхний элемент стека
    void program3(int k) {
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), k);
        }
    }

    // Программа 4: простая запись метки в магазин
    void program4(int k) {
        labelStack.push(k);
    }

    // Программа 5: вычисление метки (i+2) и безусловный переход
    void program5(int i) {
        int newLabel = i + 2;
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), newLabel);
        }
        rpn.push_back("LABEL_" + std::to_string(newLabel));
        rpn.push_back("j");
    }

    // Программа 6: переключение в режим переменных
    void program6() {
        arrayMode = false;
    }

    // Программа 7: переключение в режим массивов
    void program7() {
        arrayMode = true;
    }

    // Программа 8: обработка имени массива
    bool program8(const std::string& arrayName) {
        // Проверяем, есть ли массив в таблице
        if (arrays.find(arrayName) != arrays.end()) {
            std::cerr << "Ошибка: массив " << arrayName << " уже объявлен!\n";
            return false;
        }

        // Если в режиме массивов и имя не найдено — добавляем
        if (arrayMode) {
            arrays[arrayName] = std::vector<int>();
            rpn.push_back("ARRAY_" + arrayName);
            return true;
        }
        return false;
    }

    // Программа 9: выделение памяти для массива
    void program9(const std::string& arrayName, int size) {
        auto it = arrays.find(arrayName);
        if (it != arrays.end()) {
            it->second.resize(size, 0); // Инициализируем нулями
            rpn.push_back("ALLOC_" + arrayName + "_" + std::to_string(size));
        }
        else {
            std::cerr << "Ошибка: массив " << arrayName << " не найден!\n";
        }
    }

private:
    // Вспомогательная функция: обновление метки в ОПС
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
    // Вывод текущего состояния ОПС для отладки
    void printRPN() const {
        std::cout << "ОПС:\n";
        for (const auto& item : rpn) {
            std::cout << item << "\n";
        }
    }

    // Очистка состояния (для повторного использования)
    void reset() {
        while (!labelStack.empty()) labelStack.pop();
        rpn.clear();
        arrays.clear();
        arrayMode = false;
    }
};