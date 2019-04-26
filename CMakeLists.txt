cmake_minimum_required(VERSION 3.13)
project(proof_gen)

set(CMAKE_CXX_STANDARD 14)

add_executable(proof_gen main.cpp generator/Generator.cpp parser/parser.cpp proofChecker/proofChecker.cpp expression/expression.cpp proofGenerator/ProofGenerator.cpp)