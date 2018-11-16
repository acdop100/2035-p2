#include "map.h"
#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {

    return(key);
    // TODO: Fix me!
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    
    // TODO: Fix me!
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    map.items = createHashTable(50);
    map.h = 50;
    map.w = 50;

}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return(map.w);
}

int map_height()
{
    return(map.h);
}

int map_area()
{
    int h = map.h;
    int w = map.w;
    return(w*h);
}

MapItem* get_north(int x, int y)
{
    void *item = map(map_hash(XY_KEY(x, y+1)));
    if (!item) {
        printf("Location at given coordinates does not exist (Index OOB)");
        return(NULL);
    } else {
        return(item)
    }
}

MapItem* get_south(int x, int y)
{
    void *item = map(map_hash(XY_KEY(x, y-1)));
    if (!item) {
        printf("Location at given coordinates does not exist (Index OOB)");
        return(NULL);
    } else {
        return(item)
    }
}

MapItem* get_east(int x, int y)
{
    void *item = map(map_hash(XY_KEY(x+1, y)));
    if (!item) {
        printf("Location at given coordinates does not exist (Index OOB)");
        return(NULL);
    } else {
        return(item)
    }
}

MapItem* get_west(int x-1, int y)
{
    void *item = map(map_hash(XY_KEY(x-1, y)));
    if (!item) {
        printf("Location at given coordinates does not exist (Index OOB)");
        return(NULL);
    } else {
        return(item)
    }
}

MapItem* get_here(int x, int y)
{
    void *item = map(map_hash(XY_KEY(x, y)));
    if (!item) {
        printf("Location at given coordinates does not exist (Index OOB)");
        return(NULL);
    } else {
        return(item)
    }
}


void map_erase(int x, int y)
{
    int location = map_hash(XY_KEY(x, y));
    if (!location) {
        printf("Location at given coordinates does not exist (Index OOB)");
    } else {
        map ->location = NULL;
    }
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}