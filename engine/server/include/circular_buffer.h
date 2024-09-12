/*
** EPITECH PROJECT, 2023
** B-NWP-400-LYN-4-1-myteams-alexandre.vigneau
** File description:
** circular_buffer.h
*/

#pragma once

#include <sys/queue.h>

#define BUFFER_SIZE 4096

typedef struct circular_buffer_s {
    char buffer[BUFFER_SIZE];
    int w_ind;
    int r_ind;
    TAILQ_ENTRY(circular_buffer_s) entries;
} circular_buffer_t;

TAILQ_HEAD(tailq_circular_buffer, circular_buffer_s);

circular_buffer_t *create_buffer(void);
void destroy_buffer(circular_buffer_t *buff);
int circular_buffer_push(circular_buffer_t *buff, char *data, int n);
int circular_buffer_read_to_pattern(circular_buffer_t *buff, char *data);
int circular_buffer_read_everything(circular_buffer_t *buff, char *data);
