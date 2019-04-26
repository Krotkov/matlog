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
    Token curToken;
    string variable;
    int uk;

    void skipWhiteSpaces(string const & expression);

    Token nextToken(string const & expression);

    Expression* unary(string const & expression);

    Expression* andParse(string const & expression);

    Expression* orParse(string const & expression);

    Expression* implicationParse(string const & expression);

public:
    Expression* parse(string const & expression);
};

#endif //MATLOG2_PARSER_H
