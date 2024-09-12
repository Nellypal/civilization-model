

#include "engine.h"

int destroy_egg_in_coord(server_t *server, egg_t *egg,
    team_t *team, coords_t coord)
{
    if (egg->coords.x == coord.x && egg->coords.y == coord.y) {
        gui_egg_death_cmd(server, egg);
        TAILQ_REMOVE(&team->eggs, egg, entries);
        free(egg);
        return 1;
    }
    return 0;
}

int destroy_eggs(server_t *s, engine_server_t *server, coords_t coord)
{
    egg_t *egg = NULL;
    egg_t *next;
    int res = 0;

    for (int i = 0; server->teams[i]; i++) {
        egg = TAILQ_FIRST(&server->teams[i]->eggs);
        while (egg) {
            next = TAILQ_NEXT(egg, entries);
            res += destroy_egg_in_coord(s, egg, server->teams[i], coord);
            egg = next;
        }
    }
    return res;
}

static int relative_eject_direction(enum direction_e ejector_direction,
    enum direction_e ejected_direction)
{
    if (ejector_direction == ejected_direction)
        return 5;
    if (abs((int8_t)ejected_direction - (int8_t)ejector_direction) == 2)
        return 1;
    if ((ejector_direction - ejected_direction == 1) ||
        ((ejected_direction == WEST) && (ejector_direction == NORTH)))
        return 3;
    if ((ejected_direction - ejector_direction == 1) ||
        ((ejected_direction == NORTH) && (ejector_direction == WEST)))
        return 7;
    return 0;
}

void eject_client(client_t *receiver, engine_client_t *engine_client,
    server_t *server)
{
    char *message;

    move_ai(receiver, engine_client->client.ai_client.direction, server);
    asprintf(&message, "eject: %d\n", relative_eject_direction(
        engine_client->client.ai_client.direction,
        ((engine_client_t *)receiver->data)->client.ai_client.direction));
    send_to_client(receiver, message);
    gui_player_pos_all(server, receiver);
    free(message);
}

void eject_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    client_t *tmp;
    engine_client_t *engine_tmp;
    coords_t coord = engine_client->client.ai_client.coords;
    int i = 0;

    TAILQ_FOREACH(tmp, &server->clients, entries) {
        engine_tmp = (engine_client_t *)tmp->data;
        if (tmp == client || !engine_tmp || engine_tmp->type != AI)
            continue;
        if (engine_tmp->client.ai_client.coords.x == coord.x &&
            engine_tmp->client.ai_client.coords.y == coord.y) {
            eject_client(tmp, engine_client, server);
            i++;
        }
    }
    i += destroy_eggs(server, engine_server, coord);
    i == 0 ? gui_expulsion_cmd(server, client) : 0;
    send_to_client(client, i == 0 ? "ko\n" : "ok\n");
}
