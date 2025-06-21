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

static char *get_error_message(enum instruction_result error_code) {
    switch (error_code) {
    case POINTER_LOWER_ERROR:
        return "Pointer out of bounds (lower bound)\n";
    case POINTER_UPPER_ERROR:
        return "Pointer out of bounds (upper bound)\n";
    case VALUE_LOWER_ERROR:
        return "Invalid value (cannot decrement value already at 0)\n";
    case VALUE_UPPER_ERROR:
        return "Invalid value (cannot increment value already at 255)\n";
    default:
        return NULL;
    }
}

void print_runtime_error(char **program, struct location loc, enum instruction_result error_code) {
    char *error_message = get_error_message(error_code);

    print_location(loc, stderr);

    fputs("\033[31;1mruntime error: \033[0m", stderr);
    fputs(error_message, stderr);

    display_program_location(program, loc, RED);
}

void display_program_location(char **program, struct location location, char *color) {
    char *colored_code = color_text(program[location.i], location.j, location.j, color);
    char *point_error = point_error_text(location.j, location.j, color);
    fprintf(stderr, " %4ld | %s", location.i + 1, colored_code);
    fprintf(stderr, "      | %s", point_error);
    free(colored_code);
    free(point_error);
}

void missing_bracket_error_message(char **program, struct location location) {
    int missing_is_left = program[location.i][location.j] == ']';

    fprintf(stderr, "\033[1m%s:%ld:%ld: \033[31msyntax error: \033[0m",
            location.filename, location.i + 1, location.j + 1);

    if (missing_is_left)
        fprintf(stderr,
                "expected ‘\033[1m[\033[0m’ before ‘\033[1m]\033[0m’ token\n");
    else
        fprintf(stderr,
                "‘\033[1m[\033[0m’ with no matching ‘\033[1m]\033[0m’ token, "
                "\033[0mexpected ‘\033[1m]\033[0m’ before end of file\n");

    display_program_location(program, location, RED);
}
