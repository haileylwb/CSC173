//
// File: dfa.c
// Creator: Hailey Wong-Budiman
// Created: 1/30/2024
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dfa.h"
#include "IntHashSet.h"

struct DFA {
    int** transitions;
    int* acceptingStates;
    int numStates;
    int initialState;
};

// Allocate and return a new DFA containing the given number of states.
DFA new_DFA(int nstates){
    DFA dfa = (DFA)malloc(sizeof(struct DFA));
    dfa->numStates = nstates;
    dfa->initialState = 0;
    dfa->acceptingStates = (int*)malloc(nstates * sizeof(int));
    dfa->transitions = (int**)malloc(nstates * sizeof(int*));

    for (int i = 0; i < nstates; i++) {
        dfa->transitions[i] = (int*)malloc(128 * sizeof(int));
        memset(dfa->transitions[i], -1, 128 * sizeof(int)); // Initialize transitions to -1 (reject state)
    }
    memset(dfa->acceptingStates, 0, nstates * sizeof(int)); // Initialize all states as non-accepting
    return dfa;
}

// Free the given DFA.
void DFA_free(DFA dfa){
    for (int i = 0; i < dfa->numStates; i++) {
        free(dfa->transitions[i]);
    }
    free(dfa->transitions);
    free(dfa->acceptingStates);
    free(dfa);
}

// Return the number of states in the given DFA.
int DFA_get_size(DFA dfa){
    return dfa->numStates;
}

int DFA_get_initialState(DFA dfa){
    return dfa->initialState;
}

int DFA_set_initialState(DFA dfa, int i){
    return dfa->initialState = i;
}

// Return the state specified by the given DFA's transition function from state src on input symbol sym.
int DFA_get_transition(DFA dfa, int src, char sym){
    return dfa->transitions[src][(int)sym];
}

// For the given DFA, set the transition from state src on input symbol sym to be the state dst.
void DFA_set_transition(DFA dfa, int src, char sym, int dst){
    dfa->transitions[src][(int)sym] = dst;
}

// Set the transitions of the given DFA for each symbol in the given str.
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst){
    for (int i = 0; str[i] != '\0'; i++) {
        DFA_set_transition(dfa, src, str[i], dst);
    }
}

//Set the transitions of the given DFA for all input symbols.
void DFA_set_transition_all(DFA dfa, int src, int dst){
    for (int i = 0; i < 128; i++) {
        DFA_set_transition(dfa, src, i, dst);
    }
}


// Set whether the given DFA's state is accepting or not.
void DFA_set_accepting(DFA dfa, int state, bool value){
    dfa->acceptingStates[state] = value ? 1 : 0;
}

// Return true if the given DFA's state is an accepting state.
bool DFA_get_accepting(DFA dfa, int state){
    return dfa->acceptingStates[state] == 1;
}

// Run the given DFA on the given input string, and return true if it accepts the input, otherwise false.
bool DFA_execute(DFA dfa, char *input){
    int current_state = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        char symbol = input[i];
        current_state = DFA_get_transition(dfa, current_state, symbol);
        if (current_state == -1) {
            return false;
        }
    }
    return DFA_get_accepting(dfa, current_state);
}

// Runs any DFA in a “Read-Eval-Print Loop” (REPL)
void DFA_repl(DFA *dfa) {
    while (1) {
        char input[51];
        printf("\tEnter Input (\"quit\" to Quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "quit") == 0) {
            break;
        }
        bool result = DFA_execute(*dfa, input);
        printf("\tResult for \"%s\": %s\n", input, result ? "true" : "false");
    }
    printf("\n");
}

// Prints dfa
void DFA_print(DFA dfa){
    printf("States: ");
    for (int i = 0; i < dfa->numStates; i++) {
        printf("%d ", i);
    }
    printf("\nInput Alphabet: ASCII Characters 1-128\nTransition Table:\n");
    for (int i = 0; i < dfa->numStates; i++) {
        printf("State %d [", i);
        for (int j = 0; j < 128; j++) {
            printf("%d ", dfa->transitions[i][j]);
        }
        printf("]\n");
    }
    printf("Initial State: 0\nAccepting States:\n");
    for (int i = 0; i < dfa->numStates; i++) {
        if (dfa->acceptingStates[i] == 1) {
            printf("%d\n", i);
        }
    }
}

// Creates and returns a DFA that accepts the exact string dfa
DFA* DFA_for_contains_dfa(){
    DFA *dfa = malloc(sizeof(DFA));
    *dfa = new_DFA(4);
    DFA_set_transition(*dfa, 0, 'd', 1);
    DFA_set_transition(*dfa, 1, 'f', 2);
    DFA_set_transition(*dfa, 2, 'a', 3);
    DFA_set_accepting(*dfa, 3, true);
    return dfa;
}

// Creates and returns a DFA that accepts strings starting with cat
DFA* DFA_for_contains_cat(){
    DFA *dfa = malloc(sizeof(DFA));
    *dfa = new_DFA(4);
    DFA_set_transition(*dfa, 0, 'c', 1);
    DFA_set_transition(*dfa, 1, 'a', 2);
    DFA_set_transition(*dfa, 2, 't', 3);
    DFA_set_transition_all(*dfa, 3, 3);
    DFA_set_accepting(*dfa, 3, true);
    return dfa;
}

// Creates and returns a DFA that accepts strings of any length that contain exactly two 2's
DFA* DFA_for_contains_two2(){
    DFA *dfa = malloc(sizeof(DFA));
    *dfa = new_DFA(3);
    DFA_set_transition(*dfa, 0, '2', 1);
    DFA_set_transition(*dfa, 1, '2', 2);
    for (int sym = 0; sym < 128; sym++) {
        if (sym != '2') {
            DFA_set_transition(*dfa, 0, (char)sym, 0);
            DFA_set_transition(*dfa, 1, (char)sym, 1);
            DFA_set_transition(*dfa, 2, (char)sym, 2);
        }
    }
    DFA_set_accepting(*dfa, 2, true);
    return dfa;
}

// Creates and returns a DFA that accepts binary input with an even number of 0's and odd number of 1's
DFA* DFA_for_contains_evenOdd(){
    DFA *dfa = malloc(sizeof(DFA));
    *dfa = new_DFA(4);
    DFA_set_transition(*dfa, 0, '0', 2);
    DFA_set_transition(*dfa, 0, '1', 1);
    DFA_set_transition(*dfa, 1, '0', 3);
    DFA_set_transition(*dfa, 1, '1', 0);
    DFA_set_transition(*dfa, 2, '0', 0);
    DFA_set_transition(*dfa, 2, '1', 3);
    DFA_set_transition(*dfa, 3, '0', 1);
    DFA_set_transition(*dfa, 3, '1', 2);
    DFA_set_accepting(*dfa, 1, true);
    return dfa;
}

