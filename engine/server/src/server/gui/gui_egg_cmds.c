

#include "engine.h"

void gui_egg_death_cmd(server_t *server, egg_t *egg)
{
    size_t buff_size;
    char *buff;

    buff_size = snprintf(NULL, 0, "%s %i\n", GUI_DEATH_OF_AN_EGG, egg->id) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "%s %i\n", GUI_DEATH_OF_AN_EGG, egg->id);
    send_to_gui(server, buff);
    free(buff);
}

void gui_player_connection_for_an_egg_cmd(server_t *server, egg_t *egg)
{
    size_t buff_size;
    char *buff;

    buff_size = snprintf(NULL, 0, "%s %i\n", GUI_PLAYER_CONNECTION_FOR_AN_EGG,
    egg->id) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "%s %i\n", GUI_PLAYER_CONNECTION_FOR_AN_EGG,
    egg->id);
    send_to_gui(server, buff);
    free(buff);
}

void gui_egg_was_laid_by_a_player_cmd(server_t *server, egg_t *egg,
    client_t *player)
{
    engine_client_t *engine_client;
    size_t buff_size;
    char *buff;
    int id;

    if (player == NULL) {
        engine_client = NULL;
        id = -1;
    } else {
        engine_client = (engine_client_t *)player->data;
        id = engine_client->client.ai_client.id;
    }
    buff_size = snprintf(NULL, 0, "%s %i %i %i %i\n", GUI_EGG_LAID_BY_A_PLAYER,
    egg->id, id, egg->coords.x, egg->coords.y) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "%s %i %i %i %i\n", GUI_EGG_LAID_BY_A_PLAYER,
    egg->id, id, egg->coords.x, egg->coords.y);
    send_to_gui(server, buff);
    free(buff);
}
