

#include "network.h"

int init_buffer_queue(struct tailq_circular_buffer *queue)
{
    circular_buffer_t *new_buffer;

    if (TAILQ_EMPTY(queue)) {
        new_buffer = create_buffer();
        TAILQ_INIT(queue);
        TAILQ_INSERT_HEAD(queue, new_buffer, entries);
    }
    return 0;
}

void destroy_buffer_queue(struct tailq_circular_buffer *queue)
{
    circular_buffer_t *current = TAILQ_FIRST(queue);
    circular_buffer_t *next = NULL;

    while (current) {
        next = TAILQ_NEXT(current, entries);
        destroy_buffer(current);
        current = next;
    }
}

void add_to_queue_buffer(struct tailq_circular_buffer *queue, char *message)
{
    circular_buffer_t *current = TAILQ_FIRST(queue);
    circular_buffer_t *next = NULL;
    circular_buffer_t *new_buffer;
    size_t writed_byte = 0;

    while (current) {
        writed_byte += circular_buffer_push(current,
            &message[writed_byte], strlen(message) - writed_byte);
        if (writed_byte < strlen(message)) {
            new_buffer = create_buffer();
            TAILQ_INSERT_TAIL(queue, new_buffer, entries);
        }
        if (writed_byte == strlen(message)) {
            return;
        }
        next = TAILQ_NEXT(current, entries);
        current = next;
    }
}
