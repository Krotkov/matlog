//
// Created by daniil on 20.04.19.
//

#include "Generator.h"
#include "expression.h"
#include "proofChecker.h"
#include "ProofGenerator.h"

using namespace std;

void Generator::deduction(vector<string> &proof, vector<Expression*>& parsed_proof, const string &A) {
    vector<string> ans;
    map<string, vector<int> > mp_r;
    std::unordered_map<string, int> num;
    vector<pair<int, int> > modus;
    ExpressionParser parser;
    for (int i = 0; i < proof.size(); i++) {
        //parsed_proof.emplace_back(parser.parse(proof[i]));
        modus.emplace_back(-1, -1);
        string expr = parsed_proof[i]->toString();
        for (int &j : mp_r[expr]) {
            string f = parsed_proof[j]->getFirst()->toString();
            if (num[f] != 0) {
                modus[modus.size() - 1] = {j, num[f] - 1};
                break;
            }
        }
        num[parsed_proof[i]->toString()] = i + 1;
        if (parsed_proof[i]->getType() == Type::IMPLICATION) {
            mp_r[parsed_proof[i]->getSecond()->toString()].push_back(i);
        }
    }
    Expression *strA = parser.parse(A);
    string stA = strA->toString();
    for (int i = 0; i < proof.size(); i++) {
        if (equals(parsed_proof[i], strA)) {
            append(ans, AimplAlemm(stA));
        } else if (modus[i].first != -1) {
                string help1 = "(" + stA + "->" + proof[modus[i].second] + ")";
                string help2 = "(" + stA + "->(" + proof[modus[i].first] + "))";
                ans.emplace_back('(' + help1 + "->(" + help2 + "->" + "(" + stA + "->" + proof[i] + ")))");
                ans.emplace_back("(" + help2 + "->" + "(" + stA + "->" + proof[i] + "))");
                ans.emplace_back("((" + stA + ")->(" + proof[i] + "))");
            } else {
                ans.emplace_back(proof[i]);
                ans.emplace_back("((" + proof[i] + ")->((" + stA + ")->(" + proof[i] + ")))");
                ans.emplace_back("((" + stA + ")->(" + proof[i] + "))");
            }
    }
    proof = ans;
}

vector<string> Generator::contraposition(const string &A, const string &B) {
    vector<string> ans;
    ExpressionParser parser;
    string strA = parser.parse(A)->toString();
    string strB = parser.parse(B)->toString();
    ans.emplace_back("((" + strA + "->" + strB + ")->(" + strA + "->" + "!(" + strB + "))->!(" + strA + "))");
    ans.emplace_back("(" + strA + "->" + strB + ")");
    ans.emplace_back("((" + strA + "->!(" + strB + "))->(!(" + strA + ")))");
    ans.emplace_back("(!(" + strB + ")->(" + strA + "->!(" + strB + ")))");
    ans.emplace_back("(!(" + strB + "))");
    ans.emplace_back("((" + strA + ")->(!(" + strB + ")))");
    ans.emplace_back("(!(" + strA + "))");
    vector <Expression*> parsed_ans;
    for (auto& st: ans) {
        parsed_ans.emplace_back(parser.parse(st));
    }
    deduction(ans, parsed_ans, "!(" + strB + ")");
    parsed_ans.resize(0);
    for (auto& st: ans) {
        parsed_ans.emplace_back(parser.parse(st));
    }
    deduction(ans, parsed_ans, "(" + strA + "->" + strB + ")");
    return ans;
}

vector<string> Generator::generate(Type type, Expression *first, Expression *second, bool valFirst, bool valSecond) {
    if (type == Type::NOTHING) return vector<string>();
    if (type == Type::VARIABLE) return generateVar(first, valFirst);
    if (type == Type::IMPLICATION) return generateImpl(first, second, valFirst, valSecond);
    if (type == Type::AND) return generateAnd(first, second, valFirst, valSecond);
    if (type == Type::OR) return generateOr(first, second, valFirst, valSecond);
    if (type == Type::NOT) return generateNot(first, valFirst);
}

vector<string> Generator::generateImpl(Expression *first, Expression *second, bool valFirst, bool valSecond) {
    string a = first->toString();
    string b = second->toString();
    if (!valFirst) return replaceTwo(AimplBuseNotA, a, b);
    if (valSecond) return replaceTwo(AimplBuseB, a, b);
    return replaceTwo(notAimplB, a, b);
}

vector<string> Generator::generateNot(Expression *first, bool valFirst) {
    vector<string> ans;
    string expr = first->toString();
    if (valFirst) {
        return replaceOne(notnotA, expr);
    } else {
        ans.emplace_back("!" + expr);
    }
    return ans;
}

vector<string> Generator::generateAnd(Expression *first, Expression *second, bool valFirst, bool valSecond) {
    string a = first->toString();
    string b = second->toString();
    if (valFirst && valSecond) {
        return replaceTwo(AandB, a, b);
    } else {
        if (!valFirst) return replaceTwo(notAandBuseNotA, a, b);
        else return replaceTwo(notAandBuseNotB, a, b);
    }
}

vector<string> Generator::generateOr(Expression *first, Expression *second, bool valFirst, bool valSecond) {
    string a = first->toString();
    string b = second->toString();
    if (valFirst) return replaceTwo(AorBuseA, a, b);
    if (valSecond) return replaceTwo(AorBuseB, a, b);
    return replaceTwo(notAorB, a, b);
}

vector<string> Generator::generateVar(Expression *first, bool valFirst) {
    if (valFirst) return {first->getName()};
    else return {"!" + first->getName()};
}

vector<string> Generator::genAorNotA() {
    vector<string> v1, v2;
    v1 = Generator::contraposition("A", "A|!A");
    v2 = Generator::contraposition("!A", "A|!A");
    v1.emplace_back("(A->A|!A)");
    v1.emplace_back("(!(A|!A)->!A)");
    v2.emplace_back("(!A->A|!A)");
    v2.emplace_back("(!(A|!A)->!!A)");
    vector<string> final = v1;
    append(final, v2);
    final.emplace_back("(" + final[v1.size() - 1] + "->" + final[v1.size() + v2.size() - 1] + "->(!!(A|!A)))");
    final.emplace_back("(" + final[v1.size() + v2.size() - 1] + "->(!!(A|!A)))");
    final.emplace_back("(!!(A|!A))");
    final.emplace_back("(!!(A|!A)->(A|!A))");
    final.emplace_back("(A|!A)");
    ProofChecker checker;
    return checker.check("|-A|!A", final);
}

Generator::Generator() {
    AorNotA = genAorNotA();
    AandB = {"A", "B", "(A->B->A&B)", "(B->A&B)", "(A&B)"};
    AorBuseA = {"A", "(A->A|B)", "(A|B)"};
    AorBuseB = {"B", "(B->A|B)", "(A|B)"};
    AimplBuseB = {"B", "B->A->B", "A->B"};
    AimplBuseNotA = genAimplBuseNotA();
    notAandBuseNotA = {
            "(A&B)->A",
            "!A",
            "!A->((A&B)->!A)",
            "(A&B)->!A",
            "((A&B)->A)->(((A&B)->!A)->!(A&B))",
            "((A&B)->!A)->!(A&B)",
            "!(A&B)"
    };
    notAandBuseNotB = {
            "(A&B)->B",
            "!B",
            "!B->((A&B)->!B)",
            "(A&B)->!B",
            "((A&B)->B)->(((A&B)->!B)->!(A&B))",
            "((A&B)->!B)->!(A&B)",
            "!(A&B)"
    };
    notnotA = {
            "A",
            "A->(!!!A->A)",
            "!!!A->A",
            "!!!A->!A",
            "(!!!A->A)->((!!!A->!A)->!!!!A)",
            "(!!!A->!A)->!!!!A",
            "!!!!A",
            "!!!!A->!!A",
            "!!A"
    };
    notAorB = gennotAorB();

    notAimplB = gennotAimplB();
}

vector<string> Generator::replaceTwo(const vector<string> &proof, const string &A, const string &B) {
    vector<string> ans;
    for (const auto &st: proof) {
        string e;
        for (auto c: st) {
            if (c == 'A') e += A;
            else if (c == 'B') e += B;
            else e += c;
        }
        ans.emplace_back(e);
    }
    return ans;
}

vector<string> Generator::replaceOne(const vector<string> &proof, const string &A) {
    vector<string> ans;
    for (const auto &st: proof) {
        string e;
        for (auto c: st) {
            if (c == 'A') e += A;
            else e += c;
        }
        ans.emplace_back(e);
    }
    return ans;
}

vector<string> Generator::gennotAorB() {
    vector<string> ans;
    ans.emplace_back("!A");
    ans.emplace_back("!B");
    ans.emplace_back("(((A|B)->A)->((A|B)->!A)->(!(A|B)))");
    ans.emplace_back("((A->A)->(B->A)->((A|B)->A))");
    vector<string> imp1 = replaceTwo(AimplBuseNotA, "A", "A");
    append(ans, imp1);
    imp1 = replaceTwo(AimplBuseNotA, "B", "A");
    append(ans, imp1);
    ans.emplace_back("((B->A)->((A|B)->A))");
    ans.emplace_back("((A|B)->A)");
    ans.emplace_back("(((A|B)->!A)->(!(A|B)))");
    ans.emplace_back("((!A)->(A|B)->(!A))");
    ans.emplace_back("(!A)");
    ans.emplace_back("((A|B)->!A)");
    ans.emplace_back("(!(A|B))");
    ProofChecker checker;
    return checker.check("!A,!B|-!(A|B)",ans);
}

vector<string> Generator::AimplAlemm(const string& a) {
    vector<string> ans;
    string help3 = "(" + a + "->" + a + ")";
    string help1 = "(" + a + "->" + help3 + ")";
    string help2 = "(" + a + "->(" + help3 + "->" + a + "))";
    ans.emplace_back("(" + a + "->" + help3 + ")");
    ans.emplace_back("(" + help1 + "->" + help2 + "->" + help3 + ")");
    ans.emplace_back("(" + help2 + "->" + help3 + ")");
    ans.emplace_back(help2);
    ans.emplace_back(help3);
    return ans;
}

vector<string> Generator::gennotAimplB() {
    vector<string> ans;
    ans.emplace_back("(((A->B)->B)->((A->B)->!B)->!(A->B))");
    ans.emplace_back("(((A->B)->A)->(((A->B)->A->B)->((A->B)->B)))");
    ans.emplace_back("(A->(A->B)->(A))");
    ans.emplace_back("(A)");
    ans.emplace_back("((A->B)->(A))");
    ans.emplace_back("(((A->B)->(A->B))->((A->B)->B))");
    append(ans, AimplAlemm("(A->(B))"));
    ans.emplace_back("((A->B)->(B))");
    ans.emplace_back("((A->B)->!B)->!(A->B))");
    ans.emplace_back("(!(B))");
    ans.emplace_back("(!(B)->(A->B)->!(B))");
    ans.emplace_back("((A->B)->!(B))");
    ans.emplace_back("!(A->B)");
    ProofChecker checker;
    return checker.check("A, !B|-!(A->B)", ans);
}

vector<string> Generator::genAimplBuseNotA() {
    vector<string> ans;
    ans.emplace_back("(A)");
    ans.emplace_back("(!(A))");
    ans.emplace_back("((!(B)->A)->(!(B)->!(A))->!!(B))");
    ans.emplace_back("((A)->!(B)->(A))");
    ans.emplace_back("(!(B)->(A))");
    ans.emplace_back("(!(A)->!(B)->!(A))");
    ans.emplace_back("(!(B)->!(A))");
    ans.emplace_back("((!(B)->!(A))->!!(B))");
    ans.emplace_back("!!(B)");
    ans.emplace_back("(!!(B)->(B))");
    ans.emplace_back("B");
    ExpressionParser parser;
    vector<Expression*> parsed_ans;
    for (auto& st: ans) {
        parsed_ans.emplace_back(parser.parse(st));
    }
    deduction(ans, parsed_ans, "A");
    ProofChecker checker;
    return checker.check("!A|-A->B", ans);
}


