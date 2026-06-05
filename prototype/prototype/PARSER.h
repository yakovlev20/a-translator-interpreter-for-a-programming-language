#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <unordered_map>
#include <sstream>
#include "Token.h"
#include "ParserToken.h"

class Parser {
private:
    struct Position { int row = 1; int col = 1; };
    const vector<string> keywords = {
        "var",
        "const",
        "array",
        "if",
        "else",
        "while",
        "begin",
        "end",
        "get",
        "print",
        "in",
        "out",
    };

public:
    vector<Token> tokens;
    vector<Poken> output;
    vector<Position> positions;
    int index = 0;
    int labelCounter = 0;
    bool hasError = false;


public:
    Parser(const vector<Token>& tokens) {
        for (const auto& token : tokens) {
            this->tokens.push_back(token);
        }
        buildPositions();
    }

    vector<Poken> parse() {
        skipNewlines();
        parseProgram();
        return output;
    }

private:
    bool hasNext() const {
        return index < tokens.size();
    }
    const Token& peek() const {
        return tokens[index];
    }
    const Token& advance() {
        return tokens[index++];
    }

    bool match(TokenType type, const string& text = "") const {
        if (!hasNext()) return false;
        const Token& t = peek();
        if (t.type != type) return false;
        if (text.empty()) return true;
        if (holds_alternative<string>(t.value)) {
            return get<string>(t.value) == text;
        }
        return false;
    }


    string tokenString(const Token& t) const {
        if (holds_alternative<string>(t.value)) {
            return get<string>(t.value);
        }
        return string();
    }
    int tokenInt(const Token& t) const {
        if (holds_alternative<int>(t.value)) {
            return get<int>(t.value);
        }
        return 0;
    }
    double tokenDouble(const Token& t) const {
        if (holds_alternative<double>(t.value)) {
            return get<double>(t.value);
        }
        return 0.0;
    }
    string tokenText(const Token& t) const {
        if (t.type == NEWLINE) {
            return "\n";
        }
        if (holds_alternative<string>(t.value)) {
            return get<string>(t.value);
        }
        if (holds_alternative<int>(t.value)) {
            return to_string(get<int>(t.value));
        }
        if (holds_alternative<double>(t.value)) {
            ostringstream oss;
            oss << get<double>(t.value);
            return oss.str();
        }
        return string();
    }
    int tokenLength(const Token& t) const {
        if (t.type == NEWLINE) return 0;
        return (int)tokenText(t).size();
    }
    void buildPositions() {
        positions.clear();
        positions.reserve(tokens.size());
        int row = 1;
        int col = 1;
        for (const auto& token : tokens) {
            positions.push_back({row, col});
            if (token.type == NEWLINE) {
                row++;
                col = 1;
            } else {
                col += tokenLength(token);
            }
        }
    }
    Position currentPosition() const {
        if (!positions.empty()) {
            if (index < (int)positions.size()) {
                return positions[index];
            }
            return positions.back();
        }
        return Position{1,1};
    }
    void reportError(const string& message) const {
        Position pos = currentPosition();
        cerr << "Parse error at row " << pos.row << " col " << pos.col << ": " << message << endl;
    }

    int newLabel() {
        return labelCounter++;
    }
    void skipNewlines() {
        while (hasNext() && peek().type == NEWLINE) {
            advance();
        }
    }

    void parseDelimiter() {
        skipNewlines();
        if (hasNext() && isSymbol(";")) {
            advance();
            return;
        }
        reportError("Expect ; after statement");
    }

    bool checkIdent(const string& word) const {
        return hasNext() && peek().type == IDENTIFICATOR && tokenString(peek()) == word;
    }

    bool isSymbol(const string& symbol) const {
        if (!hasNext()) return false;
        const auto& t = peek();
        return (t.type == ELSE || t.type == DELIMITER || t.type == ASSIGN) && tokenString(t) == symbol;
    }
    bool containsKeyword(string key) const {
        for (auto& i : keywords) if (key == i) return true;
        return false;
    }

    void expectKeyword(string outName) {
        if (!hasNext() || peek().type != IDENTIFICATOR || !containsKeyword(tokenString(peek()))) {
            reportError("Expected keyword: " + outName);
            return;
        }
        advance();
    }
    void expectIdentifier(string& outName) {
        if (!hasNext() || peek().type != IDENTIFICATOR) {
            reportError("Expected identifier");
            return;
        }
        outName = tokenString(peek());
        advance();
    }
    void expectSymbol(const string& symbol) {
        if (!isSymbol(symbol)) {
            reportError("Expected symbol: " + symbol);
            return;
        }
        advance();
    }

    void parseProgram() {
        expectKeyword("begin");
        expectSymbol("{"); 
        parseStatements();
        expectSymbol("}"); 
        expectKeyword("end");
    }
    void parseStatements() {

        skipNewlines();
        while (hasNext() && !isSymbol("}") && !checkIdent("else")) {
            parseStatement();
            if (hasNext() && !isSymbol("}") && !checkIdent("else")) {
                parseDelimiter();
            }
            skipNewlines();
        }
        skipNewlines();
    }

    void parseStatement() {
        if (!hasNext()) return;

        if (checkIdent("print")) {
            parsePrint();
            return;
        }
        if (checkIdent("get")) {
            parseGet();
            return;
        }
        if (checkIdent("if")) {
            parseIf();
            return;
        }
        if (checkIdent("while")) {                                //
            parseWhile();
            return;
        }
        if (checkIdent("var")) {
            parseVariable();
            return;
        }
        if (checkIdent("const")) {
            parseConstant();
            return;
        }
        if (checkIdent("array")) {
            parseArray();
            return;
        }
        if (peek().type == IDENTIFICATOR) {         //Bez var/const/array/... -> Prisvaivanie
            parseAssignment();
            return;
        }

        advance();
    }

    void parsePrint() {
        advance();
        string name;
        expectIdentifier(name);
        output.emplace_back(PT_ID, name);
        output.emplace_back(PT_PRINT, string("print"));
    }

    void parseGet() {
        advance();
        string name;
        expectIdentifier(name);
        output.emplace_back(PT_ID, name);
        output.emplace_back(PT_GET, string("get"));
    }

    void parseIf() {
        advance();
        expectSymbol("(");
        parseCondition();
        expectSymbol(")");

        int falseLabel = newLabel();
        output.emplace_back(PT_JF, falseLabel);

        expectSymbol("{");
        parseStatements();
        expectSymbol("}");

        if (checkIdent("else")) {
            advance();
            expectSymbol("{");
            int endLabel = newLabel();
            output.emplace_back(PT_J, endLabel);
            output.emplace_back(PT_LABEL, falseLabel);
            parseStatements();
            output.emplace_back(PT_LABEL, endLabel);
            expectSymbol("}");
        } else {
            output.emplace_back(PT_LABEL, falseLabel);
        }
    }

    void parseWhile() {
        advance();
        expectSymbol("(");

        int startLabel = newLabel();
        output.emplace_back(PT_LABEL, startLabel);

        parseCondition();
        expectSymbol(")");

        int endLabel = newLabel();
        output.emplace_back(PT_JF, endLabel);

        expectSymbol("{");
        parseStatements();
        output.emplace_back(PT_J, startLabel);
        output.emplace_back(PT_LABEL, endLabel);
        expectSymbol("}");
    }

    void parseVariable() {
        advance();
        string name;
        expectIdentifier(name);

        output.emplace_back(PokenType::PT_VAR_DECL, name);
        if (isSymbol("[")) reportError("Unexpected pointer indexing");
        if (match(ASSIGN, ":=")) {
            advance();
            output.emplace_back(PT_ID, name);
            parseExpression();
            output.emplace_back(PT_ASSIGN, string("assign"));
        }
    }
    void parseConstant() {
        advance();
        string name;
        expectIdentifier(name);

        output.emplace_back(PokenType::PT_CONST_DECL, name);

        expectSymbol(":=");
        advance();
        output.emplace_back(PT_ID, name);
        parseExpression();
        output.emplace_back(PT_ASSIGN, string("assign"));
    }
    void parseArray() {
        advance();
        string name;
        expectIdentifier(name);
        expectSymbol("[");
        int size = 0;
        if (!hasNext()) {
            reportError("Expected array size");
            return;
        }
        if (peek().type == INTERGER) {
            size = tokenInt(peek());
            advance();
        } else if (peek().type == IDENTIFICATOR) {
            // Dynamic array size by identifier is allowed syntactically.
            advance();
        } else {
            reportError("Array size must be integer or identifier");
            return;
        }
        expectSymbol("]");
        output.emplace_back(PT_ARRAY_DECL, name);
        output.emplace_back(PT_ARRAY_SIZE, size);
        output.emplace_back(PT_I, string("&I"));
    }

    void parseAssignment() {
        string name = tokenString(peek());
        advance();

        output.emplace_back(PT_ID, name);
        bool isArrayAssignment = false;
        if (isSymbol("[")) {
            isArrayAssignment = true;
            advance();
            parseExpression();
            expectSymbol("]");
        }

        if (!match(ASSIGN, ":=")) {
            reportError("Expected assignment operator :=");
            return;
        }
        advance();
        if (isArrayAssignment) {
            output.emplace_back(PT_I, string("&I"));
        }
        parseExpression();
        output.emplace_back(PT_ASSIGN, string("assign"));
    }
    void parseCondition() {
        parseExpression();
        if (!hasNext()) {
            reportError("Expected comparison operator");
            return;
        }
        string op;
        if (isSymbol("<") || isSymbol(">") || isSymbol("=") || isSymbol("!=")) {
            op = tokenString(peek());
            advance();
        } else {
            reportError("Expected comparison operator");
            return;
        }
        parseExpression();
        output.emplace_back(PT_OP, op);
    }
    void parseExpression() {
        parseTerm();
        while (hasNext() && (isSymbol("+") || isSymbol("-"))) {
            string op = tokenString(peek());
            advance();
            parseTerm();
            output.emplace_back(PT_OP, op);
        }
    }
    void parseTerm() {
        parseFactor();
        while (hasNext() && (isSymbol("*") || isSymbol("/"))) {
            string op = tokenString(peek());
            advance();
            parseFactor();
            output.emplace_back(PT_OP, op);
        }
    }
    void parseFactor() {
        if (isSymbol("(")) {
            advance();
            parseExpression();
            expectSymbol(")");
            return;
        }
        if (!hasNext()) {
            reportError("Unexpected end of expression");
            return;
        }
        if (peek().type == INTERGER) {
            int value = tokenInt(peek());
            advance();
            output.emplace_back(PT_INT, value);
            return;
        }
        if (peek().type == FLOAT) {
            double value = tokenDouble(peek());
            advance();
            output.emplace_back(PT_FLOAT, value);
            return;
        }
        if (peek().type == IDENTIFICATOR) {
            string name = tokenString(peek());
            advance();
            output.emplace_back(PT_ID, name);
            if (isSymbol("[")) {
                advance();
                parseExpression();
                expectSymbol("]");
                output.emplace_back(PT_I, string("&I"));
            }
            return;
        }

        reportError("Unexpected token in expression");
        return;
    }

    int extractIntFromLastValue() {
        if (output.empty()) {
            reportError("Expected size value on stack");
            return 0;
        }
        const Poken& last = output.back();
        if (last.type != PT_INT) {
            reportError("Array size must be integer");
            return 0;
        }
        return get<int>(last.value);
    }
};
