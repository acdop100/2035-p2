#include "map.h"
#include "globals.h"
#include "graphics.h"
#include "hash_table.h"
#include "main.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map
{
    HashTable *items; // HashTable of MapItems
    int w, h;         // Ints for width and height
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
Map map;
static int active_map;
static Map *all_maps[2]; // Array of all maps generated
int counter = 0;         // A counter to add a new map to the correct space

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
unsigned int XY_KEY(int X, int Y)
{
    unsigned int key = (X * 50) + Y; // Get the 1-D coordinate for the HT
    return (key);
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned int map_hash(unsigned key)
{
    unsigned int tableKey = key % 20; // 20 buckets
    return (tableKey);
}

void maps_init(int numBuckets, int width, int height)
{
    // Initialize hash table
    // Set width & height
    map.items = createHashTable(map_hash, numBuckets);
    map.h = height;
    map.w = width;
    all_maps[counter] = &map; // Adds map to maps array
    counter++;                // Increase counter
}

Map *get_active_map()
{
    map = *all_maps[active_map]; // Get current map from the maps array at current map position
    return &map;
}

Map *set_active_map(int m)
{
    map = *all_maps[m]; // Set the map defined at point m to be active map
    active_map = m;     // Change active map number
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'T', 'G', 'N'};
    for (int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem *item = get_here(x, y);
            if (item)
                pc.printf("%c", lookup[item->type]);
            else
                pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

// This section is pretty self - explanatory
int map_width()
{
    return (map.w);
}

int map_height()
{
    return (map.h);
}

int map_area()
{
    int h = map.h;
    int w = map.w;
    return (w * h);
}

// This section is also pretty self - explanatory
MapItem *get_north(int x, int y)
{
    MapItem *item = get_here(x, y - 1); // Gets the correct MapItem from the item HashTable
    if (!item)
    {
        pc.printf("North location does not exist (Index OOB)\r\n"); // If the MapItem doesn't exist, it returns this
        return (NULL);
    }
    else
    {
        pc.printf("North not null. \n");
        return (item);
    }
}

MapItem *get_south(int x, int y)
{
    MapItem *item = get_here(x, y + 1);

    if (!item)
    {
        pc.printf("South location does not exist (Index OOB)\r\n");
        return (NULL);
    }
    else
    {
        return (item);
    }
}

MapItem *get_east(int x, int y)
{
    MapItem *item = get_here(x + 1, y);

    if (!item)
    {
        pc.printf("East location does not exist (Index OOB)\r\n");
        return (NULL);
    }
    else
    {
        return (item);
    }
}

MapItem *get_west(int x, int y)
{
    MapItem *item = get_here(x - 1, y);

    if (!item)
    {
        pc.printf("West location does not exist (Index OOB)\r\n");
        return (NULL);
    }
    else
    {
        return (item);
    }
}

MapItem *get_here(int x, int y)
{
    Map *map = get_active_map();

    void *item2 = getItem(map->items, map_hash(XY_KEY(x, y))); // Use getItem from the HashTable API to get the MapItem at that location
    
    MapItem* item = (MapItem *)item2;
    
    if (!item)
    {
        pc.printf("location at (%d, %d) does not exist\r\n", x, y);
        return (NULL);
    }
    else
    {
        return (item);
    }
}

void map_erase(int x, int y)
{
    MapItem *item = get_here(x, y);

    if (!item)
    {
        pc.printf("Location to erase does not exist (Index OOB)\r\n");
    }
    else
    {
        item->draw = draw_nothing;
        item->type = NOTHING;
        item->walkable = 1;
    }
}

void add_wall(int x, int y, int dir, int len)
{
    for (int i = 0; i < len; i++)
    {
        MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = 0;
        
        unsigned int key = (dir == HORIZONTAL) ? XY_KEY(x + i, y) : XY_KEY(x, y + i);        
        void *val = insertItem(get_active_map()->items, key, w1);
        
        //if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
    w1->type = GRASS;
    w1->draw = draw_plant;
    w1->walkable = 1;
    
    void *val = insertItem(get_active_map()->items, map_hash(XY_KEY(x, y)), w1);
    //if (val) free(val); // If something is already there, free it
}


void add_door(int x, int y)
{
    MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = 0;
    
    unsigned int key = map_hash(XY_KEY(x, y));
    void *val = insertItem(get_active_map()->items, key, w1);
    //if (val)free(val); // If something is already there, free it
}

void add_NPC(int x, int y, int type, DrawFunc draw)
{
    MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
    w1->type = type;
    w1->draw = draw;
    w1->walkable = 0;
    void *val = insertItem(get_active_map()->items, map_hash(XY_KEY(x, y)), w1);
    //if (val)free(val); // If something is already there, free it
}

void move_NPC(int x, int y, MapItem *item, int NPCcount)
{
    map_erase(x, y);
    if (NPCcount % 2 == 0){
        void *val = insertItem(get_active_map()->items, map_hash(XY_KEY(x+1, y)), item);
    }
    else
    {
        void *val = insertItem(get_active_map()->items, map_hash(XY_KEY(x-1, y)), item);
    }
}
