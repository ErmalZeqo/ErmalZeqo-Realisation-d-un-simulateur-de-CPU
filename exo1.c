//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"


// Q1.1
unsigned long simple_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    for (; *str; str++) {
        c = *str;
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % TABLE_SIZE;
}

// Q1.2
HashMap* hashmap_create() {
    HashMap* map = (HashMap*) malloc(sizeof(HashMap));
    if (!map) return NULL;
    map->size = TABLE_SIZE;
    map->table = (HashEntry*) calloc(TABLE_SIZE, sizeof(HashEntry));
    if (!map->table) {
        free(map);
        return NULL;
    }
    return map;
}

// Q1.3
char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}
int hashmap_insert(HashMap *map, const char *key, void *value) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long new_index = (index + i) % TABLE_SIZE;
        if (map->table[new_index].key == NULL || map->table[new_index].key == TOMBSTONE) {
            map->table[new_index].key = strdup(key);
            map->table[new_index].value = value;
            printf("Insertion de la clé : %s à l'index %lu\n", key, new_index);
            return 1;
        }
    }
    return 0;
}

//Q1.4
void* hashmap_get(HashMap* map, const char* key) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long pos = (index + i) % TABLE_SIZE;
        if (map->table[pos].key == NULL){
          return NULL;
        }
        if (strcmp(map->table[pos].key, key) == 0){
            return map->table[pos].value;
        }
    }
    return NULL;
}

//Q1.5
int hashmap_remove(HashMap* map, const char* key) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long pos = (index + i) % TABLE_SIZE;
        if (map->table[pos].key == NULL) return 0;
        if (strcmp(map->table[pos].key, key) == 0) {
            free(map->table[pos].key);
            map->table[pos].key = NULL;
            map->table[pos].value = TOMBSTONE;

            // Réinsérer les éléments suivants s'ils existent
            int j = (pos + 1) % TABLE_SIZE;
            while (map->table[j].key != NULL) {
                char* temp_key = map->table[j].key;
                void* temp_value = map->table[j].value;
                map->table[j].key = NULL;
                map->table[j].value = NULL;
                hashmap_insert(map, temp_key, temp_value);
                free(temp_key);
                j = (j + 1) % TABLE_SIZE;
            }
            return 1;
        }
    }
    return 0;
}

//1.6
void hashmap_destroy(HashMap* map) {
    if (!map) return;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->table[i].key) {
            printf("Libération de la clé : %s\n", map->table[i].key);
            
            // Free all register values (AX,BX,CX,DX,IP,ZF,SF)
            if (map->table[i].value && 
                map->table[i].value != TOMBSTONE &&
                (strcmp(map->table[i].key, "AX") == 0 ||
                 strcmp(map->table[i].key, "BX") == 0 ||
                 strcmp(map->table[i].key, "CX") == 0 ||
                 strcmp(map->table[i].key, "DX") == 0 ||
                 strcmp(map->table[i].key, "IP") == 0 ||
                 strcmp(map->table[i].key, "ZF") == 0 ||
                 strcmp(map->table[i].key, "SF") == 0)) {
                printf("Libération de la valeur à l'index %d\n", i);
                free(map->table[i].value);
            }
            
            free(map->table[i].key);
        }
    }
    free(map->table);
    free(map);
}
