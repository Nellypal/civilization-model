

#include "engine.h"

static const int row_length[6] = {3, 5, 7, 9, 11, 13};

static char *get_players_in_tile(server_t *server, coords_t coord)
{
    client_t *tmp;
    engine_client_t *engine_client;
    size_t size = 1;
    char *res = malloc(size);
    int ind = 0;

    res[ind] = '\0';
    TAILQ_FOREACH(tmp, &server->clients, entries) {
        engine_client = (engine_client_t *)tmp->data;
        if (!engine_client || !(engine_client->type == AI &&
            engine_client->client.ai_client.coords.x == coord.x &&
            engine_client->client.ai_client.coords.y == coord.y))
            continue;
        size += 7;
        res = realloc(res, size);
        sprintf(&res[ind], "player ");
        ind = size - 1;
        res[ind] = '\0';
        }
    return res;
}

coords_t get_new_start_coord(coords_t last_coord, enum direction_e dir,
    coords_t map_size)
{
    coords_t c = last_coord;

    if (dir == NORTH)
        c = (coords_t){last_coord.x - 1, last_coord.y - 1};
    if (dir == EAST)
        c = (coords_t){last_coord.x + 1, last_coord.y - 1};
    if (dir == SOUTH)
        c = (coords_t){last_coord.x + 1, last_coord.y + 1};
    if (dir == WEST)
        c = (coords_t){last_coord.x - 1, last_coord.y + 1};
    if (c.x < 0)
        c.x = map_size.x - 1;
    if (c.y < 0)
        c.y = map_size.y - 1;
    if (c.x >= map_size.x)
        c.x = 0;
    if (c.y >= map_size.y)
        c.y = 0;
    return c;
}

coords_t increment_coord(int i, coords_t coord, enum direction_e dir,
    coords_t map_size)
{
    coords_t c = coord;

    if (dir == NORTH)
        c = (coords_t){coord.x + i, coord.y};
    if (dir == EAST)
        c = (coords_t){coord.x, coord.y + i};
    if (dir == SOUTH)
        c = (coords_t){coord.x - i, coord.y};
    if (dir == WEST)
        c = (coords_t){coord.x, coord.y - i};
    if (c.x < 0)
        c.x = map_size.x - 1;
    if (c.y < 0)
        c.y = map_size.y - 1;
    if (c.x >= map_size.x)
        c.x = 0;
    if (c.y >= map_size.y)
        c.y = 0;
    return c;
}

static void send_content_tile(client_t *client, server_t *server,
    coords_t coord, inventory_t tile)
{
    char *res = get_players_in_tile(server, coord);
    char *res2 = get_items_in_inventory(tile);
    char *message;

    asprintf(&message, "%s%s", res, res2);
    send_to_client(client, message);
    free(res);
    free(res2);
    free(message);
}

void look_tile(client_t *client, server_t *server, int i, coords_t start_coord)
{
    coords_t t_coord;
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    ai_client_t ai = engine_client->client.ai_client;

    for (int j = 0; j < row_length[i]; j++) {
        t_coord = increment_coord(j, start_coord, ai.direction,
            engine_server->size_map);
        send_content_tile(client, server, t_coord,
            engine_server->map[t_coord.y][t_coord.x]);
        if (j + 1 != row_length[i]) {
            send_to_client(client, ", ");
        } else {
            send_to_client(client, i + 1 != ai.level ? ", " : "");
        }
    }
}

void look_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    ai_client_t ai = engine_client->client.ai_client;
    coords_t start_coord = ai.coords;

    send_to_client(client, "[");
    send_content_tile(client, server, start_coord,
        engine_server->map[start_coord.y][start_coord.x]);
    send_to_client(client, ", ");
    for (int i = 0; i < ai.level; i++) {
        start_coord = get_new_start_coord(start_coord, ai.direction,
            engine_server->size_map);
        look_tile(client, server, i, start_coord);
    }
    send_to_client(client, "]\n");
}
