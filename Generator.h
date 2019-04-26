//
// Created by daniil on 20.04.19.
//

#ifndef GLIVENKO_GENERATOR_H
#define GLIVENKO_GENERATOR_H

#include "parser.h"
#include <vector>
#include <string>
#include "string.h"

using namespace std;

class Generator {
public:

    vector<string> genAorNotA();

    vector<string> gennotAorB();

    vector<string> gennotAimplB();

    vector<string> genAimplBuseNotA();

    vector<string> AorNotA, AandB, AorBuseA, AorBuseB, AimplBuseB, notAimplB;
    vector<string> AimplBuseNotA, notAandBuseNotA, notAandBuseNotB, notnotA, notAorB;

    Generator();

    vector<string> AimplAlemm(const string& a);

    void deduction(vector<string> &proof, vector<Expression*>& parsed_proof, const string &A);

    //vector<string> step_generateMP();

    vector<string> contraposition(const string &A, const string &B);

    //vector<string> generateTenAxiom();

    vector<string> generateImpl(Expression* first, Expression* second, bool valFirst, bool valSecond);

    vector<string> generateNot(Expression* first, bool valFirst);

    vector<string> generateAnd(Expression* first, Expression* second, bool valFirst, bool valSecond);

    vector<string> generateOr(Expression* first, Expression* second, bool valFirst, bool valSecond);

    vector<string> generateVar(Expression* first, bool valFirst);

    vector<string> generate(Type type, Expression* first, Expression* second, bool valFirst, bool valSecond);

    vector<string> replaceTwo(const vector<string>& proof, const string& A, const string& B);

    vector<string> replaceOne(const vector<string>& proof, const string& A);
};


#endif //GLIVENKO_GENERATOR_H
