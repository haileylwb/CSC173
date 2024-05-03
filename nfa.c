//
// File: nfa.c
// Creator: Hailey Wong-Budiman
// Created: 2/3/2024
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"
#include "IntHashSet.h"

struct NFA{
    int numStates;
    int initialState;
    IntHashSet acceptingStates;
    IntHashSet** transitions;
    bool testing;
};

// Allocate and return a new NFA containing the given number of states.
NFA new_NFA(int nstates) {
    NFA nfa = (NFA)malloc(sizeof(struct NFA));
    nfa->numStates = nstates;
    nfa->initialState = 0;
    nfa->testing=false;
    nfa->transitions = (IntHashSet**)malloc(nstates * sizeof(IntHashSet*));
    for (int i = 0; i < nstates; i++) {
        nfa->transitions[i] = (IntHashSet *)malloc(128 * sizeof(IntHashSet));
        for (int j = 0; j < 128; j++) {
            nfa->transitions[i][j] = new_IntHashSet(20);
        }
    }
    nfa->acceptingStates = new_IntHashSet(nstates);
    return nfa;
}

int getStates (NFA this){
    return this->numStates;
}

bool getTest (NFA this){
    return this->testing;
}

bool setTest (NFA this){
    return this->testing = true;
}

IntHashSet getAcceptingStates (NFA nfa){
    return nfa->acceptingStates;
}

// Free the given NFA
void NFA_free(NFA nfa) {
    IntHashSet_free(nfa->acceptingStates);
    for (int i = 0; i < nfa->numStates; i++){
        for (int j = 0; j < 128; j++) { // Corrected loop limit
            IntHashSet_free(nfa->transitions[i][j]);
        }
        free(nfa->transitions[i]);
    }
    free(nfa->transitions);
    free(nfa);
}

// Return the number of states in the given NFA.
int NFA_get_size(NFA nfa) {
    return nfa->numStates;
}

// Return the set of next states specified by the given NFA's transition function from the given state on input symbol sym.
IntHashSet NFA_get_transitions(NFA nfa, int state, char sym) {
    return nfa->transitions[state][(int)sym];
}

// For the given NFA, add the state dst to the set of next states from state src on input symbol sym.
void NFA_add_transition(NFA nfa, int src, char sym, int dst) {
    IntHashSet_insert(nfa->transitions[src][(int)sym], dst);
}

// Add a transition for the given NFA for each symbol in the given str.
void NFA_add_transition_str(NFA nfa, int src, char *str, int dst) {
    for (int i = 0; i < strlen(str); i++) {
        char input = str[i];
        NFA_add_transition(nfa, src, input, dst);
    }
}

// Add a transition for the given NFA for each input symbol.
void NFA_add_transition_all(NFA nfa, int src, int dst) {
    for (int i = 0; i < 128; i++) {
        IntHashSet_insert(nfa->transitions[src][i], dst);
    }
}

// Add a transition for the given NFA for each input symbol except for a certain one.
void NFA_add_transition_all_but(NFA nfa, int src, char sym, int dst){
    for (int i = 0; i < 128; i++){
        if (i != (int)sym) {
            IntHashSet_insert(nfa->transitions[src][i], dst);
        }
    }
}

// Set whether the given NFA's state is accepting or not.
void NFA_set_accepting(NFA nfa, int state, bool value) {
    if (value) {
        IntHashSet_insert(nfa->acceptingStates, state);
    }
}

// Return true if the given NFA's state is an accepting state.
bool NFA_get_accepting(NFA nfa, int state) {
    return IntHashSet_lookup(nfa->acceptingStates, state);
}

// Run the given NFA on the given input string, and return true if it accepts
// the input, otherwise false.
bool NFA_execute(NFA nfa, char *input){
    IntHashSet currStates = new_IntHashSet(nfa->numStates);
    IntHashSet_insert(currStates, nfa->initialState);
    for (int i = 0; input[i] != '\0'; i++) {
        IntHashSet nextStates = new_IntHashSet(nfa->numStates);
        for (int j = 0; j < nfa->numStates; j++){
            if (IntHashSet_lookup(currStates, j)){
                IntHashSet transitions = NFA_get_transitions(nfa, j, input[i]);
                IntHashSet_union(nextStates, transitions);
            }
        }
        IntHashSet_free(currStates);
        currStates = nextStates;
    }
    bool result = false;
    for (int i = 0; i < nfa->numStates; i++) {
        if (IntHashSet_lookup(currStates, i) && NFA_get_accepting(nfa, i)) {
            result = true;
            break;
        }
    }
    IntHashSet_free(currStates);
    return result;
}

// Runs any NFA in a “Read-Eval-Print Loop” (REPL)
void NFA_repl(NFA *nfa) {
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
        bool result = NFA_execute(*nfa, input);
        printf("\tResult for \"%s\": %s\n", input, result ? "true" : "false");
    }
    printf("\n");
}

// Print the given NFA to System.out.
void NFA_print(NFA nfa) {
    printf("States: ");
    for (int i = 0; i < nfa->numStates; i++) {
        printf("%d ", i);
    }
    printf("\nInput Alphabet: ASCII Characters 1-128\nTransition Table:\n");
    for (int i = 0; i < nfa->numStates; i++) {
        printf("State %d [", i);
        for (int sym = 0; sym < 128; sym++) {
            IntHashSet* transitions = nfa->transitions[i]; // Change here
            printf("%d ", IntHashSet_lookup(*transitions, sym) ? 1 : 0);
        }
        printf("]\n");
    }
    printf("Initial State: 0\nAccepting States:\n");
    IntHashSetIterator iterator = IntHashSet_iterator(nfa->acceptingStates);
    while (IntHashSetIterator_hasNext(iterator)) {
        int state = IntHashSetIterator_next(iterator);
        printf("%d\n", state);
    }
    free(iterator);
}

// Creates and returns NFA that accepts strings ending with ing
NFA* NFA_for_ends_with_ked(){
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    *nfa = new_NFA(4);
    NFA_add_transition_all(*nfa, 0, 0);
    NFA_add_transition(*nfa, 0, 'k', 1);
    NFA_add_transition(*nfa, 1, 'e', 2);
    NFA_add_transition(*nfa, 2, 'd', 3);
    NFA_set_accepting(*nfa, 3, true);
    return nfa;
}

// Creates and returns NFA that accepts strings that contain ath
NFA* NFA_for_contains_ath(){
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    *nfa = new_NFA(4);
    NFA_add_transition(*nfa, 0, 'a', 1);
    NFA_add_transition(*nfa, 1, 't', 2);
    NFA_add_transition(*nfa, 2, 'h', 3);
    NFA_add_transition_all(*nfa, 0, 0);
    NFA_add_transition_all(*nfa, 1, 0);
    NFA_add_transition_all(*nfa, 2, 0);
    NFA_add_transition_all(*nfa, 3, 3);
    NFA_set_accepting(*nfa, 3, true);
    setTest(*nfa);
    return nfa;
}

// Creates and returns NFA that accepts strings that have more than:
//    one   o/f/r
// OR two   c/n
// OR three e
NFA* NFA_for_conference() {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    *nfa = new_NFA(17);
    NFA_add_transition_all(*nfa, 0, 0);

    NFA_add_transition(*nfa, 0, 'o', 1);
    NFA_add_transition_all_but(*nfa, 1, 'o', 1);
    NFA_add_transition(*nfa, 1, 'o', 2);

    NFA_add_transition(*nfa, 0, 'f', 3);
    NFA_add_transition_all_but(*nfa, 3, 'f', 3);
    NFA_add_transition(*nfa, 3, 'f', 4);

    NFA_add_transition(*nfa, 0, 'r', 5);
    NFA_add_transition_all_but(*nfa, 5, 'r', 5);
    NFA_add_transition(*nfa, 5, 'r', 6);

    NFA_add_transition(*nfa, 0, 'c', 7);
    NFA_add_transition_all_but(*nfa, 7, 'c', 7);
    NFA_add_transition(*nfa, 7, 'c', 8);
    NFA_add_transition_all_but(*nfa, 8, 'c', 8);
    NFA_add_transition(*nfa, 8, 'c', 9);

    NFA_add_transition(*nfa, 0, 'n', 10);
    NFA_add_transition_all_but(*nfa, 10, 'n', 10);
    NFA_add_transition(*nfa, 10, 'n', 11);
    NFA_add_transition_all_but(*nfa, 11, 'n', 11);
    NFA_add_transition(*nfa, 11, 'n', 12);

    NFA_add_transition(*nfa, 0, 'e', 13);
    NFA_add_transition_all_but(*nfa, 13, 'e', 13);
    NFA_add_transition(*nfa, 13, 'e', 14);
    NFA_add_transition_all_but(*nfa, 14, 'e', 14);
    NFA_add_transition(*nfa, 14, 'e', 15);
    NFA_add_transition_all_but(*nfa, 15, 'e', 15);
    NFA_add_transition(*nfa, 15, 'e', 16);

    NFA_set_accepting(*nfa, 2, true);
    NFA_set_accepting(*nfa, 4, true);
    NFA_set_accepting(*nfa, 6, true);
    NFA_set_accepting(*nfa, 9, true);
    NFA_set_accepting(*nfa, 12, true);
    NFA_set_accepting(*nfa, 16, true);

    return nfa;
}
