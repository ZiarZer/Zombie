#include "error.h"

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

void print_runtime_error(char **program, struct location loc, int error_code)
{
    char *error_messages[] = {
        "Pointer out of bounds (upper bound)\n",
        "Pointer out of bounds (lower bound)\n",
        ("Invalid value (cannot increment value "
         "already at 255)\n"),
        "Invalid value (cannot decrement value already at 0)\n"
    };

    print_location(loc, stderr);

    fputs("\033[31;1mruntime error: \033[0m", stderr);
    fputs(error_messages[error_code - 1], stderr);

    display_program_location(program[loc.i], loc, RED);
}

void display_program_location(char *line, struct location location, char *color)
{
    char *colored_code = color_text(line, location.j, location.j, color);
    char *point_error = point_error_text(location.j, location.j, color);
    fprintf(stderr, " %4ld | %s", location.i + 1, colored_code);
    fprintf(stderr, "      | %s", point_error);
    free(colored_code);
    free(point_error);
}

void missing_bracket_error_message(char *line, struct location location)
{
    int missing_is_left = line[location.j] == ']';

    fprintf(stderr, "\033[1m%s:%ld:%ld: \033[31msyntax error: \033[0m",
            location.filename, location.i + 1, location.j + 1);

    if (missing_is_left)
        fprintf(stderr,
                "expected ‘\033[1m[\033[0m’ before ‘\033[1m]\033[0m’ token\n");
    else
        fprintf(stderr,
                "‘\033[1m[\033[0m’ with no matching ‘\033[1m]\033[0m’ token, "
                "\033[0mexpected ‘\033[1m]\033[0m’ before end of file\n");

    display_program_location(line, location, RED);
}
