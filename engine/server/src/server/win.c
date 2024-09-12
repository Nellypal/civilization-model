

#include "engine.h"

int get_nb_teams(engine_server_t *server)
{
    int nb_teams;

    for (nb_teams = 0; server->team_names[nb_teams]; nb_teams++){
        continue;
    }
    return nb_teams;
}

int get_team_ind(engine_server_t *server, char *team)
{
    for (int i = 0; server->team_names[i]; i++) {
        if (strcmp(server->team_names[i], team) == 0) {
            return i;
        }
    }
    return -1;
}

char *get_winner_team(int nb_teams, int *scores,
    char **teams, server_t *server)
{
    for (int i = 0; i < nb_teams; i++) {
        if (scores[i] >= 6) {
            gui_end_game_cmd(server, teams[i]);
            return teams[i];
        }
    }
    return NULL;
}

char *check_for_win(server_t *server)
{
    engine_server_t *z_server = (engine_server_t *)server->data;
    client_t *client;
    engine_client_t *z_client;
    int nb_teams = get_nb_teams(z_server);
    int scores[nb_teams];

    memset(scores, 0, sizeof(scores) * nb_teams);
    TAILQ_FOREACH(client, &server->clients, entries) {
        z_client = (engine_client_t *)client->data;
        if (z_client == NULL || z_client->type != AI)
            continue;
        if (z_client->client.ai_client.level == 8 &&
            z_client->client.ai_client.alive)
            scores[
                get_team_ind(z_server, z_client->client.ai_client.team)
            ] += 1;
    }
    return get_winner_team(nb_teams, scores, z_server->team_names, server);
}
