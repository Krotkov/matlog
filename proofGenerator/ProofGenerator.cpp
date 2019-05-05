//
// Created by daniil on 21.04.19.
//

#include "ProofGenerator.h"
#include "../generator/Generator.h"

void append(vector<string> &a, const vector<string> &b) {
    for (auto &st: b) {
        a.emplace_back(st);
    }
}

vector<string> ProofGenerator::generateProof(const string &annotation) {
    whatToProof = ExpressionParser::parse(annotation);
    fillTruthTable();
    if (truthTable[1][1][1]) {
        vector<int> usefulVars = findUsefulVars(true, false);
        string ann;
        if (usefulVars[0] == 1) ann += "A,";
        if (usefulVars[1] == 1) ann += "B,";
        if (usefulVars[2] == 1) ann += "C,";
        if (!ann.empty()) ann.pop_back();
        ann += "|-" + whatToProof->toString();
        vector<string> ans = generateProofWithVars(usefulVars, true, false);
        ans.push_back(ann);
        return ans;
    } else if (!truthTable[0][0][0]) {
        whatToProof = new Expression(Type::NOT, whatToProof, nullptr);
        vector<int> usefulVars = findUsefulVars(false, true);
        string ann;
        if (usefulVars[0] == 1) ann += "!A,";
        if (usefulVars[1] == 1) ann += "!B,";
        if (usefulVars[2] == 1) ann += "!C,";
        if (!ann.empty()) ann.pop_back();
        ann += "|-" + whatToProof->toString();
        vector<string> ans = generateProofWithVars(usefulVars, false, true);
        ans.push_back(ann);
        return ans;
    } else {
        return {":("};
    }
}

void ProofGenerator::fillTruthTable() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int q = 0; q < 2; q++) {
                truthTable[i][j][q] = whatToProof->evaluate(i, j, q);
            }
        }
    }
}

vector<string> ProofGenerator::generateProof(Expression *expression, bool valA, bool valB, bool valC) {
    if (expression == nullptr) return vector<string>();
    auto type = expression->getType();
    if (type == Type::NOTHING) return vector<string>();
    auto firstSon = expression->getFirst();
    auto secondSon = expression->getSecond();
    bool valFirst = firstSon == nullptr ? false : firstSon->evaluate(valA, valB, valC);
    bool valSecond = secondSon == nullptr ? false : secondSon->evaluate(valA, valB, valC);
    vector<string> proofFirst = generateProof(firstSon, valA, valB, valC);
    vector<string> proofSecond = generateProof(secondSon, valA, valB, valC);
    vector<string> proofThis = generator.generate(type, firstSon, secondSon, valFirst, valSecond);
    vector<string> finalProof;
    append(finalProof, proofFirst);
    append(finalProof, proofSecond);
    append(finalProof, proofThis);
    return finalProof;
}

vector<int> ProofGenerator::findUsefulVars(bool trueVal, bool falseVal) {
    vector<int> ans;
    int num = -1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int q = 0; q < 2; q++) {
                    if (num == -1 || num > i + j + q) {
                        vector<int> check = {i, j, q};
                        bool flag = true;
                        for (int i1 = check[0]; i1 < 2; i1++) {
                            for (int j1 = check[1]; j1 < 2; j1++) {
                                for (int q1 = check[2]; q1 < 2; q1++) {
                                    if (!whatToProof->evaluate(i1 == 1 ? trueVal : falseVal,
                                                               j1 == 1 ? trueVal : falseVal,
                                                               q1 == 1 ? trueVal : falseVal))
                                        flag = false;
                                }
                            }
                        }
                        if (flag) {
                            num = i + j + q;
                            ans = check;
                        }
                    }
            }
        }
    }
    return ans;
}

vector<string> ProofGenerator::generateProofWithVars(const vector<int> &usefulVars, bool trueVal, bool falseVal) {
    string expr = whatToProof->toString();
    if (usefulVars[0] == 0) {
        vector<string> proofWithoutA = deleteHypVar(generateProof(whatToProof, trueVal, trueVal, trueVal),
                                                    generateProof(whatToProof, falseVal, trueVal, trueVal),
                                                    trueVal ? "A" : "!A");
        if (usefulVars[1] == 0) {
            vector<string> proofNotB = deleteHypVar(generateProof(whatToProof, trueVal, falseVal, trueVal),
                                                    generateProof(whatToProof, falseVal, falseVal, trueVal),
                                                    trueVal ? "A" : "!A");
            vector<string> proofWithoutB = deleteHypVar(proofWithoutA, proofNotB, trueVal ? "B" : "!B");
            if (usefulVars[2] == 0) {
                vector<string> proofNotA = deleteHypVar(generateProof(whatToProof, trueVal, trueVal, falseVal),
                                                        generateProof(whatToProof, falseVal, trueVal, falseVal),
                                                        trueVal ? "A" : "!A");
                vector<string> proofNotA2 = deleteHypVar(generateProof(whatToProof, trueVal, falseVal, falseVal),
                                                         generateProof(whatToProof, falseVal, falseVal, falseVal),
                                                         trueVal ? "A" : "!A");
                vector<string> proofNotB2 = deleteHypVar(proofNotA, proofNotA2, trueVal ? "B" : "!B");
                return deleteHypVar(proofWithoutB, proofNotB2, trueVal ? "C" : "!C");
            } else {
                return proofWithoutB;
            }
        } else {
            if (usefulVars[2] == 0) {
                vector<string> proofNotC = deleteHypVar(generateProof(whatToProof, trueVal, trueVal, falseVal),
                                                        generateProof(whatToProof, falseVal, trueVal, falseVal),
                                                        trueVal ? "A" : "!A");
                return deleteHypVar(proofWithoutA, proofNotC, trueVal ? "C" : "!C");
            } else {
                return proofWithoutA;
            }
        }
    } else {
        if (usefulVars[1] == 0) {
            vector<string> proofWithoutB = deleteHypVar(generateProof(whatToProof, trueVal, trueVal, trueVal),
                                                        generateProof(whatToProof, trueVal, falseVal, trueVal),
                                                        trueVal ? "B" : "!B");
            if (usefulVars[2] == 0) {
                vector<string> proofNotC = deleteHypVar(generateProof(whatToProof, trueVal, trueVal, falseVal),
                                                        generateProof(whatToProof, trueVal, falseVal, falseVal),
                                                        trueVal ? "B" : "!B");
                return deleteHypVar(proofWithoutB, proofNotC, trueVal ? "C" : "!C");
            } else {
                return proofWithoutB;
            }
        } else {
            if (usefulVars[2] == 0) {
                return deleteHypVar(generateProof(whatToProof, trueVal, trueVal, trueVal),
                                    generateProof(whatToProof, trueVal, trueVal, falseVal), trueVal ? "C" : "!C");
            } else {
                return generateProof(whatToProof, trueVal, trueVal, trueVal);
            }
        }
    }
}

vector<string>
ProofGenerator::deleteHypVar(const vector<string> &proofVar, const vector<string> &proofNotVar, string var) {
    vector<string> proof;
    string expr = whatToProof->toString();
    vector<Expression *> parsed_proofVar;
    parsed_proofVar.reserve(proofVar.size());
    for (auto &v: proofVar) {
        parsed_proofVar.emplace_back(ExpressionParser::parse(v));
    }
    vector<Expression *> parsed_proofNotVar;
    parsed_proofNotVar.reserve(proofNotVar.size());
    for (auto &v: proofNotVar) {
        parsed_proofNotVar.emplace_back(ExpressionParser::parse(v));
    }
    if (var[0] == '!') {
        Generator::deduction(const_cast<vector<string> &>(proofVar), parsed_proofVar, var);
        Generator::deduction(const_cast<vector<string> &>(proofNotVar), parsed_proofNotVar, var.substr(1));
    } else {
        Generator::deduction(const_cast<vector<string> &>(proofVar), parsed_proofVar, var);
        Generator::deduction(const_cast<vector<string> &>(proofNotVar), parsed_proofNotVar, "!" + var);
    }
    append(proof, proofVar);
    append(proof, proofNotVar);
    vector <string> help;
    if (var[0] != '!') {
        proof.emplace_back(
                "(" + var + "->" + expr + ")->(!" + var + "->" + expr + ")->(" + var + "|!" + var + "->" + expr + ")");
        proof.emplace_back("(!" + var + "->" + expr + ")->(" + var + "|!" + var + "->" + expr + ")");
        proof.emplace_back("(" + var + "|!" + var + "->" + expr + ")");
        help = Generator::replace(generator.AorNotA, var);
    } else {
        string nVar = var.substr(1);
        proof.emplace_back(
                "(" + nVar + "->" + expr + ")->(" + var + "->" + expr + ")->(" + nVar + "|" + var + "->" + expr + ")");
        proof.emplace_back("(" + var + "->" + expr + ")->(" + nVar + "|" + var + "->" + expr + ")");
        proof.emplace_back("(" + nVar + "|" + var + "->" + expr + ")");
        help = Generator::replace(generator.AorNotA, nVar);
    }
    append(proof, help);
    proof.emplace_back(expr);
    return proof;
}
