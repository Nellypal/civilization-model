

#pragma once
#include "network.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <sys/time.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sys/queue.h>
#include "splitter.h"

#define MOVE_LIMIT 7
#define TURN_LIMIT 7
#define LOOK_LIMIT 7
#define INVENTORY_LIMIT 1
#define BROADCAST_LIMIT 7
#define FORK_LIMIT 42
#define EGG_LIMIT 600
#define EJECT_LIMIT 7
#define TAKE_LIMIT 7
#define SET_LIMIT 7
#define INCATATION_LIMIT 300

#define GUI_MAP_SIZE_CMD "msz"
#define GUI_CONTENT_TILE_CMD "bct"
#define GUI_CONTENT_MAP_CMD "mct"
#define GUI_NAME_TEAMS "tna"
#define GUI_PLAYER_POSITION "ppo"
#define GUI_PLAYER_LEVEL "plv"
#define GUI_PLAYER_INVENTORY "pin"
#define GUI_TIME_UNIT_REQUEST "sgt"
#define GUI_TIME_UNIT_MODIFICATION "sst"
#define GUI_CONNECTION_NEW_PLAYER "pnw"
#define GUI_EXPULSION "pex"
#define GUI_BROADCAST "pbc"
#define GUI_START_INCANTATION "pic"
#define GUI_END_INCANTATION "pie"
#define GUI_RESSOURCE_DROP "pdr"
#define GUI_RESSOURCE_COLLECT "pgt"
#define GUI_DEATH_PLAYER "pdi"
#define GUI_PLAYER_CONNECTION_FOR_AN_EGG "ebo"
#define GUI_EGG_LAID_BY_A_PLAYER "enw"
#define GUI_DEATH_OF_AN_EGG "edi"
#define GUI_END_OF_GAME "seg"
#define GUI_MESSAGE_FROM_THE_SERVER "smg"
#define GUI_UNKNOWN_CMD "suc"
#define GUI_COMMAND_PARAM "sbp"

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct coords_s {
    int x;
    int y;
} coords_t;

typedef struct f_coord_s {
    double x;
    double y;
} f_coord_t;

typedef struct sound_tile_s {
    double x;
    double y;
    int value;
} sound_tile_t;

typedef struct ray_s {
    f_coord_t origin;
    f_coord_t dir;
} ray_t;

enum direction_e {
    OVERFLOW1 = 0,
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4,
    OVERFLOW2 = 5,
};

enum incantation_state {
    UNKNOWN,
    FAILED,
    STARTED,
    SUCCESS,
};

typedef struct gui_client_s {
    client_t *client;
} gui_client_t;

typedef struct tailq_client_s {
    client_t *client;
    TAILQ_ENTRY(tailq_client_s) entries;
} tailq_client_t;

typedef TAILQ_HEAD(tailq_client_head, tailq_client_s) tailq_client_head_t;

typedef struct incantation_data_s {
    tailq_client_head_t ai_tail;
    enum incantation_state state;
    int level;
    coords_t coords;
    struct timeval timer;
    struct timeval duration;
    TAILQ_ENTRY(incantation_data_s) entries;
} incantation_data_t;

TAILQ_HEAD(tailq_incatation, incantation_data_s);

typedef struct ai_client_s {
    // client_t *client;
    coords_t coords;
    bool alive;
    int level;
    inventory_t inventory;
    char *team;
    //pointer vers tesm_struct
    enum direction_e direction;
    int id;
    bool frozen;
    struct timeval cmd_timer;
    struct timeval cmd_duration;
    struct timeval food_timer;
    struct timeval till_death;
    char **cmd_queue;
    char *last_cmd;
} ai_client_t;

union engine_client_union {
    gui_client_t gui_client;
    ai_client_t ai_client;
};

enum client_type {
    GUI,
    AI
};

typedef struct egg_s {
    coords_t coords;
    bool ready;
    int id;
    struct timeval egg_timer;
    struct timeval till_hatching;
    TAILQ_ENTRY(egg_s) entries;
} egg_t;

TAILQ_HEAD(tailq_egg, egg_s);

typedef struct team_s {
    char *team_name;
    struct tailq_egg eggs;
} team_t;

typedef struct engine_client_s {
    enum client_type type;
    union engine_client_union client;
} engine_client_t;

typedef struct engine_server_s {
    inventory_t *global_inventory;
    inventory_t max_ressources;
    inventory_t **map;
    coords_t size_map;
    char **team_names;
    team_t **teams;
    unsigned int clients_per_team;
    unsigned int frequence;
    unsigned int port;
    bool running;
    struct tailq_incatation incantations;
} engine_server_t;

typedef struct incantation_tab_s {
    int level;
    int nb_players;
    inventory_t ressources;
} incantation_tab_t;

extern const incantation_tab_t incantation_tab[];

typedef struct ai_cmd_tab_s {
    char *cmd;
    int time_limit;
    void (*start_fun)(client_t *, server_t *, char **);
    void (*end_fun)(client_t *, server_t *, char **);
} ai_cmd_tab_t;

extern const ai_cmd_tab_t ai_cmd_tab[];


typedef struct gui_cmd_tab_s {
    char *cmd;
    void (*fun)(client_t *, server_t *, char **);
} gui_cmd_tab_t;

extern const gui_cmd_tab_t gui_cmd_tab[];

void parsing(int argc, char **argv, engine_server_t *engine_server);
void test_parsing(engine_server_t *serv);
void display_inventory(inventory_t inventory);
void regenerate_ressources(engine_server_t *serv);
void display_map(engine_server_t *serv);
void free_tab(char **tab);

bool handle_incantations(server_t *server);
void incantation_end(server_t *server, incantation_data_t *incantation);
void incantation_success(server_t *server, incantation_data_t *incantation);
bool verify_ressource(inventory_t tile, int level);
char *check_for_win(server_t *server);
// timer
bool timer_is_up(struct timeval *timer, struct timeval *duration);
bool update_duration(struct timeval *result,
    const struct timeval *present, const struct timeval *past);
void fill_duration(int frequence, int time_limit, struct timeval *res);
// team
team_t *get_team_by_name(engine_server_t *server, char *team_name);
// egg
egg_t *generate_egg(bool initial_state, int frequency,
    coords_t *coords);
egg_t *generate_team_eggs(team_t *team, engine_server_t *server,
    int eggs_to_create, bool initial_state);
egg_t *get_team_egg(team_t *team);
int count_team_eggs(team_t *team);
void destroy_team_eggs(team_t *team);
void update_eggs(engine_server_t *server);
// sound
int get_sound_direction(coords_t source, coords_t destination,
    enum direction_e direction, coords_t map_size);
//ai
void register_ai(engine_server_t *server, client_t *client, char *team,
    server_t *server_main);
void destroy_ai_client(engine_client_t *engine_client);
void manage_ai(client_t *client, server_t *server);
void exec_end_fun(client_t *client, server_t *server, char *cmd);
void exec_start_fun(client_t *client, server_t *server, char *cmd);
void move_ai(client_t *client, enum direction_e direction, server_t *server);
void move_cmd(client_t *client, server_t *server, char **);
void right_cmd(client_t *client, server_t *server, char **);
void left_cmd(client_t *client, server_t *server, char **);
void connect_nbr_cmd(client_t *client, server_t *server, char **);
void fork_cmd(client_t *client, server_t *server, char **);
void look_cmd(client_t *client, server_t *server, char **);
char *get_items_in_inventory(inventory_t tile);
void inventory_cmd(client_t *client, server_t *, char **);
void broadcast_cmd(client_t *client, server_t *server, char **params);
void eject_cmd(client_t *client, server_t *server, char **);
void take_cmd(client_t *client, server_t *server, char **params);
void set_cmd(client_t *client, server_t *server, char **params);
void incantation_start_cmd(client_t *client, server_t *server, char **);
//gui
void register_gui(client_t *client, server_t *server);
void manage_gui(client_t *client, server_t *server);
void send_to_gui(server_t *server, char *message);
void gui_map_size_cmd(client_t *client, server_t *server, char **);
void gui_content_tile_cmd(client_t *client, server_t *server, char **args);
void gui_content_map_cmd(client_t *client, server_t *server, char **);
void gui_send_tile_content_all(server_t *server, int x, int y);
void gui_name_all_team_cmd(client_t *client, server_t *server, char **);
void gui_player_pos_all(server_t *server, client_t *client);
void gui_player_pos_cmd(client_t *client, server_t *server, char **args);
void gui_player_level_cmd(client_t *client, server_t *server, char **args);
void gui_send_level_all(server_t *server, client_t *client);
void gui_player_inventory_cmd(client_t *client, server_t *server, char **args);
void gui_time_unit_req_cmd(client_t *client, server_t *server, char **);
void gui_time_unit_modif_cmd(client_t *client, server_t *server, char **args);
//
void gui_connection_player_cmd(server_t *server, client_t *new_ai);
void gui_expulsion_cmd(server_t *server, client_t *ai_client);
void gui_broadcast_cmd(server_t *server, client_t *ai_client, char *message);
void gui_death_of_player_cmd(server_t *server, client_t *dead_ai);
void gui_start_incantation_cmd(server_t *server, char *ids, client_t *player,
    int level);
void gui_end_incantation_cmd(server_t *server, coords_t *coords, int level);
void gui_ressource_drop_cmd(server_t *server, client_t *player,
    char *ressource);
void gui_ressource_collect_cmd(server_t *server, client_t *player,
    char *ressource);
void gui_end_game_cmd(server_t *server, char *team_winner);
void gui_egg_death_cmd(server_t *server, egg_t *egg);
void gui_egg_was_laid_by_a_player_cmd(server_t *server, egg_t *egg, client_t
    *player);
void gui_player_connection_for_an_egg_cmd(server_t *server, egg_t *egg);
char *get_inventory(inventory_t *inventory);
int get_ressource_number(char *ressource);
void gui_connection_player_cmd_event(client_t *new_ai, client_t *gui);
//cmd queue
void init_cmd_queue(engine_client_t *res);
void update_cmd_queue(client_t *client, ai_client_t *ai);
void add_cmd_to_queue(client_t *client, char *cmd);
void rotate_cmd_queue(client_t *client);
