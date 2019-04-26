//
// Created by krotk on 17.03.2019.
//

#ifndef MATLOG2_EXPRESSION_H
#define MATLOG2_EXPRESSION_H

#include <bits/stdc++.h>

using namespace std;

enum class Type {
    IMPLICATION, AND, OR, NOT, VARIABLE, NOTHING
};

class Expression {

    Expression *first;
    Expression *second;

    Type type;

    string name;

public:
    Expression() {
        type = Type::NOTHING;
        first = nullptr;
        second = nullptr;
        name = "";
    }

    Expression(Type type, Expression *first, Expression *second) {
        this->type = type;
        this->first = first;
        this->second = second;
    }

    Expression(Type type, string &name) {
        this->type = type;
        this->name = name;
        first = nullptr;
        second = nullptr;
    }

    Type getType();

    Expression *getFirst();

    Expression *getSecond();

    string toString();

    friend bool recursiveEqualsWithAxiom(Expression *expression, Expression *axiom, Expression &valA, Expression &valB,
                                         Expression &valC);

    friend bool equals(Expression *first, Expression *other);

    friend bool equalsWithAxiom(Expression *expression, Expression *axiom);

    bool evaluate(bool A, bool B, bool C);

    bool getValue(bool first, bool second);

    string getName();

};

#endif //MATLOG2_EXPRESSION_H
