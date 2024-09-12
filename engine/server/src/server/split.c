/*
** EPITECH PROJECT, 2022
** 2_minishell
** File description:
** split.c
*/

#include <stdio.h>
#include <string.h>
#include "splitter.h"

static int is_delim(char *delims, char carac)
{
    for (int i = 0; delims[i]; i++) {
        if (carac == delims[i])
            return 1;
    }
    return 0;
}

int is_start_word(char *input, int pos, char *delims)
{
    if (is_delim(delims, input[pos]))
        return 0;
    if (pos == 0)
        return 1;
    if (is_delim(delims, input[pos - 1]))
        return 1;
    return 0;
}

int nb_word(char *input, char *delims)
{
    int count = 0;

    for (int i = 0; input[i]; i++) {
        if (is_start_word(input, i, delims))
            count += 1;
    }
    return count;
}

char *find_words(char const *str, int index, char *delims)
{
    int len = 0;
    int i = index;
    char *word;
    int j = 0;

    while (str[i] != '\0' && !is_delim(delims, str[i])) {
        i++;
        len++;
    }
    word = malloc(sizeof(char) * (len + 1));
    word[len] = '\0';
    for (; j < len; j++) {
        word[j] = str[index];
        index++;
    }
    return word;
}

void free_tab(char **tab)
{
    int i = 0;

    for (; tab[i]; i++)
        free(tab[i]);
    free(tab);
}

char **splitter(char *input, char *delims, int *len)
{
    int i = 0;
    int place = 0;
    char **res;

    *len = nb_word(input, delims);
    res = malloc(sizeof(char *) * (*len + 1));
    res[*len] = NULL;
    while (i < (int)strlen(input)) {
        if (is_start_word(input, i, delims)) {
            res[place] = find_words(input, i, delims);
            i += strlen(res[place]);
            place++;
        }
        i++;
    }
    return res;
}
