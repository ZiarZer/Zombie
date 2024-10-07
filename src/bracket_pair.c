#include "bracket_pair.h"

struct char_coords new_char_coords(size_t i, size_t j)
{
    struct char_coords coordinates = { i, j };
    return coordinates;
}

struct bracket_pair new_bracket_pair(size_t i_left, size_t j_left,
                                     size_t i_right, size_t j_right)
{
    struct bracket_pair pair = { i_left, j_left, i_right, j_right };
    return pair;
}

struct char_coords find_matching_bracket(char **program, ssize_t i, ssize_t j,
                                         struct bracket_pair *pairs)
{
    int search_right = program[i][j] == '[';
    struct bracket_pair pair;

    for (size_t idx = 0; pairs[idx].i_left != -1; idx++)
    {
        pair = pairs[idx];
        if (search_right && pair.i_left == i && pair.j_left == j)
            return new_char_coords(pair.i_right, pair.j_right);
        else if (!search_right && pairs[idx].i_right == i
                 && pairs[idx].j_right == j)
            return new_char_coords(pair.i_left, pair.j_left);
    }
    return new_char_coords(-1, -1);
}

void missing_bracket_error_message(char *filename, char *line, size_t i,
                                   size_t j)
{
    int missing_is_left = line[j] == ']';

    fprintf(stderr, "\033[1m%s:%ld:%ld: \033[31msyntax error: \033[0m",
            filename, i + 1, j + 1);

    if (missing_is_left)
        fprintf(stderr,
                "expected ‘\033[1m[\033[0m’ before ‘\033[1m]\033[0m’ token\n");
    else
        fprintf(stderr,
                "‘\033[1m[\033[0m’ with no matching ‘\033[1m]\033[0m’ token, "
                "\033[0mexpected ‘\033[1m]\033[0m’ before end of file\n");

    print_error(line, i, j);
}

void append_bracket_pair(struct char_coords **lefts,
                         struct bracket_pair **pairs, size_t *counts[],
                         struct char_coords new_right_bracket)
{
    size_t i = new_right_bracket.i;
    size_t j = new_right_bracket.j;
    size_t *nb_lefts = counts[0];
    size_t *nb_pairs = counts[1];

    *pairs = realloc(*pairs, ++(*nb_pairs) * sizeof(struct bracket_pair));
    (*pairs)[*nb_pairs - 1] = new_bracket_pair((*lefts)[*nb_lefts - 1].i,
                                               (*lefts)[*nb_lefts - 1].j, i, j);

    *lefts = realloc(*lefts, --(*nb_lefts) * sizeof(struct char_coords));
}

struct bracket_pair *return_after_error(struct location loc, char *line,
                                        struct bracket_pair *pairs,
                                        struct char_coords *lefts)
{
    missing_bracket_error_message(loc.filename, line, loc.i, loc.j);
    if (pairs)
        free(pairs);
    if (lefts)
        free(lefts);
    return NULL;
}

struct bracket_pair *get_bracket_pairs(char **program, char *filename)
{
    struct char_coords *lefts = malloc(0);
    struct bracket_pair *pairs = malloc(0);
    size_t nb_pairs = 0;
    size_t nb_lefts = 0;

    for (size_t i = 0; program[i]; i++)
    {
        for (size_t j = 0; j < strlen(program[i]); j++)
        {
            if (program[i][j] == '[')
            {
                lefts = realloc(lefts, ++nb_lefts * sizeof(struct char_coords));
                lefts[nb_lefts - 1] = new_char_coords(i, j);
            }
            else if (program[i][j] == ']')
            {
                if (nb_lefts == 0)
                {
                    struct location loc = { filename, i, j };
                    return return_after_error(loc, program[i], pairs, lefts);
                }

                struct char_coords new_right_bracket = { i, j };
                size_t *counts[] = { &nb_lefts, &nb_pairs };
                append_bracket_pair(&lefts, &pairs, counts, new_right_bracket);
            }
        }
    }

    if (nb_lefts != 0)
    {
        struct char_coords latest_left = lefts[nb_lefts - 1];
        struct location loc = { filename, latest_left.i, latest_left.j };
        return return_after_error(loc, program[latest_left.i], pairs, lefts);
    }

    if (lefts)
        free(lefts);

    pairs = realloc(pairs, sizeof(struct bracket_pair) * (nb_pairs + 1));
    pairs[nb_pairs] = new_bracket_pair(-1, -1, -1, -1);
    return pairs;
}
