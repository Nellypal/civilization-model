

#include "engine.h"

void test_parsing(engine_server_t *serv)
{
    printf("Port: %u\n", serv->port);
    printf("Map size: %d x %d\n", serv->size_map.x, serv->size_map.y);
    printf("Clients per team: %u\n", serv->clients_per_team);
    printf("Frequency: %u\n", serv->frequence);
    printf("Team names:\n");
    for (int i = 0; serv->team_names[i] != NULL; i++) {
        printf("\t%s\n", serv->team_names[i]);
    }
}

void display_inventory(inventory_t inventory)
{
    printf("Food: %i\n", inventory.food);
    printf("Linemate: %i\n", inventory.linemate);
    printf("Deraumere: %i\n", inventory.deraumere);
    printf("Sibur: %i\n", inventory.sibur);
    printf("Mendiane: %i\n", inventory.mendiane);
    printf("Phiras: %i\n", inventory.phiras);
    printf("Thystame: %i\n", inventory.thystame);
}

void display_map(engine_server_t *serv)
{
    for (int i = 0; i < serv->size_map.y; i++) {
        for (int j = 0; j < serv->size_map.x; j++) {
            printf("Coords: X = %d | Y = %d \n", j, i);
            display_inventory(serv->map[i][j]);
            printf("\n");
        }
    }
}
