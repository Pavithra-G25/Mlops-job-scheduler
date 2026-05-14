#ifndef HASHMAP_H
#define HASHMAP_H

#define MAP_CAPACITY 20

typedef struct {
    int keys[MAP_CAPACITY];
    int values[MAP_CAPACITY];
    int used[MAP_CAPACITY];
} HashMap;

HashMap* create_hashmap(void);
void     map_set(HashMap* map, int key, int value);
int      map_get(HashMap* map, int key);
void     map_free(HashMap* map);

#endif