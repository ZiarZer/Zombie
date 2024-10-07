#define DEFAULT_ARRAY_SIZE 100

#include "execution.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "\033[1mUsage:\033[0m %s ", argv[0]);
        fputs("\033[4mFILE\033[0m [\033[4mARRAY SIZE\033[0m]\n", stderr);
        return 1;
    }

    char **program = getlines(argv[1]);
    if (!program)
        return 1;

    ssize_t array_size =
        argc > 2 ? check_array_size(argv[2]) : DEFAULT_ARRAY_SIZE;
    if (!array_size)
        return 1;

    struct bracket_pair *brackets = get_bracket_pairs(program, argv[1]);
    if (!brackets)
        return free_and_return(program, brackets, NULL, 2);

    return run_program(program, argv[1], brackets, array_size);
}
