//
// Created by daniil on 21.04.19.
//

#ifndef PROOF_GEN_PROOFGENERATOR_H
#define PROOF_GEN_PROOFGENERATOR_H

#include "parser.h"
#include "expression.h"
#include "proofChecker.h"
#include "Generator.h"

class ProofGenerator {
    bool truthTable[2][2][2];
    Expression* whatToProof;
    Generator generator;

    vector<int> findUsefulVars(bool trueVal, bool falseVal);

    vector<string> generateProof(Expression* expression, bool valA, bool valB, bool valC);

    vector<string> generateProofWithVars(const vector<int>& usefulVars, bool trueVal, bool falseVal);

    vector<string> deleteHypVar(const vector<string>& proofVar, const vector<string>& proofNotVar, string var);

    void fillTruthTable();
public:
    vector<string> generateProof(const string &annotation);
};

void append(vector<string> &a, const vector<string> &b);

#endif //PROOF_GEN_PROOFGENERATOR_H








