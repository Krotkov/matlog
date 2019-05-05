//
// Created by krotk on 17.03.2019.
//

#include "parser.h"

void ExpressionParser::skipWhiteSpaces(string const &expression, int& uk) {
    while (uk != expression.size() && (expression[uk] == ' ' || expression[uk] == '\n')) {
        uk++;
    }
}

ExpressionParser::Token ExpressionParser::nextToken(string const &expression, string& varName, int& uk) {
    skipWhiteSpaces(expression, uk);
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
            varName = "";
            while (uk != expression.size() && ((expression[uk] >= 'a' && expression[uk] <= 'z') ||
                                               (expression[uk] >= 'A' && expression[uk] <= 'Z') ||
                                               (expression[uk] >= '0' && expression[uk] <= '9') ||
                                               (expression[uk] == '\''))) {
                varName += expression[uk];
                uk++;
            }
            return Token::VARIABLE;
    }
}

Expression* ExpressionParser::unary(string const &expression, Token& curToken, int& uk) {
    Expression* ans;
    string varName;
    switch (curToken = nextToken(expression, varName, uk)) {
        case Token::VARIABLE:
            curToken = nextToken(expression, varName, uk);
            return new Expression(Type::VARIABLE, varName);
        case Token::NOT:
            return new Expression(Type::NOT, unary(expression, curToken, uk), nullptr);
        case Token::OPEN_BRACKET:
            ans = implicationParse(expression, curToken, uk);
            curToken = nextToken(expression, varName, uk);
            return ans;
        default:
            break;
    }
    return nullptr;
}

Expression* ExpressionParser::andParse(string const &expression, Token& curToken, int& uk) {
    Expression* first = unary(expression, curToken, uk);
    while (true) {
        switch (curToken) {
            case Token::AND:
                first = new Expression(Type::AND, first, unary(expression, curToken, uk));
                continue;
            default:
                return first;
        }
    }
}

Expression* ExpressionParser::orParse(string const &expression, Token& curToken, int& uk) {
    Expression* first = andParse(expression, curToken, uk);
    while (true) {
        switch (curToken) {
            case Token::OR:
                first = new Expression(Type::OR, first, andParse(expression, curToken, uk));
                continue;
            default:
                return first;
        }
    }
}

Expression* ExpressionParser::implicationParse(string const &expression, Token& curToken, int& uk) {
    Expression* first = orParse(expression, curToken, uk);
    while (true) {
        switch (curToken) {
            case Token::IMPLICATION:
                first = new Expression(Type::IMPLICATION, first, implicationParse(expression, curToken, uk));
                continue;
            default:
                return first;
        }
    }

}

Expression* ExpressionParser::parse(string const &expression) {
    int uk = 0;
    Token curToken = Token::BEGIN;
    return implicationParse(expression, curToken, uk);
}


