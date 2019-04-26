//
// Created by krotk on 17.03.2019.
//

#ifndef MATLOG2_PROOFCHECKER_H
#define MATLOG2_PROOFCHECKER_H

#include "parser.h"
#include "expression.h"

class ProofChecker {
    vector<string> axioms;
    vector<Expression *> parsedAxioms;
    vector<Expression *> hyps;
public:
    struct Answer {
        Expression *expression;
        int firstArgument;
        int secondArgument;

        Answer(Expression *e, int f, int s) {
            expression = e;
            firstArgument = f;
            secondArgument = s;
        }
    };

    ProofChecker();

private:

    int pos_of_whatToProf = -1;
    vector<Answer> ans;
    vector<bool> used_hyps;
    unordered_map<string, int> proofInd;
    map<int, int> newInd;
    Expression *whatToProof;
    unordered_map<string, vector<int> > mp_r;

    vector<string> checkAndMinProof(vector<Expression *> &proof);

    void minProof();

    void checkInd(int ind, set<int> &usedInd);

    vector<string> toString();

    bool checkExpression(Expression *expression);

    void prepare_mp(Expression *expression, int ind);

public:

    static pair<vector<string>, string> prepareAnnotation (string const &annotation);

    static string deleteWhiteSpaces(string const &annotation);

    int isHyp(Expression *expression);

    int isAxiom(Expression *expression);

    vector<string> check(const string& ann, const vector<string>& proof);
};

#endif //MATLOG2_PROOFCHECKER_H
