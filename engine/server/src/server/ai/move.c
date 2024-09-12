

#include "engine.h"

void move_ai(client_t *client, enum direction_e direction, server_t *server)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    engine_client_t *engine_client = (engine_client_t *)client->data;

    if (direction == SOUTH)
        engine_client->client.ai_client.coords.y += 1;
    if (direction == NORTH)
        engine_client->client.ai_client.coords.y -= 1;
    if (direction == EAST)
        engine_client->client.ai_client.coords.x += 1;
    if (direction == WEST)
        engine_client->client.ai_client.coords.x -= 1;
    if (engine_client->client.ai_client.coords.x < 0)
        engine_client->client.ai_client.coords.x = engine_server->size_map.x - 1;
    if (engine_client->client.ai_client.coords.y < 0)
        engine_client->client.ai_client.coords.y = engine_server->size_map.y - 1;
    if (engine_client->client.ai_client.coords.x >= engine_server->size_map.x)
        engine_client->client.ai_client.coords.x = 0;
    if (engine_client->client.ai_client.coords.y >= engine_server->size_map.y)
        engine_client->client.ai_client.coords.y = 0;
}

void move_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;

    move_ai(client, engine_client->client.ai_client.direction, server);
    asprintf(&message, "ppo %d %d %d %d\n", engine_client->client.ai_client.id,
        engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y,
        engine_client->client.ai_client.direction);
    send_to_gui(server, message);
    free(message);
    send_to_client(client, "ok\n");
}

void left_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;

    engine_client->client.ai_client.direction =
        engine_client->client.ai_client.direction - 1;
    if (engine_client->client.ai_client.direction == 0) {
        engine_client->client.ai_client.direction = 4;
    }
    asprintf(&message, "ppo %d %d %d %d\n",
        engine_client->client.ai_client.id,
        engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y,
        engine_client->client.ai_client.direction);
    send_to_gui(server, message);
    free(message);
    send_to_client(client, "ok\n");
}

void right_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;

    engine_client->client.ai_client.direction =
        engine_client->client.ai_client.direction + 1;
    if (engine_client->client.ai_client.direction == 5) {
        engine_client->client.ai_client.direction = 1;
    }
    asprintf(&message, "ppo %d %d %d %d\n", engine_client->client.ai_client.id,
        engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y,
        engine_client->client.ai_client.direction);
    send_to_gui(server, message);
    free(message);
    send_to_client(client, "ok\n");
}
