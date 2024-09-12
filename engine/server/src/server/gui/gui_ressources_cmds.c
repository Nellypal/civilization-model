

#include "engine.h"

int get_ressource_number(char *ressource)
{
    if (ressource == NULL)
        return -1;
    if (!strcmp(ressource, "food"))
        return 0;
    if (!strcmp(ressource, "linemate"))
        return 1;
    if (!strcmp(ressource, "deraumere"))
        return 2;
    if (!strcmp(ressource, "sibur"))
        return 3;
    if (!strcmp(ressource, "mendiane"))
        return 4;
    if (!strcmp(ressource, "phiras"))
        return 5;
    if (!strcmp(ressource, "thystame"))
        return 6;
    return -1;
}

void gui_ressource_collect_cmd(server_t *server, client_t *player,
    char *ressource)
{
    int res = get_ressource_number(ressource);
    engine_client_t *engine_client = (engine_client_t *)player->data;
    size_t buff_size;
    char *buff;

    if (res == -1)
        return;
    buff_size = snprintf(NULL, 0, "%s %i %i\n", GUI_RESSOURCE_COLLECT,
    engine_client->client.ai_client.id, res) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "%s %i %i\n", GUI_RESSOURCE_COLLECT,
    engine_client->client.ai_client.id, res);
    send_to_gui(server, buff);
    free(buff);
}

void gui_ressource_drop_cmd(server_t *server, client_t *player,
    char *ressource)
{
    int res = get_ressource_number(ressource);
    engine_client_t *engine_client = (engine_client_t *)player->data;
    size_t buff_size;
    char *buff;

    if (res == -1)
        return;
    buff_size = snprintf(NULL, 0, "%s %i %i\n", GUI_RESSOURCE_DROP,
    engine_client->client.ai_client.id, res) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "%s %i %i\n", GUI_RESSOURCE_DROP,
    engine_client->client.ai_client.id, res);
    send_to_gui(server, buff);
    free(buff);
}
