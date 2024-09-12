

#include "engine.h"

void send_broadcast(ai_client_t sender, client_t *receiver, char *msg,
    coords_t map_size)
{
    engine_client_t *engine_receiver = (engine_client_t *)receiver->data;
    ai_client_t ai_receiver = engine_receiver->client.ai_client;
    int sound = get_sound_direction(sender.coords, ai_receiver.coords,
        engine_receiver->client.ai_client.direction, map_size);
    char *message;

    asprintf(&message, "message %d, %s\n", sound, msg);
    send_to_client(receiver, message);
}

static char *append_txts(char **tabs)
{
    char *res = malloc(1);
    size_t size;

    res[0] = 0;
    for (int i = 0; tabs[i]; i++) {
        size = strlen(res) + strlen(tabs[i]) + 2;
        res = realloc(res, size);
        strcat(res, tabs[i]);
        strcat(res, " ");
        res[size - 1] = 0;
    }
    return res;
}

void broadcast_cmd(client_t *client, server_t *server, char **params)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    ai_client_t ai = engine_client->client.ai_client;
    client_t *tmp;
    engine_client_t *engine_tmp;
    char *msg = append_txts(&params[1]);

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_tmp = (engine_client_t *)tmp->data;
        if (tmp == client || engine_tmp->type != AI) {
            continue;
        }
        send_broadcast(ai, tmp, msg, engine_server->size_map);
    }
    gui_broadcast_cmd(server, client, msg);
    send_to_client(client, "ok\n");
}
