

#include "engine.h"

char *add_ressource_to_message(char *res, int resource,
    char *resource_txt)
{
    size_t size;

    for (int i = 0; i < resource; i++) {
        size = strlen(res) + strlen(resource_txt) + 2;
        res = realloc(res, size);
        strcat(res, resource_txt);
        strcat(res, " ");
        res[size - 1] = 0;
    }
    return res;
}

char *get_items_in_inventory(inventory_t tile)
{
    char *res = malloc(1);

    res[0] = 0;
    res = add_ressource_to_message(res, tile.food, "food");
    res = add_ressource_to_message(res, tile.linemate, "linemate");
    res = add_ressource_to_message(res, tile.deraumere, "deraumere");
    res = add_ressource_to_message(res, tile.sibur, "sibur");
    res = add_ressource_to_message(res, tile.mendiane, "mendiane");
    res = add_ressource_to_message(res, tile.phiras, "phiras");
    res = add_ressource_to_message(res, tile.thystame, "thystame");
    return res;
}

void inventory_cmd(client_t *client, server_t *, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;
    inventory_t inv = engine_client->client.ai_client.inventory;

    asprintf(&message, "[food %d, linemate %d, deraumere %d, sibur %d,"
        " mendiane %d, phiras %d, thystame %d]\n", inv.food, inv.linemate,
        inv.deraumere, inv.sibur, inv.mendiane, inv.phiras, inv.thystame);
    send_to_client(client, message);
    free(message);
}

static bool inventory_contain_obj(inventory_t tile, int obj)
{
    switch (obj) {
        case 0:
            return tile.food > 0;
        case 1:
            return tile.linemate > 0;
        case 2:
            return tile.deraumere > 0;
        case 3:
            return tile.sibur > 0;
        case 4:
            return tile.mendiane > 0;
        case 5:
            return tile.phiras > 0;
        case 6:
            return tile.thystame > 0;
        default:
            return false;
    }
    return false;
}

static void inventory_add_obj(inventory_t *tile, int obj, int value)
{
    if (obj == 0)
        tile->food += value;
    if (obj == 1)
        tile->linemate += value;
    if (obj == 2)
        tile->deraumere += value;
    if (obj == 3)
        tile->sibur += value;
    if (obj == 4)
        tile->mendiane += value;
    if (obj == 5)
        tile->phiras += value;
    if (obj == 6)
        tile->thystame += value;
}

static void send_events(server_t *server, client_t *client)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;

    asprintf(&message, "pin %d %d %d %d %d %d %d %d %d %d\n",
        engine_client->client.ai_client.id,
        engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y,
        engine_client->client.ai_client.inventory.food,
        engine_client->client.ai_client.inventory.linemate,
        engine_client->client.ai_client.inventory.deraumere,
        engine_client->client.ai_client.inventory.sibur,
        engine_client->client.ai_client.inventory.mendiane,
        engine_client->client.ai_client.inventory.phiras,
        engine_client->client.ai_client.inventory.thystame);
    send_to_gui(server, message);
    free(message);
    gui_send_tile_content_all(server, engine_client->client.ai_client.coords.x,
        engine_client->client.ai_client.coords.y);
}

void take_cmd(client_t *client, server_t *server, char **params)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    int obj = get_ressource_number(params[1]);
    coords_t coord = engine_client->client.ai_client.coords;
    inventory_t tile = engine_server->map[coord.y][coord.x];

    if (inventory_contain_obj(tile, obj)) {
        inventory_add_obj(&engine_server->map[coord.y][coord.x], obj, -1);
        inventory_add_obj(&engine_client->client.ai_client.inventory, obj, 1);
        gui_ressource_collect_cmd(server, client, params[1]);
        send_events(server, client);
        send_to_client(client, "ok\n");
    } else {
        send_to_client(client, "ko\n");
    }
}

void set_cmd(client_t *client, server_t *server, char **params)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    int obj = get_ressource_number(params[1]);
    coords_t coord = engine_client->client.ai_client.coords;

    if (inventory_contain_obj(engine_client->client.ai_client.inventory, obj)) {
        inventory_add_obj(&engine_server->map[coord.y][coord.x], obj, 1);
        inventory_add_obj(&engine_client->client.ai_client.inventory, obj, -1);
        gui_ressource_drop_cmd(server, client, params[1]);
        send_events(server, client);
        send_to_client(client, "ok\n");
    } else {
        send_to_client(client, "ko\n");
    }
}
