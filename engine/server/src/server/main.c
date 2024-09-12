

#include <signal.h>
#include "network.h"
#include "engine.h"

bool team_exists(char *team, engine_server_t *server)
{
    for (int i = 0; server->team_names[i]; i++) {
        if (!strcmp(team, server->team_names[i]))
            return true;
    }
    return false;
}

static void register_client(client_t *client, engine_server_t *server,
    server_t *server_main)
{
    char *cmd = get_command(client);
    char **args;
    int nb;

    if (cmd != NULL && strlen(cmd) > 1) {
        if (!strcmp(cmd, "GRAPHIC\n")) {
            register_gui(client, server_main);
            free(cmd);
            return;
        }
        args = splitter(cmd, "\t \n", &nb);
        if (team_exists(args[0], server)) {
            register_ai(server, client, args[0], server_main);
        } else {
            send_to_client(client, "ko\n");
        }
        free_tab(args);
        free(cmd);
    }
}

static void manage_clients(client_t *client, server_t *server)
{
    engine_client_t *engine_client = NULL;
    engine_server_t *engine_server = (engine_server_t *)server->data;

    if (client->data) {
        engine_client = (engine_client_t *)client->data;
        switch (engine_client->type) {
        case GUI:
            manage_gui(client, server);
            break;
        case AI:
            manage_ai(client, server);
            break;
        }
    } else {
        register_client(client, engine_server, server);
    }
    update_eggs(engine_server);
}

static void prepare_engine(engine_server_t *serv)
{
    int cells = serv->size_map.y * serv->size_map.x;

    srand(time(NULL));
    serv->global_inventory = malloc(sizeof(inventory_t));
    memset(serv->global_inventory, 0, sizeof(inventory_t));
    serv->map = malloc(serv->size_map.y * sizeof(inventory_t *));
    for (int i = 0; i < serv->size_map.y; i++) {
        serv->map[i] = malloc(serv->size_map.x * sizeof(inventory_t));
        for (int j = 0; j < serv->size_map.x; j++)
            memset(&serv->map[i][j], 0, sizeof(inventory_t));
    }
    serv->max_ressources = (inventory_t){cells * 0.5, cells * 0.3,
    cells * 0.15, cells * 0.1, cells * 0.1, cells * 0.08, cells * 0.05};
    regenerate_ressources(serv);
    for (int i = 0; serv->teams[i]; i++) {
        generate_team_eggs(serv->teams[i], serv, serv->clients_per_team, true);
    }
    TAILQ_INIT(&serv->incantations);
}

void destroy_engine_client(void *client)
{
    engine_client_t *engine_client = (engine_client_t *)client;

    if (!engine_client)
        return;
    switch (engine_client->type) {
        case GUI:
            break;
        case AI:
            destroy_ai_client(engine_client);
            break;
    }
}

void d_engine_server(void *server)
{
    engine_server_t *engine_server = (engine_server_t *)server;

    if (!engine_server)
        return;
    free(engine_server->global_inventory);
    for (int i = 0; i < engine_server->size_map.y; i++)
        free(engine_server->map[i]);
    free(engine_server->map);
    free(engine_server->team_names);
    for (int i = 0; engine_server->teams[i]; i++) {
        destroy_team_eggs(engine_server->teams[i]);
        free(engine_server->teams[i]);
    }
    free(engine_server->teams);
    free(engine_server);
}

static void get_timeout(struct timeval *res, engine_server_t *engine_server)
{
    double total = 1 / engine_server->frequence;

    res->tv_sec = total;
    res->tv_usec = total * 1000000;
}

bool *is_running(void)
{
    static bool should_run = true;

    return &should_run;
}

static void signal_handler(int sig)
{
    (void)sig;
    (*is_running()) = false;
}

int main(int argc, char **argv)
{
    engine_server_t *engine_server = malloc(sizeof(engine_server_t));
    server_t *server;
    struct timeval timeout;

    parsing(argc, argv, engine_server);
    prepare_engine(engine_server);
    server = create_server(engine_server->port, engine_server, d_engine_server);
    signal(SIGINT, &signal_handler);
    signal(SIGPIPE, SIG_IGN);
    while (*is_running()) {
        get_timeout(&timeout, engine_server);
        prepare_server(server);
        select_client(server, &timeout);
        accept_new_client(server, "WELCOME\n", NULL, destroy_engine_client);
        handle_clients(server, manage_clients);
        if (handle_incantations(server))
            break;
    }
    destruct_server(server);
    return 0;
}
