

#include "network.h"

client_t *new_client(int socket, struct sockaddr_in *address, char *message)
{
    client_t *client = malloc(sizeof(client_t));
    circular_buffer_t *new_buffer;

    client->address = *address;
    client->socket = socket;
    client->disconnect = NULL;
    client->data = NULL;
    TAILQ_INIT(&client->buff_r);
    TAILQ_INIT(&client->buff_w);
    new_buffer = create_buffer();
    TAILQ_INSERT_HEAD(&client->buff_r, new_buffer, entries);
    new_buffer = create_buffer();
    TAILQ_INSERT_HEAD(&client->buff_w, new_buffer, entries);
    circular_buffer_push(TAILQ_FIRST(&client->buff_w), message,
        strlen(message));
    return client;
}

void accept_new_client(server_t *server, char *message,
    void *(*constructor)(void), void (*destructor)(void *))
{
    int new_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    client_t *n_client;

    if (FD_ISSET(server->server_socket, &server->read_sockets)) {
        new_socket = accept(server->server_socket, (struct sockaddr *)&address,
        &addr_len);
        if (new_socket < 0) {
            perror("accept failed");
            return;
        }
        n_client = new_client(new_socket, &address, message);
        if (constructor)
            n_client->data = constructor();
        if (destructor)
            n_client->disconnect = destructor;
        TAILQ_INSERT_TAIL(&server->clients, n_client, entries);
    }
}

void disconnect_client(client_t *client, server_t *server)
{
    destroy_buffer_queue(&client->buff_r);
    destroy_buffer_queue(&client->buff_w);
    close(client->socket);
    if (client->disconnect && client->data) {
        client->disconnect(client->data);
    }
    TAILQ_REMOVE(&server->clients, client, entries);
    free(client);
    return;
}

int write_client_buff(client_t *client, server_t *server)
{
    int bytes_writen;
    char tmp[BUFFER_SIZE] = {};
    circular_buffer_t *buff;

    TAILQ_FOREACH(buff, &client->buff_w, entries) {
        circular_buffer_read_everything(buff, tmp);
        bytes_writen = write(client->socket, tmp, strlen(tmp));
        if (bytes_writen == -1) {
            disconnect_client(client, server);
            return 1;
        }
    }
    return 0;
}

void handle_clients(server_t *server,
    void (*process_fun)(client_t *, server_t *))
{
    client_t *current = TAILQ_FIRST(&server->clients);
    client_t *next = NULL;
    int disconnected = 0;

    while (current) {
        next = TAILQ_NEXT(current, entries);
        if (FD_ISSET(current->socket, &server->read_sockets)) {
            disconnected = read_cmd(current, server);
        }
        if (!disconnected && FD_ISSET(current->socket, &server->write_sockets))
            disconnected = write_buff(current, server);
        if (!disconnected)
            process_fun(current, server);
        current = next;
    }
}
