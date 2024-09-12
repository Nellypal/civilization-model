

#include "engine.h"

const gui_cmd_tab_t gui_cmd_tab[] = {
    {GUI_MAP_SIZE_CMD, gui_map_size_cmd},
    {GUI_CONTENT_TILE_CMD, gui_content_tile_cmd},
    {GUI_CONTENT_MAP_CMD, gui_content_map_cmd},
    {GUI_NAME_TEAMS, gui_name_all_team_cmd},
    {GUI_PLAYER_POSITION, gui_player_pos_cmd},
    {GUI_PLAYER_LEVEL, gui_player_level_cmd},
    {GUI_PLAYER_INVENTORY, gui_player_inventory_cmd},
    {GUI_TIME_UNIT_REQUEST, gui_time_unit_req_cmd},
    {GUI_TIME_UNIT_MODIFICATION, gui_time_unit_modif_cmd},
    {NULL, NULL}
};

void gui_name_all_team_cmd(client_t *client, server_t *server, char **)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char *buff;
    size_t len;

    for (int i = 0; engine_server->team_names[i]; i++) {
        len = snprintf(NULL, 0, "tna %s\n", engine_server->team_names[i]) + 1;
        buff = malloc(len);
        snprintf(buff, len, "tna %s\n", engine_server->team_names[i]);
        send_to_client(client, buff);
        free(buff);
    }
}

void gui_time_unit_req_cmd(client_t *client, server_t *server, char **)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char buff[10];

    sprintf(buff, "sgt %i\n", engine_server->frequence);
    send_to_client(client, buff);
}

void gui_time_unit_modif_cmd(client_t *client, server_t *server, char **args)
{
    engine_server_t *engine_server = (engine_server_t *)server->data;
    char buff[10];
    int new_freq = atoi(args[1]);

    engine_server->frequence = new_freq;
    sprintf(buff, "sgt %i\n", engine_server->frequence);
    send_to_client(client, buff);
}

void gui_end_game_cmd(server_t *server, char *team_winner)
{
    char buff[20];

    sprintf(buff, "%s %s\n", GUI_END_OF_GAME, team_winner);
    send_to_gui(server, buff);
}
