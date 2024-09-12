#include "engine.h"

void destroy_team_eggs(team_t *team)
{
    egg_t *egg = TAILQ_FIRST(&team->eggs);
    egg_t *next;

    while (egg) {
        next = TAILQ_NEXT(egg, entries);
        free(egg);
        egg = next;
    }
}
