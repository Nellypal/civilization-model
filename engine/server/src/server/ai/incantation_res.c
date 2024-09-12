

#include "engine.h"

static void remove_incantation_ressources(server_t *server,
    incantation_data_t *incantation)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    inventory_t *tile =
        &engine_server->map[incantation->coords.y][incantation->coords.x];

    tile->food -= incantation_tab[incantation->level - 2].ressources.food;
    tile->linemate -=
        incantation_tab[incantation->level - 2].ressources.linemate;
    tile->deraumere -=
        incantation_tab[incantation->level - 2].ressources.deraumere;
    tile->sibur -= incantation_tab[incantation->level - 2].ressources.sibur;
    tile->mendiane -=
        incantation_tab[incantation->level - 2].ressources.mendiane;
    tile->phiras -= incantation_tab[incantation->level - 2].ressources.phiras;
    tile->thystame -=
        incantation_tab[incantation->level - 2].ressources.thystame;
    gui_send_tile_content_all(server,
        incantation->coords.x, incantation->coords.y);
}

static void incantation_failure(incantation_data_t *incantation)
{
    tailq_client_t *tmp;
    engine_client_t *engine_tmp;

    TAILQ_FOREACH(tmp, &incantation->ai_tail, entries) {
        engine_tmp = (engine_client_t *)(tmp->client->data);
        engine_tmp->client.ai_client.frozen = false;
        send_to_client(tmp->client, "ko\n");
    }
}

void incantation_end(server_t *server, incantation_data_t *incantation)
{
    tailq_client_t *tmp;
    engine_client_t *engine_tmp;
    int players = 0;
    inventory_t tile = ((engine_server_t *)
        (server->data))->map[incantation->coords.y][incantation->coords.x];

    if (!verify_ressource(tile, incantation->level - 2))
        return incantation_failure(incantation);
    TAILQ_FOREACH(tmp, &incantation->ai_tail, entries) {
        engine_tmp = (engine_client_t *)(tmp->client->data);
        if (engine_tmp->client.ai_client.coords.x == incantation->coords.x &&
            engine_tmp->client.ai_client.coords.y == incantation->coords.y &&
            engine_tmp->client.ai_client.level == (incantation->level - 1) &&
            engine_tmp->client.ai_client.alive)
            players++;
    }
    if (players < incantation_tab[incantation->level - 2].nb_players)
        return incantation_failure(incantation);
    incantation_success(server, incantation);
    gui_end_incantation_cmd(server, &incantation->coords, incantation->level);
}

static void send_success(char *message, tailq_client_t *tmp, server_t *server)
{
    send_to_client(tmp->client, message);
    gui_send_level_all(server, tmp->client);
}

void incantation_success(server_t *server, incantation_data_t *incantation)
{
    char *message = NULL;
    tailq_client_t *tmp;
    ai_client_t *ai;

    asprintf(&message, "%s %d\n", "Current level:", incantation->level);
    TAILQ_FOREACH(tmp, &incantation->ai_tail, entries) {
        ai = (ai_client_t *)
        (&((engine_client_t *)(tmp->client->data))->client.ai_client);
        ai->frozen = false;
        if (ai->level == (incantation->level - 1) &&
            ai->coords.x == incantation->coords.x &&
            ai->coords.y == incantation->coords.y && ai->alive) {
                ai->level = incantation->level;
                send_success(message, tmp, server);
            } else {
                send_to_client(tmp->client, "ko\n");
            }
    }
    remove_incantation_ressources(server, incantation);
    free(message);
}
