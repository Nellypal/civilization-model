

#include "engine.h"

void send_events_food(server_t *server, client_t *client)
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
}

static bool dead_of_hunger(client_t *client, server_t *server)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    engine_server_t *engine_server = (engine_server_t *)server->data;
    ai_client_t *ai = &engine_client->client.ai_client;

    if (!ai->alive)
        return true;
    if (timer_is_up(&ai->food_timer, &ai->till_death)) {
        ai->inventory.food -= 1;
        fill_duration(engine_server->frequence, 125, &ai->till_death);
        if (ai->inventory.food < 0) {
            ai->alive = false;
            gui_death_of_player_cmd(server, client);
            send_to_client(client, "dead\n");
            return true;
        }
        send_events_food(server, client);
    }
    return false;
}

void manage_ai(client_t *client, server_t *server)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    ai_client_t *ai = &engine_client->client.ai_client;
    char *cmd = get_command(client);

    if (dead_of_hunger(client, server))
        return;
    if (cmd != NULL)
        add_cmd_to_queue(client, cmd);
    if (!ai->alive || ai->frozen)
        return;
    if (timer_is_up(&ai->cmd_timer, &ai->cmd_duration)) {
        if (ai->last_cmd) {
            exec_end_fun(client, server, ai->last_cmd);
            ai->last_cmd = NULL;
        }
        if (ai->cmd_queue[0]) {
            update_cmd_queue(client, ai);
            exec_start_fun(client, server, ai->last_cmd);
        }
    }
}

static void send_response_registration(engine_server_t *server,
    client_t *client)
{
    engine_client_t *engine_client = (engine_client_t *)client->data;
    char *msg;

    asprintf(&msg, "%d\n", count_team_eggs(get_team_by_name(server,
    engine_client->client.ai_client.team)));
    send_to_client(client, msg);
    free(msg);
    asprintf(&msg, "%d %d\n", server->size_map.x, server->size_map.y);
    send_to_client(client, msg);
    free(msg);
}

static void ai_registration_helper(engine_client_t *res)
{
    int dir = rand() % 5;

    if (dir == 0)
        dir++;
    res->client.ai_client.direction = dir;
    res->type = AI;
    res->client.ai_client.level = 1;
    res->client.ai_client.cmd_duration = (struct timeval){0, 0};
    init_cmd_queue(res);
}

void register_ai(engine_server_t *server, client_t *client, char *team,
    server_t *server_main)
{
    egg_t *egg = get_team_egg(get_team_by_name(server, team));
    engine_client_t *res;
    static int nb_players = 0;

    if (egg == NULL)
        return send_to_client(client, "ko\n");
    res = malloc(sizeof(engine_client_t));
    ai_registration_helper(res);
    res->client.ai_client.team = strdup(team);
    res->client.ai_client.inventory = (inventory_t){10, 0, 0, 0, 0, 0, 0};
    res->client.ai_client.id = nb_players;
    gettimeofday(&res->client.ai_client.cmd_timer, NULL);
    gettimeofday(&res->client.ai_client.food_timer, NULL);
    fill_duration(server->frequence, 126, &res->client.ai_client.till_death);
    res->client.ai_client.coords = egg->coords;
    client->data = res;
    free(egg);
    nb_players++;
    send_response_registration(server, client);
    gui_connection_player_cmd(server_main, client);
}

static void destroy_cmd_queue(char **cmd_queue)
{
    for (int i = 0; i < 11; ++i) {
        if (cmd_queue[i]) {
            free(cmd_queue[i]);
        }
    }
}

void destroy_ai_client(engine_client_t *engine_client)
{
    if (engine_client->client.ai_client.team) {
        free(engine_client->client.ai_client.team);
    }
    if (engine_client->client.ai_client.cmd_queue) {
        destroy_cmd_queue(engine_client->client.ai_client.cmd_queue);
        free(engine_client->client.ai_client.cmd_queue);
    }
}
