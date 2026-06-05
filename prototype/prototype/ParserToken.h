#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <iomanip>

using namespace std;
using ParserValue = variant<int, double, string>;

enum PokenType {
    PT_ID,
    PT_INT,
    PT_FLOAT,
    PT_PRINT,
    PT_GET,
    PT_VAR_DECL,
    PT_CONST_DECL,
    PT_ARRAY_DECL,
    PT_ASSIGN,
    PT_JF,
    PT_J,
    PT_LABEL,
    PT_OP,
    PT_UNKNOWN,
    PT_ARRAY_SIZE,
    PT_I,
};

class Poken {
public:
    PokenType type;
    ParserValue value;

    Poken(PokenType type) : type(type), value(std::string()) {}
    Poken(PokenType type, ParserValue value) : type(type), value(std::move(value)) {}
    virtual ~Poken() = default;

    void print(std::ostream& os) const {
        static const std::vector<std::string> NAMES = {
            "ID",
            "INT",
            "FLOAT",
            "PRINT",
            "GET",
            "VAR_DECL",
            "CONST_DECL",
            "ARRAY_DECL",
            "ASSIGN",
            "JF",
            "J",
            "LABEL",
            "OP",
            "UNKNOWN",
            "ARRAY_SIZE",
            "I",
        };

        const std::string& name = (type >= 0 && (size_t)type < NAMES.size()) ? NAMES[type] : std::string("<INVALID>");
        os << std::left << std::setw(25) << name << " ";
        switch (value.index()) {
        case 0:
            os << std::get<int>(value);
            break;
        case 1:
            os << std::get<double>(value);
            break;
        case 2:
            os << std::get<std::string>(value);
            break;
        default:
            os << "?";
            break;
        }
        os << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Poken& p) {
        p.print(os);
        return os;
    }
};

