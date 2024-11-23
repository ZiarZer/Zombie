#include "utils.h"

size_t size_t_max(size_t a, size_t b) {
    return a > b ? a : b;
}

void free_all(char **program, struct bracket_pair *brackets, unsigned char *array, map *breakpoints) {
    if (brackets)
        free(brackets);

    for (size_t i = 0; program[i]; i++)
        free(program[i]);
    free(program);

    if (array)
        free(array);

    map_destroy(breakpoints);
}

int check_array_size(char *array_size_string)
{
    for (size_t i = 0; array_size_string[i]; i++)
    {
        if (array_size_string[i] < '0' || array_size_string[i] > '9')
        {
            fprintf(stderr,
                    "\033[31;1mError: \033[0mInvalid array size (‘%s’)\n",
                    array_size_string);
            return -1;
        }
    }

    return atol(array_size_string);
}

FILE *open_file(char *filename)
{
    struct stat path_stat;
    stat(filename, &path_stat);
    if (S_ISDIR(path_stat.st_mode))
    {
        fputs("\033[31;1mError: \033[0m", stderr);
        fprintf(stderr, "\033[1m%s\033[0m is a directory\n", filename);
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fputs("\033[31;1mError: \033[0m", stderr);
        if (errno == ENOENT)
            fprintf(stderr, "File \033[1m%s\033[0m was not found\n", filename);
        else if (errno == EACCES)
            fprintf(stderr, "\033[1m%s\033[0m: Permission denied\n", filename);
        return NULL;
    }
    return file;
}

char **getlines(char *filename)
{
    FILE *file = open_file(filename);
    if (!file)
        return NULL;

    char **program = calloc(1, sizeof(char *));

    if (!program)
    {
        fclose(file);
        return NULL;
    }

    char *line = NULL;
    size_t n = 0;
    size_t lines_index = 0;
    ssize_t char_read = getline(&line, &n, file);
    while (char_read != -1)
    {
        program = realloc(program, (lines_index + 2) * sizeof(char *));
        program[lines_index] = malloc((strlen(line) + 1) * sizeof(char));
        program[lines_index] = strcpy(program[lines_index], line);
        program[++lines_index] = NULL;

        char_read = getline(&line, &n, file);
    }
    if (line)
        free(line);

    fclose(file);
    return program;
}

void putescchar(unsigned char c)
{
    if (c > 127)
    {
        fprintf(stderr, "0x%x", c);
        return;
    }

    switch (c)
    {
    case '\0':
        fputs("NUL (\\0)", stderr);
        break;
    case '\1':
        fputs("SOH", stderr);
        break;
    case '\2':
        fputs("STX", stderr);
        break;
    case '\3':
        fputs("ETX", stderr);
        break;
    case '\4':
        fputs("EOT", stderr);
        break;
    case '\5':
        fputs("ENQ", stderr);
        break;
    case '\6':
        fputs("ACK", stderr);
        break;
    case '\a':
        fputs("BEL (\\a)", stderr);
        break;
    case '\b':
        fputs("BS (\\b)", stderr);
        break;
    case '\t':
        fputs("HT (\\t)", stderr);
        break;
    case '\n':
        fputs("LF (\\n)", stderr);
        break;
    case '\v':
        fputs("VT (\\v)", stderr);
        break;
    case '\f':
        fputs("FF (\\f)", stderr);
        break;
    case '\r':
        fputs("CR (\\r)", stderr);
        break;
    case '\16':
        fputs("SO", stderr);
        break;
    case '\17':
        fputs("SI", stderr);
        break;
    case '\020':
        fputs("DLE", stderr);
        break;
    case '\021':
        fputs("DC1", stderr);
        break;
    case '\022':
        fputs("DC2", stderr);
        break;
    case '\023':
        fputs("DC3", stderr);
        break;
    case '\024':
        fputs("DC4", stderr);
        break;
    case '\025':
        fputs("NAK", stderr);
        break;
    case '\026':
        fputs("SYN", stderr);
        break;
    case '\027':
        fputs("ETB", stderr);
        break;
    case '\030':
        fputs("CAN", stderr);
        break;
    case '\031':
        fputs("EM", stderr);
        break;
    case '\032':
        fputs("SUB", stderr);
        break;
    case '\033':
        fputs("ESC", stderr);
        break;
    case '\034':
        fputs("FS", stderr);
        break;
    case '\035':
        fputs("GS", stderr);
        break;
    case '\036':
        fputs("RS", stderr);
        break;
    case '\037':
        fputs("US", stderr);
        break;
    case ' ':
        fputs("SPACE", stderr);
        break;
    case 127:
        fputs("DEL", stderr);
        break;
    default:
        fputc(c, stderr);
    }
}
