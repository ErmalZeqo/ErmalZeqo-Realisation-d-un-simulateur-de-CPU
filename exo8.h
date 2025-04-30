//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#ifndef EXO8_H
#define EXO8_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define TABLE_SIZE  128
#define TOMBSTONE ((void*)-1)

// Déclaration des structures

// La table de hachage
typedef struct hashentry {
    char *key;
    void *value;
} HashEntry;

typedef struct hashmap {
    int size;
    HashEntry *table;
} HashMap;

// Exo.2
typedef struct segment {
    int start; // Position de début (adresse) du segment dans la mémoire
    int size;  // Taille du segment en unités de mémoire
    struct segment *next; // Pointeur vers le segment suivant dans la liste chaînée
} Segment;

typedef struct memoryHandler {
    void **memory; // Tableau de pointeurs vers la mémoire allouée
    int total_size; // Taille totale de la mémoire gérée
    Segment *free_list; // Liste chaînée des segments de mémoire libres
    HashMap *allocated; // Table de hachage (nom, segment)
} MemoryHandler;

// Exo.3
typedef struct {
    char *mnemonic;  // Nom de la variable
    char *operand1;  // Type (DW, DB...)
    char *operand2;  // Valeur(s)
} Instruction;

typedef struct {
    Instruction **data_instructions; // Tableau d'instructions .DATA
    int data_count; // Nombre d'instructions .DATA
    Instruction **code_instructions; // Tableau d'instructions .CODE
    int code_count; // Nombre d'instructions .CODE
    HashMap *labels; // Labels -> indices dans code_instructions
    HashMap *memory_locations; // Noms de variables -> adresse mémoire
} ParserResult;

// Exo.4
typedef struct {
    MemoryHandler *memory_handler; // Gestionnaire de mémoire
    HashMap *context; // Registres (AX, BX, CX, DX)
} CPU;

// Exo.5
typedef struct {
    MemoryHandler *memory_handler; // Gestionnaire de mémoire
    HashMap *context; // Registres (AX, BX, CX, DX)
    HashMap *constant_pool; // Table de hachage pour stocker les valeurs immédiates
} CPU2;

unsigned long simple_hash(const char *str);
void *hashmap_get(HashMap *map, const char *key);
int create_segment(MemoryHandler *handler, const char *name, int start, int size);
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);
void *store(MemoryHandler *handler, const char *segment_name, int pos, void *data);
void *load(MemoryHandler *handler, const char *segment_name, int pos);
char* strdup(const char* str);
void hashmap_destroy(HashMap *map);
int handle_instruction(CPU2 *cpu, Instruction *instr, void *src, void *dest);
int remove_segment(MemoryHandler *handler, const char *name);
int hashmap_remove(HashMap *map, const char *key);
int hashmap_insert(HashMap *map, const char *key, void *value);
CPU2 *cpu_init_2(int memory_size);
MemoryHandler* memory_init(int size);
void memory_destroy(MemoryHandler *handler);
HashMap *hashmap_create();

// Prototypes de fonctions pour l'exercice 8
void setup_memory_and_registers(CPU2 *cpu, int memory_size);
void* segment_override_addressing(CPU2 *cpu, const char *operand);
void* resolve_addressing_2(CPU2 *cpu, const char *operand);
int find_free_address_strategy(MemoryHandler *handler, int size, int strategy);
int alloc_es_segment(CPU2 *cpu);
int free_es_segment(CPU2 *cpu);
void handle_instruction_extended(CPU2 *cpu, const char *instruction, const char *operand);

#endif // EXO8_H
