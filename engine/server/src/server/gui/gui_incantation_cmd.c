

#include "engine.h"

void gui_end_incantation_cmd(server_t *server, coords_t *coords, int level)
{
    size_t buff_size = snprintf(NULL, 0, "%s %i %i %i\n", GUI_END_INCANTATION,
    coords->x, coords->y, level) + 1;
    char *buff = malloc(buff_size);

    if (level == -1) {
        snprintf(buff, buff_size, "%s %i %i ko\n", GUI_END_INCANTATION,
        coords->x, coords->y);
    } else {
        snprintf(buff, buff_size, "%s %i %i %i\n", GUI_END_INCANTATION,
        coords->x, coords->y, level);
    }
    send_to_gui(server, buff);
    free(buff);
}

void gui_start_incantation_cmd(server_t *server, char *ids, client_t *player,
    int level)
{
    engine_client_t *engine_client = (engine_client_t *)player->data;
    size_t buff_size = snprintf(NULL, 0, "%s %i %i %i %s\n",
    GUI_START_INCANTATION, engine_client->client.ai_client.coords.x,
    engine_client->client.ai_client.coords.y, level, ids) + 1;
    char *buff = malloc(buff_size);

    snprintf(buff, buff_size, "%s %i %i %i %s\n", GUI_START_INCANTATION,
    engine_client->client.ai_client.coords.x,
    engine_client->client.ai_client.coords.y, level, ids);
    send_to_gui(server, buff);
    free(ids);
    free(buff);
}
