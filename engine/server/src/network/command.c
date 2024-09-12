

#include "network.h"

char *get_command(client_t *client)
{
    circular_buffer_t *current = TAILQ_FIRST(&client->buff_r);
    circular_buffer_t *next;
    char *res;
    char tmp[BUFFER_SIZE] = {};
    int i = 0;

    if (current->r_ind == current->w_ind)
        return NULL;
    res = malloc(sizeof(char) * BUFFER_SIZE);
    memset(res, 0, BUFFER_SIZE);
    while (current && circular_buffer_read_to_pattern(current, tmp) != 0) {
        next = TAILQ_NEXT(current, entries);
        i++;
        res = realloc(res, sizeof(char) * BUFFER_SIZE * (i + 1));
        strcat(res, tmp);
        memset(tmp, 0, BUFFER_SIZE);
        current = next;
    }
    strcat(res, tmp);
    return res;
}
