

#include "engine.h"

void gui_connection_player_cmd_event(client_t *new_ai, client_t *gui)
{
    engine_client_t *engine_client = (engine_client_t *)new_ai->data;
    char buff[100];

    sprintf(buff, "%s %i %i %i %i %i %s\n", GUI_CONNECTION_NEW_PLAYER,
    engine_client->client.ai_client.id, engine_client->client.ai_client.coords.x,
    engine_client->client.ai_client.coords.y,
    engine_client->client.ai_client.direction,
    engine_client->client.ai_client.level,
    engine_client->client.ai_client.team);
    send_to_client(gui, buff);
}

void gui_connection_player_cmd(server_t *server, client_t *new_ai)
{
    engine_client_t *engine_client = (engine_client_t *)new_ai->data;
    char buff[100];

    sprintf(buff, "%s %i %i %i %i %i %s\n", GUI_CONNECTION_NEW_PLAYER,
    engine_client->client.ai_client.id, engine_client->client.ai_client.coords.x,
    engine_client->client.ai_client.coords.y,
    engine_client->client.ai_client.direction,
    engine_client->client.ai_client.level,
    engine_client->client.ai_client.team);
    send_to_gui(server, buff);
}

void gui_expulsion_cmd(server_t *server, client_t *ai_client)
{
    char buff[10];
    engine_client_t *engine_client = (engine_client_t *)ai_client->data;

    sprintf(buff, "%s %i\n", GUI_EXPULSION, engine_client->client.ai_client.id);
    send_to_gui(server, buff);
}

void gui_broadcast_cmd(server_t *server, client_t *ai_client, char *message)
{
    engine_client_t *engine_client = (engine_client_t *)ai_client->data;
    size_t buff_size = snprintf(NULL, 0, "%s %i %s\n", GUI_BROADCAST,
    engine_client->client.ai_client.id, message) + 1;
    char *buff = malloc(buff_size);

    snprintf(buff, buff_size, "%s %i %s\n", GUI_BROADCAST,
    engine_client->client.ai_client.id, message);
    send_to_gui(server, buff);
    free(buff);
}

void gui_death_of_player_cmd(server_t *server, client_t *dead_ai)
{
    char buff[10];
    engine_client_t *engine_client = (engine_client_t *)dead_ai->data;

    sprintf(buff, "%s %i\n", GUI_DEATH_PLAYER,
    engine_client->client.ai_client.id);
    send_to_gui(server, buff);
}
