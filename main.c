//
// File: main.c
// Creator: Hailey Wong-Budiman
// Created: 2/3/2024
//

#include <stdlib.h>
#include <stdio.h>
#include "dfa.h"
#include "nfa.h"
#include "translate.h"

int main () {
    printf("CSC173 Project by Hailey Wong-Budiman\n\n");

    // Part 1: DFA

    printf("Testing DFA that recognizes exactly \"dfa\":\n");
    DFA* dfa1 = DFA_for_contains_dfa();
    DFA_repl(dfa1);
    DFA_free(*dfa1);
    free(dfa1);

    printf("Testing DFA that recognizes starting with \"cat\":\n");
    DFA* dfa2 = DFA_for_contains_cat();
    DFA_repl(dfa2);
    DFA_free(*dfa2);
    free(dfa2);

    printf("Testing DFA for exactly two 2’s:\n");
    DFA* dfa3 = DFA_for_contains_two2();
    DFA_repl(dfa3);
    DFA_free(*dfa3);
    free(dfa3);

    printf("Testing DFA that recognizes binary input with an even number of 0’s and odd number of 1's:\n");
    DFA* dfa4 = DFA_for_contains_evenOdd();
    DFA_repl(dfa4);
    DFA_free(*dfa4);
    free(dfa4);

    // Part 2: NFA

    printf("Testing NFA that recognizes strings ending with \"ked\":\n");
    NFA* nfa1 = NFA_for_ends_with_ked();
    NFA_repl(nfa1);

    printf("Testing NFA that recognizes strings containing \"ath\":\n");
    NFA* nfa2 = NFA_for_contains_ath();
    NFA_repl(nfa2);

    printf("Testing NFA that recognizes strings that have more than one o/f/r OR two c/n OR three e:\n");
    NFA* nfa3 = NFA_for_conference();
    NFA_repl(nfa3);
    NFA_free(*nfa3);
    free(nfa3);

    // Part 3: Converting NFA to DFA

    printf("Testing NFA to DFA conversion for NFA Pt 1\nTesting DFA that recognizes strings ending with \"ked\"\n");
    DFA* dfa5 = NFA_to_DFA(nfa1);
    DFA_repl(dfa5);
    NFA_free(*nfa1);
    free(nfa1);
    DFA_free(*dfa5);
    free(dfa5);

    printf("Testing NFA to DFA conversion for NFA Pt 2\nTesting DFA that recognizes strings containing \"ath\"\n");
    DFA* dfa6 = NFA_to_DFA(nfa2);
    DFA_repl(dfa6);
    NFA_free(*nfa2);
    free(nfa2);
    DFA_free(*dfa6);
    free(dfa6);
}


