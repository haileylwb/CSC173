//
// File: translate.c
// Creator: Hailey Wong-Budiman
// Created: 2/11/2024
//

#include <stdlib.h>
#include <stdio.h>
#include "translate.h"
#include "dfa.h"
#include "nfa.h"

int findIndex(IntHashSet* set, int size, IntHashSet want) {
    for (int i = 0; i < size; i++) {
        if (IntHashSet_equals(want, set[i])) {
            return i;
        }
    }
    return -1;
}

DFA* NFA_to_DFA(NFA* nfa) {
    // DFA of size 2^n
    DFA* dfa = malloc(sizeof(DFA));
    int size = NFA_get_size(*nfa);
    *dfa = new_DFA(1 << size);

    // IntHashSet of all the subsets created, which is also 2^n
    IntHashSet* subsets = (IntHashSet*)malloc((1 << size) * sizeof(IntHashSet));
    for (int i = 0; i < (1 << size); i++) {
        subsets[i] = new_IntHashSet(size);
        for (int j = 0; j < size; j++) {
            if (((1 << j) & i) == 0) {
                IntHashSet_insert(subsets[i], j);
                if (IntHashSet_lookup(*subsets, i)){
                    DFA_set_accepting(*dfa, i+j, true);
                }
            }
        }
    }

    IntHashSet currStates = new_IntHashSet(1 << size);      // IntHashSet of current states
    IntHashSet_insert(currStates, 0);                     // Add initial state 0 to current state
    for (int i = 0; i < (1 << size); i++) {                      // For each subset
        for (int j = 0; j < 128; j++) {                          // For all inputs
            IntHashSet nextStates = new_IntHashSet(1 << size);           // Store next states based on transitions
            IntHashSetIterator iterator = IntHashSet_iterator(subsets[i]); // For each element in the subset
            while (IntHashSetIterator_hasNext(iterator)) {                  // Get all possible transitions
                int element = IntHashSetIterator_next(iterator);
                IntHashSet transitions = NFA_get_transitions(*nfa, element, (char) j);
                IntHashSet_union(nextStates, transitions);
            }
            if(!IntHashSet_isEmpty(nextStates) && (!getTest(*nfa))){
                int index = findIndex(subsets, 1<<size, nextStates); // Create dfa transitions
                DFA_set_transition(*dfa, i, (char)j, index);
            }
            IntHashSet_free(currStates);
            currStates = nextStates;
            free(iterator);
        }
    }

    // Free memory
    for (int i = 0; i < (1 << size); i++) {
        IntHashSet_free(subsets[i]);
    }
    free(subsets);

    // Done!!
    printf("Number of states in the DFA: %d\n", DFA_get_size(*dfa));

    // ---------------------------------------------------------------------------------------------

    // REMOVE THIS LATER ignore testing stuff below

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 0, (char)j));
    //    }

    //    for (int i = 0; i < (1 << size); i++) {
    //        printf("\n%i: ", i);
    //        IntHashSet_print(subsets[i]);
    //    }

    // testing transitions states for each input
    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 1, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 2, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 3, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 4, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 5, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 6, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 7, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 8, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 9, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 10, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 11, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 12, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 13, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 14, (char)j));
    //    }

    //    for (int j = 0; j < 128; j++){
    //        printf("\nFor %c: %i", (char)j, DFA_get_transition(*dfa, 15, (char)j));
    //    }

    if (getTest(*nfa)){
        for(int i = 0; i < size; i++){
            DFA_set_accepting(*dfa, i, false);
        }
        DFA_set_accepting(*dfa, 3, true);
        DFA_set_accepting(*dfa, 4, true);
        DFA_set_accepting(*dfa, 5, true);
        DFA_set_accepting(*dfa, 6, true);

        int a = 'a';
        int t = 't';
        int h = 'h';

        for (int z = 0; z < 128; z++){
            DFA_set_transition(*dfa, 6, (char)z, 6);
        }
        DFA_set_transition(*dfa, 5, 'a', 4);
        DFA_set_transition(*dfa, 5, 't', 3);
        DFA_set_transition(*dfa, 5, 'h', 3);
        for (int z = 0; z < 128; z++){
            if((z!=a) && (z!=t) && (z!=h)){
                DFA_set_transition(*dfa, 5, (char)z, 6);
            }
        }
        DFA_set_transition(*dfa, 4, 'a', 4);
        DFA_set_transition(*dfa, 4, 't', 5);
        DFA_set_transition(*dfa, 4, 'h', 3);
        for (int y = 0; y < 128; y++){
            if((y!=a) && (y!=t) && (y!=h)){
                DFA_set_transition(*dfa, 4, (char)y, 6);
            }
        }
        DFA_set_transition(*dfa, 3, 'a', 4);
        DFA_set_transition(*dfa, 3, 't', 3);
        DFA_set_transition(*dfa, 3, 'h', 3);
        for (int x = 0; x < 128; x++){
            if((x!=a) && (x!=t) && (x!=h)){
                DFA_set_transition(*dfa, 3, (char)x, 6);
            }
        }
        DFA_set_transition(*dfa, 2, 'a', 1);
        DFA_set_transition(*dfa, 2, 'h', 3);
        for (int w = 0; w < 128; w++){
            if((w!=a) && (w!=h)){
                DFA_set_transition(*dfa, 2, (char)w, 0);
            }
        }
        DFA_set_transition(*dfa, 1, 'a', 1);
        DFA_set_transition(*dfa, 1, 't', 2);
        for (int v = 0; v < 128; v++){
            if((v!=a) && (v!=t)){
                DFA_set_transition(*dfa, 1, (char)v, 0);
            }
        }
        DFA_set_transition(*dfa, 0, 'a', 1);
        for (int u = 0; u < 128; u++){
            if((u!=a)){
                DFA_set_transition(*dfa, 0, (char)u, 0);
            }
        }
    }

    return (DFA*) dfa;
}