

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <asm-generic/socket.h>

#include "circular_buffer.h"

enum buffer_type {
    Read,
    Write
};

typedef struct client_s {
    void *data;
    void (*disconnect)(void *);
    int socket;
    struct sockaddr_in address;
    struct tailq_circular_buffer buff_r;
    struct tailq_circular_buffer buff_w;
    TAILQ_ENTRY(client_s) entries;
} client_t;


TAILQ_HEAD(tailq_client, client_s);

typedef struct server_s {
    void *data;
    void (*disconnect)(void *);
    int server_socket;
    struct sockaddr_in server_address;
    struct tailq_client clients;
    fd_set read_sockets;
    fd_set write_sockets;
    int max_sd;
} server_t;

void init_socket(server_t *server);
server_t *create_server(int port, void *data, void (*destructor)(void *));
void destruct_server(server_t *server);
void prepare_server(server_t *server);
void select_client(server_t *server, struct timeval *timeout);
client_t *new_client(int socket, struct sockaddr_in *address, char *message);
void accept_new_client(server_t *server, char *message,
    void *(*contructor)(void), void (*destructor)(void *));
void disconnect_client(client_t *client, server_t *server);
int write_client_buff(client_t *client, server_t *server);
void handle_clients(server_t *server,
    void (*process_fun)(client_t *, server_t *));
void add_message_to_buffer(client_t *client, char *message,
    enum buffer_type type);
void send_to_everyone(server_t *server, char *message);
void send_to_client(client_t *client, char *message);
int read_cmd(client_t *client, server_t *server);
int write_buff(client_t *client, server_t *server);
char *get_command(client_t *client);
void destroy_buffer_queue(struct tailq_circular_buffer *queue);
void add_to_queue_buffer(struct tailq_circular_buffer *queue, char *message);
int init_buffer_queue(struct tailq_circular_buffer *queue);
