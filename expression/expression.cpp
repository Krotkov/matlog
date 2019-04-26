//
// Created by krotk on 17.03.2019.
//

#include "expression.h"

bool equals(Expression *first, Expression *second) {
    if ((first == nullptr) ^ (second == nullptr)) return false;
    if (first == nullptr) return true;
    if (first->type != second->type) return false;
    if (first->type == Type::VARIABLE) {
        return first->name == second->name;
    }
    return (equals(first->first, second->first) && equals(first->second, second->second));
}

bool recursiveEqualsWithAxiom(Expression *expression, Expression *axiom, Expression &valA, Expression &valB,
                              Expression &valC) {
    if ((expression == nullptr) ^ (axiom == nullptr)) return false;
    if (expression == nullptr) return true;
    if (axiom->type == Type::VARIABLE) {
        if (axiom->name == "A") {
            if (valA.type == Type::NOTHING) {
                valA.type = expression->type;
                valA.first = expression->first;
                valA.second = expression->second;
                valA.name = expression->name;
                return true;
            } else {
                return equals(expression, &valA);
            }
        }
        if (axiom->name == "B") {
            if (valB.type == Type::NOTHING) {
                valB.type = expression->type;
                valB.first = expression->first;
                valB.second = expression->second;
                valB.name = expression->name;
                return true;
            } else {
                return equals(expression, &valB);
            }
        }
        if (axiom->name == "C") {
            if (valC.type == Type::NOTHING) {
                valC.type = expression->type;
                valC.first = expression->first;
                valC.second = expression->second;
                valC.name = expression->name;
                return true;
            } else {
                return equals(expression, &valC);
            }
        }
    }
    if (axiom->type != expression->type) {
        return false;
    }
    return (recursiveEqualsWithAxiom(expression->first, axiom->first, valA, valB, valC) &&
            recursiveEqualsWithAxiom(expression->second, axiom->second, valA, valB, valC));
}

bool equalsWithAxiom(Expression *expression, Expression *axiom) {
    Expression valA = Expression(), valB = Expression(), valC = Expression();
    return recursiveEqualsWithAxiom(expression, axiom, valA, valB, valC);
}

string Expression::toString() {
    string l = first == nullptr ? "" : first->toString();
    string r = second == nullptr ? "" : second->toString();
    if (type == Type::VARIABLE) {
        return '(' + name + ')';
    }
    if (type == Type::NOT) {
        return "(!" + l + ')';
    }
    string t;
    switch (type) {
        case Type::IMPLICATION:
            t = "->";
            break;
        case Type::AND:
            t = "&";
            break;
        case Type::OR:
            t = "|";
    }
    return '(' + l + t + r + ')';
}

Expression *Expression::getSecond() {
    return this->second;
}

Expression *Expression::getFirst() {
    return this->first;
}

Type Expression::getType() {
    return type;
}

bool Expression::evaluate(bool A, bool B, bool C) {
    if (type == Type::NOTHING) {
        return false;
    } else if (type == Type::VARIABLE) {
        if (name == "A") return A;
        else if (name == "B") return B;
        else return C;
    } else {
        bool valFirst = first == nullptr ? false : first->evaluate(A, B, C);
        bool valSecond = second == nullptr ? false : second->evaluate(A, B, C);
        return getValue(valFirst, valSecond);
    }
}

bool Expression::getValue(bool a, bool b) {
    if (type == Type::NOTHING && type == Type::VARIABLE) return false;
    else if (type == Type::AND) return a && b;
    else if (type == Type::OR) return a || b;
    else if (type == Type::IMPLICATION) return !a || b;
    else return !a;
}

string Expression::getName() {
    return name;
}



