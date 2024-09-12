

#include "engine.h"

team_t *get_team_by_name(engine_server_t *server, char *team_name)
{
    for (int i = 0; server->teams[i]; i++) {
        if (strcmp(server->teams[i]->team_name, team_name) == 0) {
            return server->teams[i];
        }
    }
    return NULL;
}
