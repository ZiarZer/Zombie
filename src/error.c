#include "error.h"

void print_location(struct location loc, FILE *stream)
{
    fprintf(stream, "\033[1m%s:%ld:%ld: \033[0m", loc.filename, loc.i + 1,
            loc.j + 1);
}

char *color_text(char *text, size_t from, size_t to, char *color_code)
{
    char *colored_text =
        calloc(strlen(text) + strlen(color_code) + 5, sizeof(char));

    char *normal_text_color_code = "\033[0m";
    size_t colored_text_index = 0;
    for (size_t i = 0; text[i]; i++)
    {
        if (i == from)
            for (size_t j = 0; color_code[j]; j++)
                colored_text[colored_text_index++] = color_code[j];

        colored_text[colored_text_index++] = text[i];

        if (i == to)
            for (size_t j = 0; normal_text_color_code[j]; j++)
                colored_text[colored_text_index++] = normal_text_color_code[j];
    }
    return colored_text;
}

char *point_error_text(size_t from, size_t to, char *color_code)
{
    char *point_error_text = calloc(to + strlen(color_code) + 7, sizeof(char));

    char *normal_text_color_code = "\033[0m";
    size_t point_error_text_index = 0;
    for (size_t i = 0; i < to + 2; i++)
    {
        if (i == from)
            for (size_t j = 0; color_code[j]; j++)
                point_error_text[point_error_text_index++] = color_code[j];

        if (i < from)
            point_error_text[point_error_text_index++] = ' ';
        else if (i <= to)
            point_error_text[point_error_text_index++] = '^';

        if (i == to)
            for (size_t j = 0; normal_text_color_code[j]; j++)
                point_error_text[point_error_text_index++] =
                    normal_text_color_code[j];
    }
    point_error_text[point_error_text_index] = '\n';
    return point_error_text;
}

void print_runtime_error(char **program, char *filename,
                         struct char_coords coordinates, int error_code)
{
    char *error_messages[] = {
        "Pointer out of bounds (upper bound)\n",
        "Pointer out of bounds (lower bound)\n",
        "Invalid value (cannot increment value "
        "already at 255)\n",
        "Invalid value (cannot decrement value already at 0)\n"
    };

    struct location loc = { filename, coordinates.i, coordinates.j };
    print_location(loc, stderr);

    fputs("\033[31;1mruntime error: \033[0m", stderr);
    fputs(error_messages[error_code - 1], stderr);

    print_error(program[loc.i], loc.i, loc.j);
}

void print_error(char *line, size_t i, size_t j)
{
    char *colored_code = color_text(line, j, j, "\033[31;1m");
    char *point_error = point_error_text(j, j, "\033[31;1m");
    fprintf(stderr, " %4ld | %s", i + 1, colored_code);
    fprintf(stderr, "      | %s", point_error);
    free(colored_code);
    free(point_error);
}
