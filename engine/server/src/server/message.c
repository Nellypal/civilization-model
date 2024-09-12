

#include "engine.h"

void send_to_gui(server_t *server, char *message)
{
    client_t *tmp;
    engine_client_t *engine_client;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == GUI) {
            send_to_client(tmp, message);
        }
    }
}

void send_to_ai(server_t *server, char *message)
{
    client_t *tmp;
    engine_client_t *engine_client;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == AI) {
            send_to_client(tmp, message);
        }
    }
}
