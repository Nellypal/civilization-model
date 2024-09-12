/*
** EPITECH PROJECT, 2023
** B-NWP-400-LYN-4-1-myteams-alexandre.vigneau
** File description:
** circular_buffer.c
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "circular_buffer.h"

/*
Function to create buffer

Params: void

Return: Pointer to empty circular buffer
*/
circular_buffer_t *create_buffer(void)
{
    circular_buffer_t *buff = malloc(sizeof(circular_buffer_t));

    memset(buff->buffer, 0, BUFFER_SIZE);
    buff->r_ind = 0;
    buff->w_ind = 0;
    return buff;
}

/*
Function to destroy buffer

Params: pointer to a circular buffer

Return: void
*/
void destroy_buffer(circular_buffer_t *buff)
{
    if (buff) {
        free(buff);
    }
}

/*
Function to push data into a circular buffer

Params: pointer to a circular buffer,
        an array of char,
        the number of char to push

Return: return the number of char pushed
*/
int circular_buffer_push(circular_buffer_t *buff, char *data, int n)
{
    int next;

    for (int i = 0; i < n; i++) {
        next = buff->w_ind + 1;
        if (next >= BUFFER_SIZE)
            next = 0;
        if (next == buff->r_ind)
            return i;
        buff->buffer[buff->w_ind] = data[i];
        buff->w_ind = next;
    }
    return n;
}

/*
Function to get data of the circular buffer to the end pattern "\\n"

Params: pointer to a circular buffer
        an array of char to contain the result

Return: 0 if the pattern where finded data is in the data array
        -1 if the pattern was not found
*/
int circular_buffer_read_to_pattern(circular_buffer_t *buff, char *data)
{
    int next;
    int end = 0;
    int save_ind = buff->r_ind;
    int i = 0;

    while (buff->r_ind != buff->w_ind && !end) {
        if (buff->r_ind == buff->w_ind) {
            buff->r_ind = save_ind;
            return -1;
        }
        next = buff->r_ind + 1;
        if (next >= BUFFER_SIZE)
            next = 0;
        data[i] = buff->buffer[buff->r_ind];
        buff->r_ind = next;
        if (data[i] == '\n')
            end = 1;
        i++;
    }
    return buff->r_ind == buff->w_ind && !end ? -1 : 0;
}

/*
Function to get every data of the circular buffer to the end

Params: pointer to a circular buffer
        an array of char to contain the result

Return: -1 if buffer is empty
        0 otherwise
*/
int circular_buffer_read_everything(circular_buffer_t *buff, char *data)
{
    int next;
    int i = 0;

    if (buff->r_ind == buff->w_ind) {
        return -1;
    }
    while (buff->r_ind != buff->w_ind) {
        next = buff->r_ind + 1;
        if (next >= BUFFER_SIZE)
            next = 0;
        data[i] = buff->buffer[buff->r_ind];
        buff->r_ind = next;
        i++;
    }
    return 0;
}
