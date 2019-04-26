#include <iostream>
#include "proofGenerator/ProofGenerator.h"
#include "generator/Generator.h"

int main() {
    string s;
    getline(cin, s);
    string A, B, C;
    string ans;
    string var;
    for (char i : s) {
        if ((i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9') || i == '\'') {
            var += i;
        } else {
            if (!var.empty()) {
                if (var == A) ans += "A";
                else if (var == B) ans += "B";
                else if (var == C) ans += "C";
                else if (A.empty()) {
                    A = var;
                    ans += "A";
                }
                else if (B.empty()) {
                    B = var;
                    ans += "B";
                }
                else {
                    C = var;
                    ans += "C";
                }
                var = "";
            }
            ans += i;
        }
    }
    if (!var.empty()) {
        if (var == A) ans += "A";
        else if (var == B) ans += "B";
        else if (var == C) ans += "C";
        else if (A.empty()) {
            A = var;
            ans += "A";
        }
        else if (B.empty()) {
            B = var;
            ans += "B";
        }
        else {
            C = var;
            ans += "C";
        }
        var = "";
    }
    if (A.empty()) A = "A";
    if (B.empty()) B = "B";
    if (C.empty()) C = "C";
    ProofGenerator generator;
    vector<string> proof = generator.generateProof(ans);
    for (auto& st: proof) {
        string st1;
        for (auto c: st) {
            if (c == 'A') st1 += A;
            else if (c == 'B') st1 += B;
            else if (c == 'C') st1 += C;
            else st1 += c;
        }
        st = st1;
    }
    string ann = proof.back();
    cout << proof.back() << "\n";
    proof.pop_back();
    for (auto& st: proof) {
        cout << st << "\n";
    }
    return 0;
}