

#include "network.h"

void init_socket(server_t *server)
{
    int opt = 1;

    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket < 0) {
        perror("Socket creation failed");
        exit(84);
    }
    if (setsockopt(server->server_socket, SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        exit(84);
    }
    if (bind(server->server_socket, (struct sockaddr *)&server->server_address,
    sizeof(server->server_address)) < 0) {
        perror("bind failed");
        exit(84);
    }
    if (listen(server->server_socket, 128) < 0) {
        perror("listen failed");
        exit(84);
    }
}

server_t *create_server(int port, void *data, void (*destructor)(void *))
{
    server_t *server = malloc(sizeof(server_t));

    server->server_address.sin_port = htons(port);
    server->server_address.sin_family = AF_INET;
    server->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    init_socket(server);
    TAILQ_INIT(&server->clients);
    server->max_sd = server->server_socket;
    server->data = data;
    server->disconnect = destructor;
    return server;
}

void destruct_server(server_t *server)
{
    client_t *current;
    client_t *next = NULL;

    if (!server)
        return;
    current = TAILQ_FIRST(&server->clients);
    while (current) {
        next = TAILQ_NEXT(current, entries);
        destroy_buffer_queue(&current->buff_r);
        destroy_buffer_queue(&current->buff_w);
        close(current->socket);
        if (current->disconnect && current->data)
            current->disconnect(current->data);
        free(current);
        current = next;
    }
    close(server->server_socket);
    if (server->disconnect)
        server->disconnect(server->data);
    free(server);
}

void prepare_server(server_t *server)
{
    client_t *current;

    FD_ZERO(&server->read_sockets);
    FD_ZERO(&server->write_sockets);
    FD_SET(server->server_socket, &server->read_sockets);
    TAILQ_FOREACH(current, &server->clients, entries) {
        FD_SET(current->socket, &server->read_sockets);
        FD_SET(current->socket, &server->write_sockets);
        if (current->socket > server->max_sd)
            server->max_sd = current->socket;
    }
}

void select_client(server_t *server, struct timeval *timeout)
{
    if (select(server->max_sd + 1, &server->read_sockets,
    &server->write_sockets, NULL, timeout) < 0) {
        perror("select failed");
        destruct_server(server);
        exit(0);
    }
}
