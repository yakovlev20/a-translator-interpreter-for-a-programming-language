#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "Token.h"

enum class ParserState {
	A, L0, A1, X, X1, X2, X3,
	E, S, H, G, Q, Q1, T, T1, F,
};

enum class Lemexe {

};


class Parser {
	vector<Token*> tokens;

public:
	Parser(vector<Token*> tokens) { this->tokens = tokens; }
	
	string Parse() {
		string RPN = "";
		ParserState state = ParserState::A;

		for (auto& token : tokens) {

		}
	}
};