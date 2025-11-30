cmake_minimum_required(VERSION 4.0)
project(Automat_Finit_Determinist)

set(CMAKE_CXX_STANDARD 20)

add_executable(Automat_Finit_Determinist
        main.cpp
        DeterministicFiniteAutomaton.h
        DeterministicFiniteAutomaton.cpp
        RegexToDFA.cpp
        RegexToDFA.h
        dfa.txt)
