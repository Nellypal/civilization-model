

#include "network.h"

void add_message_to_buffer(client_t *client, char *message,
    enum buffer_type type)
{
    if (type == Write) {
        add_to_queue_buffer(&client->buff_w, message);
    }
    if (type == Read) {
        add_to_queue_buffer(&client->buff_r, message);
    }
}

void send_to_everyone(server_t *server, char *message)
{
    client_t *tmp;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        add_message_to_buffer(tmp, message, Write);
    }
}

void send_to_client(client_t *client, char *message)
{
    add_message_to_buffer(client, message, Write);
}

int read_cmd(client_t *client, server_t *server)
{
    int read_bytes;
    char buff[BUFFER_SIZE] = {};

    do {
        memset(buff, 0, BUFFER_SIZE);
        read_bytes = read(client->socket, buff, BUFFER_SIZE - 1);
        if (read_bytes <= 0) {
            disconnect_client(client, server);
            return 1;
        }
        if (read_bytes > 0)
            add_message_to_buffer(client, buff, Read);
    } while (read_bytes >= BUFFER_SIZE - 1);
    return 0;
}

int write_buff(client_t *client, server_t *server)
{
    circular_buffer_t *current = TAILQ_FIRST(&client->buff_w);
    int bytes_writen;
    char tmp[BUFFER_SIZE] = {};
    circular_buffer_t *next;

    while (!TAILQ_EMPTY(&client->buff_w) && current) {
        next = TAILQ_NEXT(current, entries);
        if (circular_buffer_read_everything(current, tmp) == -1)
            break;
        TAILQ_REMOVE(&client->buff_w, current, entries);
        bytes_writen = write(client->socket, tmp, strlen(tmp));
        if (bytes_writen == -1) {
            disconnect_client(client, server);
            return 1;
        }
        destroy_buffer(current);
        memset(tmp, 0, BUFFER_SIZE);
        current = next;
    }
    return init_buffer_queue(&client->buff_w);
}
