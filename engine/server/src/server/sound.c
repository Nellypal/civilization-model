

#include "engine.h"

static f_coord_t shortest_line(f_coord_t source, f_coord_t destination,
    coords_t map_size)
{
    f_coord_t vector;

    vector.x = destination.x - source.x;
    vector.y = destination.y - source.y;
    if (fabs(vector.x) > map_size.x / 2) {
        vector.x = (vector.x > 0) ? vector.x - map_size.x :
            vector.x + map_size.x;
    }
    if (fabs(vector.y) > map_size.y / 2) {
        vector.y = (vector.y > 0) ? vector.y - map_size.y :
            vector.y + map_size.y;
    }
    return vector;
}

static void rotate_tiles_value(sound_tile_t tiles[], int size)
{
    int n = size % 8;
    sound_tile_t temp[n];

    for (int i = 0; i < n; i++) {
        temp[i] = tiles[i];
    }
    for (int i = n; i < 8; i++) {
        tiles[i - n].value = tiles[i].value;
    }
    for (int i = 0; i < n; i++) {
        tiles[8 - n + i].value = temp[i].value;
    }
}

static void rotate_tiles(sound_tile_t tiles[], enum direction_e dir)
{
    if (dir == WEST) {
        rotate_tiles_value(tiles, 6);
    }
    if (dir == SOUTH) {
        rotate_tiles_value(tiles, 4);
    }
    if (dir == EAST) {
        rotate_tiles_value(tiles, 2);
    }
}

f_coord_t normalize(f_coord_t v)
{
    double magnitude = sqrt(v.x * v.x + v.y * v.y);
    f_coord_t unit_vector;

    if (magnitude != 0) {
        unit_vector.x = v.x / magnitude;
        unit_vector.y = v.y / magnitude;
    } else {
        unit_vector.x = 0;
        unit_vector.y = 0;
    }
    return unit_vector;
}

static f_coord_t hangle_toroidal_map(f_coord_t new_point, coords_t map_size)
{
    if (new_point.x >= map_size.x)
        new_point.x = 0;
    if (new_point.y >= map_size.y)
        new_point.y = 0;
    if (new_point.x < 0)
        new_point.x = map_size.x - 1;
    if (new_point.y < 0)
        new_point.y = map_size.y - 1;
    return new_point;
}

int loop_through_tiles(sound_tile_t tiles[], f_coord_t coord)
{
    for (int j = 0; j < 8; j++) {
        if (tiles[j].x == (int)coord.x && tiles[j].y == (int)coord.y) {
            return tiles[j].value;
        }
    }
    return 0;
}

int get_tile_collid(sound_tile_t tiles[], ray_t ray, coords_t destination,
    coords_t map_size)
{
    int max_size = map_size.y > map_size.x ? map_size.y : map_size.x;
    int res = 0;

    for (int i = 0; i < max_size; i++) {
        ray.origin.x += ray.dir.x;
        ray.origin.y += ray.dir.y;
        ray.origin = hangle_toroidal_map(ray.origin, map_size);
        if ((int)ray.origin.x >= destination.x - 1 &&
            (int)ray.origin.x <= destination.x + 1 &&
            (int)ray.origin.y >= destination.y - 1 &&
            (int)ray.origin.y <= destination.y + 1)
            res = loop_through_tiles(tiles, ray.origin);
        if (res != 0)
            return res;
    }
    return 0;
}

int get_sound_direction(coords_t source, coords_t destination,
    enum direction_e direction, coords_t map_size)
{
    f_coord_t s = (f_coord_t){(double)source.x + 0.5, (double)source.y + 0.5};
    f_coord_t d = (f_coord_t){(double)destination.x + 0.5,
        (double)destination.y + 0.5};
    sound_tile_t tiles[] = {
        {destination.x, destination.y - 1, 5},
        {destination.x - 1, destination.y - 1, 6},
        {destination.x - 1, destination.y, 7},
        {destination.x - 1, destination.y + 1, 8},
        {destination.x, destination.y + 1, 1},
        {destination.x + 1, destination.y + 1, 2},
        {destination.x + 1, destination.y, 3},
        {destination.x + 1, destination.y - 1, 4}
        };
    f_coord_t ray = shortest_line(s, d, map_size);
    ray_t r = {s, normalize(ray)};

    rotate_tiles(tiles, direction);
    return get_tile_collid(tiles, r, destination, map_size);
}
