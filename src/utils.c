#include "utils.h"

void free_all(char **program, struct bracket_pair *brackets,
                    unsigned char *array)
{
    if (brackets)
        free(brackets);

    for (size_t i = 0; program[i]; i++)
        free(program[i]);
    free(program);

    if (array)
        free(array);
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
            return 0;
        }
    }

    ssize_t array_size = atol(array_size_string);
    if (array_size == 0)
        fprintf(stderr, "\033[31;1mError: \033[0mArray size cannot be zero\n");
    else if (array_size < 0)
        fprintf(stderr,
                "\033[31;1mError: \033[0mArray size cannot be a negative "
                "number (%ld)\n",
                array_size);
    return array_size;
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
