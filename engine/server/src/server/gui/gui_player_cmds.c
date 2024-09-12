

#include "engine.h"

void gui_player_pos_all(server_t *server, client_t *client)
{
    char *message;
    engine_client_t *engine_client = (engine_client_t *)client->data;

    asprintf(&message, "ppo %i %i %i %i\n",
        engine_client->client.ai_client.id,
        engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y,
        engine_client->client.ai_client.direction);
    send_to_gui(server, message);
    free(message);
}

void gui_player_pos_cmd(client_t *client, server_t *server, char **args)
{
    int player_id = atoi(args[1]);
    client_t *tmp;
    engine_client_t *engine_client;
    char buffer[50];

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == AI &&
        engine_client->client.ai_client.id == player_id) {
            snprintf(buffer, sizeof(buffer), "ppo %i %i %i %i\n",
            player_id, engine_client->client.ai_client.coords.x,
            engine_client->client.ai_client.coords.y,
            engine_client->client.ai_client.direction);
            send_to_client(client, buffer);
            return;
        }
    }
}

void gui_player_level_cmd(client_t *client, server_t *server, char **args)
{
    int player_id = atoi(args[1]);
    client_t *tmp;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client_t *engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == AI &&
        engine_client->client.ai_client.id == player_id) {
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "plv %i %i\n",
            player_id, engine_client->client.ai_client.level);
            send_to_client(client, buffer);
            return;
        }
    }
}

void gui_send_level_all(server_t *server, client_t *client)
{
    char *message;
    engine_client_t *engine_client = (engine_client_t *)client->data;

    asprintf(&message, "plv %i %i\n", engine_client->client.ai_client.id,
        engine_client->client.ai_client.level);
    send_to_gui(server, message);
    free(message);
}

void gui_player_inventory_cmd(client_t *client, server_t *server, char **args)
{
    int player_id = atoi(args[1]);
    client_t *tmp;
    char buffer[50];
    char *invent;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client_t *engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == AI &&
        engine_client->client.ai_client.id == player_id) {
            invent = get_inventory(&engine_client->client.ai_client.inventory);
            snprintf(buffer, sizeof(buffer), "pin %i %i %i %s", player_id,
            engine_client->client.ai_client.coords.x,
            engine_client->client.ai_client.coords.y, invent);
            send_to_client(client, buffer);
            free(invent);
            return;
        }
    }
}
