//
// Created by krotk on 17.03.2019.
//

#include "parser.h"

void ExpressionParser::skipWhiteSpaces(string const &expression) {
    while (uk != expression.size() && (expression[uk] == ' ' || expression[uk] == '\n')) {
        uk++;
    }
}

ExpressionParser::Token ExpressionParser::nextToken(string const &expression) {
    skipWhiteSpaces(expression);
    if (uk == expression.size()) {
        return Token::END;
    }
    char cur = expression[uk];
    switch (cur) {
        case '(':
            uk++;
            return Token::OPEN_BRACKET;
        case ')':
            uk++;
            return Token::CLOSE_BRACKET;
        case '!':
            uk++;
            return Token::NOT;
        case '|':
            uk++;
            return Token::OR;
        case '&':
            uk++;
            return Token::AND;
        case '-':
            uk += 2;
            return Token::IMPLICATION;
        default:
            variable = "";
            while (uk != expression.size() && ((expression[uk] >= 'a' && expression[uk] <= 'z') ||
                                               (expression[uk] >= 'A' && expression[uk] <= 'Z') ||
                                               (expression[uk] >= '0' && expression[uk] <= '9') ||
                                               (expression[uk] == '\''))) {
                variable += expression[uk];
                uk++;
            }
            return Token::VARIABLE;
    }
}

Expression* ExpressionParser::unary(string const &expression) {
    Expression* ans;
    switch (curToken = nextToken(expression)) {
        case Token::VARIABLE:
            curToken = nextToken(expression);
            return new Expression(Type::VARIABLE, variable);
        case Token::NOT:
            return new Expression(Type::NOT, unary(expression), nullptr);
        case Token::OPEN_BRACKET:
            ans = implicationParse(expression);
            curToken = nextToken(expression);
            return ans;
    }
}

Expression* ExpressionParser::andParse(string const &expression) {
    Expression* first = unary(expression);
    while (true) {
        switch (curToken) {
            case Token::AND:
                first = new Expression(Type::AND, first, unary(expression));
                continue;
            default:
                return first;
        }
    }
}

Expression* ExpressionParser::orParse(string const &expression) {
    Expression* first = andParse(expression);
    while (true) {
        switch (curToken) {
            case Token::OR:
                first = new Expression(Type::OR, first, andParse(expression));
                continue;
            default:
                return first;
        }
    }
}

Expression* ExpressionParser::implicationParse(string const &expression) {
    Expression* first = orParse(expression);
    while (true) {
        switch (curToken) {
            case Token::IMPLICATION:
                first = new Expression(Type::IMPLICATION, first, implicationParse(expression));
                continue;
            default:
                return first;
        }
    }

}

Expression* ExpressionParser::parse(string const &expression) {
    uk = 0;
    curToken = Token::BEGIN;
    return implicationParse(expression);
}


