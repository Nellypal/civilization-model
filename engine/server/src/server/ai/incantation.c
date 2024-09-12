

#include "engine.h"

const incantation_tab_t incantation_tab[] = {
    {2, 1, {0, 1, 0, 0, 0, 0, 0}},
    {3, 2, {0, 1, 1, 1, 0, 0, 0}},
    {4, 2, {0, 2, 0, 1, 0, 2, 0}},
    {5, 4, {0, 1, 1, 2, 0, 1, 0}},
    {6, 4, {0, 1, 2, 1, 3, 0, 0}},
    {7, 6, {0, 1, 2, 3, 0, 1, 0}},
    {8, 6, {0, 2, 2, 2, 2, 2, 1}}
};

void get_ai_at_level(incantation_data_t *data, int level,
    server_t *server, coords_t coord)
{
    client_t *tmp;
    engine_client_t *engine_tmp;
    tailq_client_t *ttmpmp = NULL;

    TAILQ_INIT(&data->ai_tail);
    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_tmp = (engine_client_t *)tmp->data;
        if (engine_tmp->type != AI ||
            (engine_tmp->client.ai_client.coords.x != coord.x ||
            engine_tmp->client.ai_client.coords.y != coord.y) ||
            (engine_tmp->client.ai_client.level != level))
            continue;
        ttmpmp = malloc(sizeof(tailq_client_t));
        ttmpmp->client = tmp;
        TAILQ_INSERT_HEAD(&data->ai_tail, ttmpmp, entries);
        ttmpmp = NULL;
    }
}

static int count_ai_in_tail(tailq_client_head_t *head)
{
    tailq_client_t *tmp;
    int i = 0;

    TAILQ_FOREACH(tmp, head, entries) {
        i++;
    }
    return i;
}

bool verify_ressource(inventory_t tile, int level)
{
    if (tile.food < incantation_tab[level].ressources.food)
        return false;
    if (tile.linemate < incantation_tab[level].ressources.linemate)
        return false;
    if (tile.deraumere < incantation_tab[level].ressources.deraumere)
        return false;
    if (tile.sibur < incantation_tab[level].ressources.sibur)
        return false;
    if (tile.mendiane < incantation_tab[level].ressources.mendiane)
        return false;
    if (tile.phiras < incantation_tab[level].ressources.phiras)
        return false;
    if (tile.thystame < incantation_tab[level].ressources.thystame)
        return false;
    return true;
}

incantation_data_t *check_condition(int targeted_level, server_t *server,
    coords_t coord)
{
    incantation_data_t *data = malloc(sizeof(incantation_data_t));
    engine_server_t *engine_server = (engine_server_t *)server->data;
    inventory_t tile = engine_server->map[coord.y][coord.x];

    get_ai_at_level(data, targeted_level - 1, server, coord);
    if (count_ai_in_tail(&data->ai_tail) <
        incantation_tab[targeted_level - 2].nb_players) {
        data->state = FAILED;
        return data;
    }
    if (!verify_ressource(tile, targeted_level - 2)) {
        data->state = FAILED;
        return data;
    }
    data->state = STARTED;
    data->level = targeted_level;
    data->coords = coord;
    gettimeofday(&data->timer, NULL);
    fill_duration(engine_server->frequence, 300, &data->duration);
    return data;
}

static char *get_ids_ais(incantation_data_t *data)
{
    char *response = NULL;
    char *tmp = NULL;
    tailq_client_t *client = NULL;
    engine_client_t *engine_tmp;

    response = strdup("");
    TAILQ_FOREACH(client, &data->ai_tail, entries) {
        engine_tmp = (engine_client_t *)(client->client->data);
        asprintf(&tmp, "%s #%d", response, engine_tmp->client.ai_client.id);
        free(response);
        response = tmp;
    }
    return response;
}

void incantation_start_cmd(client_t *client, server_t *server, char **)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    int targeted_level = engine_client->client.ai_client.level + 1;
    coords_t coord = engine_client->client.ai_client.coords;
    tailq_client_t *tmp;
    incantation_data_t *new_incatation_data;

    new_incatation_data = check_condition(targeted_level, server, coord);
    if (new_incatation_data->state == FAILED) {
        free(new_incatation_data);
        return send_to_client(client, "ko\n");
    }
    TAILQ_FOREACH(tmp, &new_incatation_data->ai_tail, entries) {
        engine_client = (engine_client_t *)(tmp->client->data);
        send_to_client(tmp->client, "Elevation underway\n");
        engine_client->client.ai_client.frozen = true;
    }
    gui_start_incantation_cmd(server, get_ids_ais(new_incatation_data),
        client, targeted_level);
    TAILQ_INSERT_TAIL(&((engine_server_t *)server->data)->incantations,
        new_incatation_data, entries);
}

bool handle_incantations(server_t *server)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    incantation_data_t *tmp = TAILQ_FIRST(&engine_server->incantations);
    incantation_data_t *next;
    // char *winner;

    while (tmp) {
        next = TAILQ_NEXT(tmp, entries);
        if (timer_is_up(&tmp->timer, &tmp->duration)) {
            incantation_end(server, tmp);
            TAILQ_REMOVE(&engine_server->incantations, tmp, entries);
            free(tmp);
        }
        tmp = next;
    }
    // winner = check_for_win(server);
    // if (winner != NULL) {
    //     printf("%s won !\n", winner);
    //     return true;
    // }
    return false;
}
