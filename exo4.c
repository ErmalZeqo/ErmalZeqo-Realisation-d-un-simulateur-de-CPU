//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include "exo.h"

// Q 4.1 fonction permettant d'initialiser le processeur
CPU *cpu_init(int memory_size) {

    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    if (!cpu) return NULL;

    cpu->memory_handler = memory_init(memory_size); // Initialiser le gestionnaire de mémoire
    cpu->context = hashmap_create(); // Créer la table de hachage pour les registres

    // Initialiser les registres à zéro
    int initial_value = 0;
    
    hashmap_insert(cpu->context, "AX", &initial_value);
    hashmap_insert(cpu->context, "BX", &initial_value);
    hashmap_insert(cpu->context, "CX", &initial_value);
    hashmap_insert(cpu->context, "DX", &initial_value);

    return cpu;
}

// Q 4.2  fonction permettant  de liberer toutes les ressources allouées
void cpu_distroy(CPU *cpu) {
    if (cpu) {
        hashmap_destroy(cpu->context); // Libérer la table de hachage des registres
        memory_destroy(cpu->memory_handler); // Libérer le gestionnaire de mémoire
        free(cpu); // Libérer la structure CPU
    }
}

// Q 4.3 fonction permettant de stocker data
void *store(MemoryHandler *handler, const char *segment_name, int pos, void *data) {
    Segment *segment = hashmap_get(handler->allocated, segment_name);
    if (!segment || pos < 0 || pos >= segment->size) {
        return NULL; // Segment non trouvé ou position invalide
    }
    handler->memory[segment->start + pos] = data; // Stocker la donnée à la position spécifiée
    return data;
}

// Q 4.4 fonction permettant de recupérer les donnees stocker
void *load(MemoryHandler *handler, const char *segment_name, int pos) {
    Segment *segment = hashmap_get(handler->allocated, segment_name);
    if (!segment || pos < 0 || pos >= segment->size) {
        return NULL; // Segment non trouvé ou position invalide
    }
    return handler->memory[segment->start + pos]; // Récupérer la donnée à la position spécifiée
}

// Q 4.5 fonction permettant d'allouer dynamiquement le segment de donner (utilisation de paser)
void allocate_variables(CPU *cpu, Instruction **data_instructions, int data_count) {
    int total_size = 0;

    // Calculer la taille totale nécessaire pour les variables
    for (int i = 0; i < data_count; i++) {
        total_size += 1; // Chaque variable occupe 1 unité de mémoire
    }
    // Créer un segment de données
    create_segment(cpu->memory_handler, "DS", 0, total_size);
    // Remplir le segment de données avec les valeurs des instructions
    for (int i = 0; i < data_count; i++) {
        // Supposons que les valeurs sont stockées dans data_instructions
        int *value = (int *)malloc(sizeof(int));
        *value = atoi(data_instructions[i]->operand2); // Convertir la valeur en entier
        store(cpu->memory_handler, "DS", i, value); // Stocker la valeur dans le segment
    }
}

// Q 4.6 fonction permettant d'aficher le contenu 
void print_data_segment(CPU *cpu) {
    Segment *segment = hashmap_get(cpu->memory_handler->allocated, "DS");
    if (!segment) {
        printf("Segment de données non trouvé.\n");
        return;
    }

    printf("Contenu du segment de données (DS):\n");
    for (int i = 0; i < segment->size; i++) {
        int *data = (int *)load(cpu->memory_handler, "DS", i);
        if (data) {
            printf("Position %d: %d\n", i, *data);
        }
    }
}



