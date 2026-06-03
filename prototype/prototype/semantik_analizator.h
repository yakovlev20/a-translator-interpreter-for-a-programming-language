#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

class SemanticAnalyzer {
private:
    std::stack<int> labelStack;        // Stack to store labels
    std::vector<std::string> rpn;     // RPN (Reverse Polish Notation) output
    std::unordered_map<std::string, std::vector<int>> arrays; // Map of arrays
    bool arrayMode = false;            // Flag indicating array processing mode

public:
    // Method 1: Start a program block and push a label onto the stack
    void program1(int k) {
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER"); // Placeholder for the label
        rpn.push_back("jf");             // Jump if false
    }

    // Method 2: Handle label and add jump instructions
    void program2(int k) {
        if (!labelStack.empty()) {
            int topLabel = labelStack.top();
            // Update the previous label
            updateLabel(topLabel, k);
        }
        labelStack.push(k);
        rpn.push_back("LABEL_PLACEHOLDER");
        rpn.push_back("j"); // Unconditional jump
    }

    // Method 3: Update the top label in the stack with the current value
    void program3(int k) {
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), k);
        }
    }

    // Method 4: Push a new label onto the label stack
    void program4(int k) {
        labelStack.push(k);
    }

    // Method 5: Create a new label (i+2) and add a jump instruction
    void program5(int i) {
        int newLabel = i + 2;
        if (!labelStack.empty()) {
            updateLabel(labelStack.top(), newLabel);
        }
        rpn.push_back("LABEL_" + std::to_string(newLabel));
        rpn.push_back("j");
    }

    // Method 6: Exit array processing mode
    void program6() {
        arrayMode = false;
    }

    // Method 7: Enter array processing mode
    void program7() {
        arrayMode = true;
    }

    // Method 8: Declare a new array
    bool program8(const std::string& arrayName) {
        // Check if the array already exists in the map
        if (arrays.find(arrayName) != arrays.end()) {
            std::cerr << "Error: Array " << arrayName << " already declared!\n";
            return false;
        }

        // If in array mode, create a new empty array and add it to RPN
        if (arrayMode) {
            arrays[arrayName] = std::vector<int>();
            rpn.push_back("ARRAY_" + arrayName);
            return true;
        }
        return false;
    }

    // Method 9: Allocate memory for an array with a given size
    void program9(const std::string& arrayName, int size) {
        auto it = arrays.find(arrayName);
        if (it != arrays.end()) {
            it->second.resize(size, 0); // Resize array with default values (zeros)
            rpn.push_back("ALLOC_" + arrayName + "_" + std::to_string(size));
        } else {
            std::cerr << "Error: Array " << arrayName << " not found!\n";
        }
    }

private:
    // Helper method: Update a label in RPN with a new value
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
    // Print the current RPN sequence
    void printRPN() const {
        std::cout << "RPN:\n";
        for (const auto& item : rpn) {
            std::cout << item << "\n";
        }
    }

    // Reset the analyzer state (clear all data structures)
    void reset() {
        while (!labelStack.empty()) labelStack.pop();
        rpn.clear();
        arrays.clear();
        arrayMode = false;
    }
};
