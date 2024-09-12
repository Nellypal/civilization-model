

#include "engine.h"

const ai_cmd_tab_t ai_cmd_tab[] = {
    {"Forward", MOVE_LIMIT, NULL, &move_cmd},
    {"Right", TURN_LIMIT, NULL, &right_cmd},
    {"Left", TURN_LIMIT, NULL, &left_cmd},
    {"Look", LOOK_LIMIT, NULL, &look_cmd},
    {"Inventory", INVENTORY_LIMIT, NULL, &inventory_cmd},
    {"Broadcast", BROADCAST_LIMIT, NULL, &broadcast_cmd},
    {"Connect_nbr", 0, NULL, &connect_nbr_cmd},
    {"Fork", FORK_LIMIT, NULL, &fork_cmd},
    {"Eject", EJECT_LIMIT, NULL, &eject_cmd},
    {"Take", TAKE_LIMIT, NULL, &take_cmd},
    {"Set", SET_LIMIT, NULL, &set_cmd},
    {"Incantation", INCATATION_LIMIT, &incantation_start_cmd,
        NULL},
    {NULL, 0, NULL, NULL}
};
