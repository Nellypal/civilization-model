/*
** EPITECH PROJECT, 2023
** B-NWP-400-LYN-4-1-myteams-alexandre.vigneau
** File description:
** splitter.h
*/

#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct splitter_s {
    char **command;
    int nb_args;
} splitter_t;

char **splitter(char *input, char *delims, int *len);
