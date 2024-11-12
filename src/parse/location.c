#include "location.h"

struct location make_location(char *filename, size_t i, size_t j)
{
    struct location new_location = { filename, i, j };
    return new_location;
}

void print_location(struct location loc, FILE *stream)
{
    fprintf(stream, "\033[1m%s:%ld:%ld: \033[0m", loc.filename, loc.i + 1,
            loc.j + 1);
}
