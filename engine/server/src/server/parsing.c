

#include "engine.h"

void error_message(void)
{
    printf("USAGE: ./engine_server -p port -x width -y height -n "
            "name1 name2 ... -c clientNb -f freq\n");
    exit(EXIT_FAILURE);
}

void init_teams(int argc, char **argv, engine_server_t *engine_server)
{
    int i;
    int nb_teams = 0;

    optind--;
    while (optind < argc && argv[optind][0] != '-') {
        nb_teams++;
        optind++;
    }
    engine_server->team_names = malloc(sizeof(char *) * (nb_teams + 1));
    engine_server->teams = malloc(sizeof(team_t) * (nb_teams + 1));
    for (i = 0; i < nb_teams; i++) {
        engine_server->team_names[i] = argv[optind - nb_teams + i];
        engine_server->teams[i] = malloc(sizeof(team_t));
        engine_server->teams[i]->team_name = argv[optind - nb_teams + i];
        TAILQ_INIT(&engine_server->teams[i]->eggs);
    }
    engine_server->team_names[i] = NULL;
    engine_server->teams[i] = NULL;
}

int parse_opt(int opt, int argc, char **argv, engine_server_t *engine_server)
{
    switch (opt) {
        case 'y':
            engine_server->size_map.y = atoi(optarg);
            break;
        case 'n':
            init_teams(argc, argv, engine_server);
            break;
        case 'c':
            engine_server->clients_per_team = atoi(optarg);
            break;
        case 'f':
            engine_server->frequence = atoi(optarg);
            break;
        default:
            error_message();
            return 0;
    }
    return 1;
}

void parsing(int argc, char **argv, engine_server_t *engine_server)
{
    int count = 0;

    for (int opt = getopt(argc, argv, "p:x:y:n:c:f:"); opt != -1;
        opt = getopt(argc, argv, "p:x:y:n:c:f:")) {
        switch (opt) {
        case 'p':
            engine_server->port = atoi(optarg);
            count++;
            break;
        case 'x':
            engine_server->size_map.x = atoi(optarg);
            count++;
            break;
        default:
            count += parse_opt(opt, argc, argv, engine_server);
            break;
        }
    }
    if (count != 6)
        error_message();
}
