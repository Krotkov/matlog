//
// Created by krotk on 17.03.2019.
//

#ifndef MATLOG2_PARSER_H
#define MATLOG2_PARSER_H

#include <bits/stdc++.h>
#include "../expression/expression.h"

using namespace std;


class ExpressionParser {

    enum class Token {
        OPEN_BRACKET, CLOSE_BRACKET, OR, AND, IMPLICATION, END, NOT, VARIABLE, BEGIN,
    };

    static void skipWhiteSpaces(string const &expression, int &uk);

    static Token nextToken(string const &expression, string &varName, int &uk);

    static Expression *unary(string const &expression, Token &curToken, int &uk);

    static Expression *andParse(string const &expression, Token &curToken, int &uk);

    static Expression *orParse(string const &expression, Token &curToken, int &uk);

    static Expression *implicationParse(string const &expression, Token &curToken, int &uk);

public:
    static Expression *parse(string const &expression);
};

#endif //MATLOG2_PARSER_H
