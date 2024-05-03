//
// File: translate.h
// Creator: Hailey Wong-Budiman
// Created: 2/11/2024
//

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdbool.h>
#include "dfa.h"
#include "nfa.h"

extern int findIndex(IntHashSet* set, int size, IntHashSet want);

extern DFA* NFA_to_DFA(NFA* nfa);

#endif //TRANSLATE_H
