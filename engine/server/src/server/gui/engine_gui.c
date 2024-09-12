

#include "engine.h"

void manage_gui(client_t *client, server_t *server)
{
    char *cmd = get_command(client);
    int nb_args = 0;
    char **args;

    if (cmd == NULL)
        return;
    args = splitter(cmd, " \t\n", &nb_args);
    for (int i = 0; args[0] && gui_cmd_tab[i].cmd; i++)
        if (strcmp(args[0], gui_cmd_tab[i].cmd) == 0) {
            gui_cmd_tab[i].fun(client, server, args);
            free_tab(args);
            return;
        }
    return;
}

static void send_map(client_t *client, server_t *server)
{
    gui_map_size_cmd(client, server, NULL);
    gui_content_map_cmd(client, server, NULL);
    gui_name_all_team_cmd(client, server, NULL);
    gui_time_unit_req_cmd(client, server, NULL);
}

void init_events_for_gui_client(client_t *client, server_t *server)
{
    client_t *tmp;
    engine_client_t *engine_client;
    char **args = malloc(sizeof(char *) * 3);

    args[1] = malloc(10);
    send_map(client, server);
    TAILQ_FOREACH(tmp, &server->clients, entries) {
        if (!tmp->data)
            continue;
        engine_client = (engine_client_t *)tmp->data;
        if (engine_client->type == AI) {
            memset(args[1], 0, 10);
            sprintf(args[1], "%d", engine_client->client.ai_client.id);
            gui_connection_player_cmd_event(tmp, client);
            gui_player_inventory_cmd(client, server, args);
            gui_player_level_cmd(client, server, args);
        }
    }
    free(args[1]);
    free(args);
}

void register_gui(client_t *client, server_t *server)
{
    engine_client_t *res = malloc(sizeof(engine_client_t));
    engine_server_t *engine_server = (engine_server_t *)server->data;
    egg_t *egg;

    res->type = GUI;
    res->client.gui_client.client = client;
    client->data = res;
    init_events_for_gui_client(client, server);
    for (int i = 0; engine_server->teams[i]; i++) {
        TAILQ_FOREACH(egg, &engine_server->teams[i]->eggs, entries) {
            gui_egg_was_laid_by_a_player_cmd(server, egg, NULL);
        }
    }
    return;
}
