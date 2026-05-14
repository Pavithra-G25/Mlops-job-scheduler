#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

HashMap* create_hashmap(void) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        printf("ERROR: malloc failed for HashMap\n");
        exit(1);
    }
    for (int i = 0; i < MAP_CAPACITY; i++) {
        map->keys[i]   = -1;
        map->values[i] = -1;
        map->used[i]   = 0;
    }
    return map;
}

static int hash_fn(int key) {
    return key % MAP_CAPACITY;
}

void map_set(HashMap* map, int key, int value) {
    if (!map) return;
    int idx = hash_fn(key);
    for (int i = 0; i < MAP_CAPACITY; i++) {
        int probe = (idx + i) % MAP_CAPACITY;
        if (!map->used[probe] || map->keys[probe] == key) {
            map->keys[probe]   = key;
            map->values[probe] = value;
            map->used[probe]   = 1;
            return;
        }
    }
    printf("ERROR: HashMap is full\n");
}

int map_get(HashMap* map, int key) {
    if (!map) return -1;
    int idx = hash_fn(key);
    for (int i = 0; i < MAP_CAPACITY; i++) {
        int probe = (idx + i) % MAP_CAPACITY;
        if (!map->used[probe]) return -1;
        if (map->keys[probe] == key) return map->values[probe];
    }
    return -1;
}

void map_free(HashMap* map) {
    free(map);
}