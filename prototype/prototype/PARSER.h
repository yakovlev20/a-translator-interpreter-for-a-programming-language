#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

class Parser {
private:
    std::stack<int> labelStack;        // Стек для хранения меток
    std::vector<std::string> rpn;     // Выход в обратной польской записи (ОПЗ)
    std::unordered_map<std::string, std::vector<int>> arrays; // Карта массивов
    bool arrayMode = false;            // Флаг, указывающий режим обработки массивов


public:
    // Метод 1: Начать программный блок и поместить метку в стек
    void program1(int k) {
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER"); // Заполнитель для метки
        rpn.push_back("jf");             // Переход, если ложь
    }

    // Метод 2: Обработать метку и добавить инструкции перехода
    void program2(int k) {
        if (!labelStack.empty()) {
            int topLabel = labelStack.top();
            // Обновить предыдущую метку
            updateLabel(topLabel, k);
        }
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER");
        rpn.push_back("j"); // Безусловный переход
    }

    // Метод 3: Обновить верхнюю метку в стеке текущим значением
    void program3(int k) {
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), k);
        }
    }

    // Метод 4: Поместить новую метку в стек меток
    void program4(int k) {
        labelStack.push(k);
    }

    // Метод 5: Создать новую метку (i+2) и добавить инструкцию перехода
    void program5(int i) {
        int newLabel = i + 2;
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), newLabel);
        }
        rpn.push_back("LABEL_" + std::to_string(newLabel));
        rpn.push_back("j");
    }

    // Метод 6: Выйти из режима обработки массивов
    void program6() {
        arrayMode = false;
    }

    // Метод 7: Войти в режим обработки массивов
    void program7() {
        arrayMode = true;
    }

    // Метод 8: Объявить новый массив
    bool program8(const std::string& arrayName) {
        // Проверить, существует ли массив уже в карте
        if (arrays.find(arrayName) != arrays.end()) {
            std::cerr << "Ошибка: Массив " << arrayName << " уже объявлен!\n";
            return false;
        }

        // Если в режиме обработки массивов, создать новый пустой массив и добавить его в ОПЗ
        if (arrayMode) {
            arrays[arrayName] = std::vector<int>();
            rpn.push_back("ARRAY_" + arrayName);
            return true;
        }
        return false;
    }

    // Метод 9: Выделить память для массива заданного размера
    void program9(const std::string& arrayName, int size) {
        auto it = arrays.find(arrayName);
        if (it != arrays.end()) {
            it->second.resize(size, 0); // Изменить размер массива с значениями по умолчанию (нули)
            rpn.push_back("ALLOC_" + arrayName + "_" + std::to_string(size));
        }
        else {
            std::cerr << "Ошибка: Массив " << arrayName << " не найден!\n";
        }
    }

private:
    // Вспомогательный метод: Обновить метку в ОПЗ новым значением
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
    // Вывести текущую последовательность в ОПЗ
    void printRPN() const {
        std::cout << "ОПЗ:\n";
        for (const auto& item : rpn) {
            std::cout << item << "\n";
        }
    }

    // Сбросить состояние анализатора (очистить все структуры данных)
    void reset() {
        while (!labelStack.empty()) labelStack.pop();
        rpn.clear();
        arrays.clear();
        arrayMode = false;
    }
};