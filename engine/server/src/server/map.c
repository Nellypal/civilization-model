

#include "engine.h"

static inventory_t get_goal(engine_server_t *serv)
{
    inventory_t goal;

    goal.food = serv->max_ressources.food - serv->global_inventory->food;
    goal.deraumere =
        serv->max_ressources.deraumere - serv->global_inventory->deraumere;
    goal.linemate =
        serv->max_ressources.linemate - serv->global_inventory->linemate;
    goal.mendiane =
        serv->max_ressources.mendiane - serv->global_inventory->mendiane;
    goal.phiras = serv->max_ressources.phiras - serv->global_inventory->phiras;
    goal.sibur = serv->max_ressources.sibur - serv->global_inventory->sibur;
    goal.thystame =
        serv->max_ressources.thystame - serv->global_inventory->thystame;
    return goal;
}

static void add_ressources(inventory_t **map, coords_t size_map,
    int ressource_type, int amount)
{
    coords_t coord;

    for (int i = 0; i < amount; i++) {
        coord.x = rand() % size_map.x;
        coord.y = rand() % size_map.y;
        if (ressource_type == 0)
            map[coord.y][coord.x].food++;
        if (ressource_type == 1)
            map[coord.y][coord.x].linemate++;
        if (ressource_type == 2)
            map[coord.y][coord.x].deraumere++;
        if (ressource_type == 3)
            map[coord.y][coord.x].sibur++;
        if (ressource_type == 4)
            map[coord.y][coord.x].mendiane++;
        if (ressource_type == 5)
            map[coord.y][coord.x].phiras++;
        if (ressource_type == 6)
            map[coord.y][coord.x].thystame++;
    }
}

void regenerate_ressources(engine_server_t *serv)
{
    inventory_t goal = get_goal(serv);

    add_ressources(serv->map, serv->size_map, 0, goal.food);
    add_ressources(serv->map, serv->size_map, 1, goal.linemate);
    add_ressources(serv->map, serv->size_map, 2, goal.deraumere);
    add_ressources(serv->map, serv->size_map, 3, goal.sibur);
    add_ressources(serv->map, serv->size_map, 4, goal.mendiane);
    add_ressources(serv->map, serv->size_map, 5, goal.phiras);
    add_ressources(serv->map, serv->size_map, 6, goal.thystame);
}
