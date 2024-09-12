

#include "engine.h"

void init_cmd_queue(engine_client_t *res)
{
    res->client.ai_client.frozen = false;
    res->client.ai_client.alive = true;
    res->client.ai_client.last_cmd = NULL;
    res->client.ai_client.cmd_queue = malloc(sizeof(char *) * 11);
    for (int i = 0; i < 11; i++)
        res->client.ai_client.cmd_queue[i] = NULL;
}

void update_cmd_queue(client_t *client, ai_client_t *ai)
{
    ai->last_cmd = strdup(ai->cmd_queue[0]);
    free(ai->cmd_queue[0]);
    ai->cmd_queue[0] = NULL;
    rotate_cmd_queue(client);
}

void add_cmd_to_queue(client_t *client, char *cmd)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    ai_client_t *ai = &engine_client->client.ai_client;

    for (int i = 0; i < 10; i++) {
        if (ai->cmd_queue[i] == NULL && cmd != NULL && strlen(cmd) > 1) {
            ai->cmd_queue[i] = strdup(cmd);
            break;
        }
    }
    if (cmd)
        free(cmd);
}

void rotate_cmd_queue(client_t *client)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    ai_client_t *ai = &engine_client->client.ai_client;

    for (int i = 1; i < 10; i++) {
        ai->cmd_queue[i - 1] = ai->cmd_queue[i];
    }
}
