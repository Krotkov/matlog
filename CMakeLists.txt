cmake_minimum_required(VERSION 3.13)
project(proof_gen)

set(CMAKE_CXX_STANDARD 14)

add_executable(proof_gen main.cpp Generator.cpp parser.cpp proofChecker.cpp expression.cpp ProofGenerator.cpp)