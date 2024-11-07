#ifndef BRACKET_PAIR_H
#define BRACKET_PAIR_H

#include "error.h"

struct bracket_pair
{
    ssize_t i_left;
    ssize_t j_left;
    ssize_t i_right;
    ssize_t j_right;
};

struct location find_matching_bracket(char **program, struct location,
                                      struct bracket_pair *pairs);

struct bracket_pair *get_bracket_pairs(char **program, char *filename);

#endif /* BRACKET_PAIR_H*/
