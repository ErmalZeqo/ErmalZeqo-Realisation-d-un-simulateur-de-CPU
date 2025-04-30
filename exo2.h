#ifndef EXO2_H
#define EXO2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STACK_SIZE 128

// Structure pour le CPU avec gestion de la pile
typedef struct hashentry {
char * key ;
void * value ;
} HashEntry ;

typedef struct hashmap {
 int size ;
 HashEntry * table ;
} HashMap ;

typedef struct segment {
    int start;
    int size;
    struct segment *next;
} Segment;

typedef struct memoryHandler {
    void **memory;
    int total_size;
    Segment *free_list;
    HashMap *allocated;
} MemoryHandler;

typedef struct {
    MemoryHandler* memory_handler; // Gestionnaire de mémoire 
    HashMap *context;      // Registres (AX, BX, CX, DX)
    int *stack;            // Segment de pile
    int SP;                // Stack Pointer
    int BP;                // Base Pointer
} CPU2;


// Fonctions de hachage
unsigned long simple_hash(const char *str);
HashMap *hashmap_create();
char* strdup(const char* str);
int hashmap_insert(HashMap *map, const char *key, void *value);
void *hashmap_get(HashMap *map, const char *key);
int hashmap_remove(HashMap *map, const char *key);
void hashmap_destroy(HashMap *map);


// Prototypes des fonctions pour la gestion de la pile exo7
CPU2* cpu_init_stack(int memory_size);
void initialize_stack(CPU2 *cpu);
int push_value(CPU2 *cpu, int value);
int pop_value(CPU2 *cpu, int *dest);
void handle_instruction_2(CPU2 *cpu, const char *instruction, const char *operand);

// Fonctions mémoire
MemoryHandler* memory_init(int size);
int create_segment(MemoryHandler *handler, const char *name, int start, int size);
void* store(MemoryHandler *handler, const char *segment_name, int pos, void *data);
void* load(MemoryHandler *handler, const char *segment_name, int pos);

#endif // EXO2_H
