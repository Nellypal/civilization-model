

#include "engine.h"

void connect_nbr_cmd(client_t *client, server_t *server, char **)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *message;
    int nb = count_team_eggs(
        get_team_by_name(engine_server, engine_client->client.ai_client.team));

    asprintf(&message, "%d\n", nb);
    send_to_client(client, message);
    free(message);
}

void fork_cmd(client_t *client, server_t *server, char **)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    engine_client_t *engine_client = (engine_client_t *)client->data;
    team_t *team = get_team_by_name(engine_server,
        engine_client->client.ai_client.team);
    egg_t *egg = generate_egg(false, engine_server->frequence,
        &engine_client->client.ai_client.coords);

    TAILQ_INSERT_HEAD(&team->eggs, egg, entries);
    send_to_client(client, "ok\n");
    gui_egg_was_laid_by_a_player_cmd(server, egg, client);
}
