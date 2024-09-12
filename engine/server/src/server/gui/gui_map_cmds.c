

#include "engine.h"

void gui_map_size_cmd(client_t *client, server_t *server, char **)
{
    char buffer[50];
    engine_server_t *engine_server = (engine_server_t *)server->data;

    snprintf(buffer, sizeof(buffer), "msz %i %i\n", engine_server->size_map.x,
    engine_server->size_map.y);
    send_to_client(client, buffer);
}

char *get_inventory(inventory_t *inventory)
{
    char *buff = malloc(50);

    snprintf(buff, 50, "%d %d %d %d %d %d %d\n", inventory->food,
            inventory->linemate, inventory->deraumere, inventory->sibur,
            inventory->mendiane, inventory->phiras, inventory->thystame);
    return buff;
}

void gui_content_tile_cmd(client_t *client, server_t *server, char **args)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char *invent;
    size_t buff_size;
    char *buff;

    invent = get_inventory(&engine_server->map[atoi(args[2])][atoi(args[1])]);
    buff_size = snprintf(NULL, 0, "bct %s %s %s", args[1], args[2],
    invent) + 1;
    buff = malloc(buff_size);
    snprintf(buff, buff_size, "bct %s %s %s", args[1], args[2], invent);
    send_to_client(client, buff);
    free(buff);
    free(invent);
}

void gui_content_map_cmd(client_t *client, server_t *server, char **)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char **tile_args = malloc(sizeof(char *) * 3);

    tile_args[0] = "bct";
    tile_args[1] = malloc(10);
    tile_args[2] = malloc(10);
    for (int y = 0; y < engine_server->size_map.y; y++) {
        for (int x = 0; x < engine_server->size_map.x; x++) {
            memset(tile_args[1], 0, 10);
            memset(tile_args[2], 0, 10);
            sprintf(tile_args[1], "%d", x);
            sprintf(tile_args[2], "%d", y);
            gui_content_tile_cmd(client, server, tile_args);
        }
    }
    free(tile_args[1]);
    free(tile_args[2]);
    free(tile_args);
}

void gui_send_tile_content_all(server_t *server, int x, int y)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char *invent;
    char *buffer = NULL;

    invent = get_inventory(&engine_server->map[y][x]);
    asprintf(&buffer, "bct %i %i %s", x, y, invent);
    send_to_gui(server, buffer);
    free(invent);
    free(buffer);
}
