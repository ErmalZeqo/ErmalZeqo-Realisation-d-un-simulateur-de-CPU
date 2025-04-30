//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#ifndef EXO_H
#define EXO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


#define TABLE_SIZE  128
#define TOMBSTONE ((void*)-1)

//declaration des structures
//la table de hachage
typedef struct hashentry {
char * key ;
void * value ;
} HashEntry ;

typedef struct hashmap {
 int size ;
 HashEntry * table ;
} HashMap ;

//Exo.2
typedef struct segment {
    int start; // Position de debut (adresse) du segment dans la memoire
    int size;// Taille du segment en unites de memoire
    struct segment *next; // Pointeur vers le segment suivant dans la liste chainee
} Segment;

typedef struct memoryHandler {
    void **memory; // Tableau de pointeurs vers la mémoire allouee
    int total_size ; // Taille totale de la mémoire gérée
    Segment * free_list ; // Liste chainee des segments de mémoire libres
    HashMap * allocated ; // Table de hachage (nom, segment)
} MemoryHandler;

//Exo.3
typedef struct {
    char *mnemonic;  // Nom de la variable
    char *operand1;  // Type (DW, DB...)
    char *operand2;  // Valeur(s)
} Instruction;

typedef struct {
    Instruction **data_instructions;
    int data_count;
    Instruction **code_instructions;
    int code_count;
    HashMap *labels;
    HashMap *memory_locations;
} ParserResult;

//Exo.4
typedef struct {
  MemoryHandler* memory_handler; //gestionnaire de memoire
  HashMap *context;             // registre (AX, BX, CX, DX)
} CPU; 

//Exo.5
typedef struct {
  MemoryHandler* memory_handler; //gestionnaire de memoire
  HashMap *context;             // registre (AX, BX, CX, DX)
  HashMap * constant_pool ;
} CPU2; 

 

//les prototypes des fonctions de l'exos 1
unsigned long simple_hash(const char *str);
HashMap *hashmap_create();
char* strdup(const char* str);
int hashmap_insert(HashMap *map, const char *key, void *value);
void *hashmap_get(HashMap *map, const char *key);
int hashmap_remove(HashMap *map, const char *key);
void hashmap_destroy(HashMap *map);

//les prototypes des fonctions de l'exos 2
MemoryHandler* memory_init(int size);
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);
int create_segment(MemoryHandler *handler, const char *name, int start, int size);
int remove_segment(MemoryHandler *handler, const char *name);
void memory_destroy(MemoryHandler *handler);

//les prototypes des fonctions de l'exos 3
Instruction *parse_data_instruction(const char *line, HashMap *memory_locations);
Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count);
ParserResult *parse(const char *filename);
void free_parser_result(ParserResult *result);

//les prototypes des fonctions de l'exos 4
CPU *cpu_init(int memory_size);
void cpu_distroy(CPU *cpu);
void *store(MemoryHandler *handler, const char *segment_name, int pos, void *data);
void *load(MemoryHandler *handler, const char *segment_name, int pos);
void allocate_variables(CPU *cpu, Instruction **data_instructions, int data_count);
void print_data_segment(CPU *cpu);

//les prototypes des fonctions de l'exos 5
int matches(const char *pattern, const char *string);
int hashmap_insert_2(HashMap *map, const char *key, void *value);
void hashmap_destroy_2(HashMap* map);
CPU2 *cpu_init_2(int memory_size);
void free_segment_data(MemoryHandler *memory_handler, const char *segment_name);
void cpu_destroy(CPU2 *cpu);
void *immediate_addressing(CPU2 *cpu, const char *operand);
void *register_addressing(CPU2 *cpu, const char *operand);
void *memory_direct_addressing(CPU2 *cpu, const char *operand);
void *register_indirect_addressing(CPU2 *cpu, const char *operand);
void handle_MOV(CPU2 *cpu, void *src, void *dest);
void *resolve_addressing(CPU2 *cpu, const char *operand);

//les prototypes de l'exos 6 
char *trim(char *str) ;
int search_and_replace(char **str, HashMap *values);
int resolve_constants(ParserResult *result);
CPU2 *cpu_inite(int memory_size);
void allocate_code_segment(CPU2 *cpu, Instruction **code_instructions, int code_count);
int handle_instruction(CPU2 *cpu, Instruction *instr, void *src, void *dest);
int execute_instruction(CPU2 *cpu, Instruction *instr);
Instruction* fetch_next_instruction(CPU2 *cpu);
int run_program(CPU2 *cpu);


#endif
