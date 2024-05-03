cmake_minimum_required(VERSION 3.22)
project(program)

set(CMAKE_C_STANDARD 99)

add_executable(program main.c
        dfa.c
        dfa.h
        nfa.c
        nfa.h
        translate.c
        translate.h
        IntHashSet.c
        IntHashSet.h
        Set.h
)
