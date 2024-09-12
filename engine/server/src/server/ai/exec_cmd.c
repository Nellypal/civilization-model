

#include "engine.h"

static int get_limit(char *cmd)
{
    for (int i = 0; ai_cmd_tab[i].cmd != NULL; i++) {
        if (!strcmp(cmd, ai_cmd_tab[i].cmd))
            return ai_cmd_tab[i].time_limit;
    }
    return -1;
}

void exec_end_fun(client_t *client, server_t *server, char *cmd)
{
    int nb;
    char **splitted_cmd = splitter(cmd, "\t \n", &nb);

    for (int i = 0; ai_cmd_tab[i].cmd != NULL; i++) {
        if (strcmp(splitted_cmd[0], ai_cmd_tab[i].cmd)) {
            continue;
        }
        if (ai_cmd_tab[i].end_fun) {
            return ai_cmd_tab[i].end_fun(client, server, splitted_cmd);
        } else {
            return;
        }
    }
}

bool set_timer(client_t *client, server_t *server, char **args)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    ai_client_t *ai = &engine_client->client.ai_client;
    int limit;

    limit = get_limit(args[0]);
    if (limit == -1)
        return false;
    fill_duration(engine_server->frequence, limit, &ai->cmd_duration);
    gettimeofday(&ai->cmd_timer, NULL);
    return true;
}

void exec_start_fun(client_t *client, server_t *server, char *cmd)
{
    int nb;
    char **splitted_cmd = splitter(cmd, "\t \n", &nb);

    if (!set_timer(client, server, splitted_cmd)) {
        send_to_client(client, "ko\n");
        free_tab(splitted_cmd);
        return;
    }
    for (int i = 0; ai_cmd_tab[i].cmd != NULL; i++) {
        if (strcmp(splitted_cmd[0], ai_cmd_tab[i].cmd))
            continue;
        if (ai_cmd_tab[i].start_fun) {
            ai_cmd_tab[i].start_fun(client, server, splitted_cmd);
            return free_tab(splitted_cmd);
        } else {
            return;
        }
    }
}
