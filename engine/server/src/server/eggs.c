

#include "engine.h"

egg_t *generate_egg(bool initial_state, int frequency,
    coords_t *coords)
{
    static int id = 0;
    egg_t *egg = malloc(sizeof(egg_t));

    gettimeofday(&egg->egg_timer, NULL);
    fill_duration(frequency, EGG_LIMIT, &egg->till_hatching);
    egg->ready = initial_state;
    egg->coords.x = coords->x;
    egg->coords.y = coords->y;
    egg->id = id;
    id++;
    return egg;
}

egg_t *generate_team_eggs(team_t *team, engine_server_t *server,
    int eggs_to_create, bool initial_state)
{
    egg_t *egg;
    coords_t coords = {};

    for (int i = 0; i < eggs_to_create; i++) {
        coords.x = rand() % server->size_map.x;
        coords.y = rand() % server->size_map.y;
        egg = generate_egg(initial_state, server->frequence, &coords);
        TAILQ_INSERT_HEAD(&team->eggs, egg, entries);
    }
    return egg;
}

static void loop_through_eggs(engine_server_t *server, void (*fun)(egg_t *))
{
    egg_t *egg = NULL;

    for (int i = 0; server->teams[i]; i++) {
        TAILQ_FOREACH(egg, &server->teams[i]->eggs, entries) {
            fun(egg);
        }
    }
}

int count_team_eggs(team_t *team)
{
    int i = 0;
    egg_t *egg = NULL;

    TAILQ_FOREACH(egg, &team->eggs, entries) {
        if (egg->ready)
            i++;
    }
    return i;
}

static void update_egg(egg_t *egg)
{
    if (!egg->ready) {
        if (timer_is_up(&egg->egg_timer, &egg->till_hatching)) {
            egg->ready = true;
        }
    }
}

void update_eggs(engine_server_t *server)
{
    loop_through_eggs(server, update_egg);
}

egg_t *get_team_egg(team_t *team)
{
    egg_t *egg;
    egg_t *next;

    if (team == NULL || TAILQ_EMPTY(&team->eggs)) {
        return NULL;
    }
    egg = TAILQ_FIRST(&team->eggs);
    while (egg) {
        next = TAILQ_NEXT(egg, entries);
        if (egg->ready) {
            TAILQ_REMOVE(&team->eggs, egg, entries);
            return egg;
        }
        egg = next;
    }
    return NULL;
}
