//
// Created by krotk on 17.03.2019.
//

#include "proofChecker.h"
#include "../expression/expression.h"

ProofChecker::ProofChecker() {
    axioms.resize(10);
    axioms = {
            "A->B->A", //1
            "(A->B)->(A->B->C)->(A->C)", //2
            "A->B->A&B", //3
            "(A&B)->A", //4
            "(A&B)->B", //5
            "A->(A|B)", //6
            "B->(A|B)", //7
            "(A->C)->(B->C)->((A|B)->C)", //8
            "(A->B)->(A->(!B))->(!A)", //9
            "!!A->A"
    };
    whatToProof = nullptr;
    ExpressionParser parser;
    for (int i = 0; i < 10; i++) {
        parsedAxioms.push_back(parser.parse(axioms[i]));
    }
}

vector<string> ProofChecker::check(const string& annotation, const vector<string>& proof) {
    pair<vector<string>, string> parsedAnnotation = prepareAnnotation(annotation);
    used_hyps.resize(parsedAnnotation.first.size(), false);
    ExpressionParser parser;
    vector<Expression *> parsedHyp;
    for (const auto &i : parsedAnnotation.first) {
        parsedHyp.push_back(parser.parse(i));
    }
    vector<Expression *> parsedProof;
    string s;
    for (const auto & i : proof) {
        Expression *exp = parser.parse(i);

        parsedProof.push_back(exp);
    }
    hyps = parsedHyp;
    Expression *result = parser.parse(parsedAnnotation.second);
    whatToProof = result;
    return checkAndMinProof(parsedProof);
}

pair<vector<string>, string> ProofChecker::prepareAnnotation(string const &annotation) {
    vector<string> hyp;
    string str = deleteWhiteSpaces(annotation);
    int uk = 0;
    string curHyp;
    while (str[uk] != '|' || str[uk + 1] != '-') {
        while (str[uk] != ',' && (str[uk] != '|' || str[uk + 1] != '-')) {
            curHyp += str[uk];
            uk++;
        }
        if (!curHyp.empty()) {
            hyp.push_back(curHyp);
            curHyp = "";
        }
        if (str[uk] == '|' && str[uk + 1] == '-') {
            break;
        }
        uk++;
    }
    uk += 2;
    string proof;
    for (; uk < str.size(); uk++) {
        proof += str[uk];
    }
    return make_pair(hyp, proof);
}

string ProofChecker::deleteWhiteSpaces(string const &annotation) {
    string answer;
    for (char i : annotation) {
        if (i != ' ' && i != '\n') answer += i;
    }
    return answer;
}

vector<string> ProofChecker::checkAndMinProof(vector<Expression *> &proof) {
    string error = "Proof is incorrect";
    int k = 0;
    for (auto expression: proof) {
        if (!checkExpression(expression)) {
            return {error, to_string(k)};
        }
        k++;
        if (pos_of_whatToProf == -1 && expression->toString() == whatToProof->toString()) {
            pos_of_whatToProf = ans.size();
        }
    }
    if (!equals(proof.back(), whatToProof)) {
        return {error};
    }
    minProof();
    return toString();
}

bool ProofChecker::checkExpression(Expression *expression) {
    int num = isHyp(expression);
    if (num == -1) {
        num = isAxiom(expression);
        if (num == -1) {
            string expr = expression->toString();
            for (auto a: mp_r[expr]) {
                if (proofInd[ans[a - 1].expression->getFirst()->toString()] != 0) {
                    if (proofInd[expr] == 0) {
                        ans.emplace_back(expression, a - 1,
                                         proofInd[ans[a - 1].expression->getFirst()->toString()] - 1);
                        proofInd[expr] = ans.size();
                        prepare_mp(expression, ans.size());
                    }
                    return true;
                }
            }
            return false;
        } else {
            Expression *expr = expression;
            string expr2 = expr->toString();
            if (proofInd[expr2] == 0) {
                ans.emplace_back(expr, -1, num);
                proofInd[(expr2)] = ans.size();
                prepare_mp(expression, ans.size());
            }
        }
    } else {
        Expression *expr = expression;
        string expr2 = expr->toString();
        used_hyps[num] = true;
        if (proofInd[(expr2)] == 0) {
            ans.emplace_back(expr, num, -1);
            proofInd[(expr2)] = ans.size();
            prepare_mp(expression, ans.size());
        }
    }
    return true;
}

int ProofChecker::isHyp(Expression *expression) {
    for (int i = 0; i < hyps.size(); i++) {
        if (equals(hyps[i], expression)) {
            return i;
        }
    }
    return -1;
}

int ProofChecker::isAxiom(Expression *expression) {
    for (int i = 0; i < parsedAxioms.size(); i++) {
        if (equalsWithAxiom(expression, parsedAxioms[i])) {
            return i;
        }
    }
    return -1;
}

void ProofChecker::minProof() {
    vector<Answer> newAns;
    set<int> usedInd;
    checkInd(pos_of_whatToProf - 1, usedInd);
    for (auto it: usedInd) {
        newInd[it] = newAns.size();
        newAns.push_back(ans[it]);
    }
    ans = newAns;
}


void ProofChecker::checkInd(int ind, set<int> &usedInd) {
    if (ans[ind].firstArgument != -1 && ans[ind].secondArgument != -1) {
        usedInd.insert(ind);
        if (usedInd.find(ans[ind].firstArgument) == usedInd.end()) checkInd(ans[ind].firstArgument, usedInd);
        if (usedInd.find(ans[ind].secondArgument) == usedInd.end()) checkInd(ans[ind].secondArgument, usedInd);
    } else {
        usedInd.insert(ind);
    }
}

vector<string> ProofChecker::toString() {
    vector <string> answer;
    for (auto elem : ans) {
        answer.emplace_back(elem.expression->toString());
    }
    return answer;
}

void ProofChecker::prepare_mp(Expression *expression, int ind) {
    Expression *second = expression->getSecond();
    Type type = expression->getType();
    hash<string> hash_st;
    if (type == Type::IMPLICATION) {
        mp_r[second->toString()].push_back(ind);
    }
}
