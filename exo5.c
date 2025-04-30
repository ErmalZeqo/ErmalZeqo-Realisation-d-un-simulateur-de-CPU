// Ermal ZEQO 21315866
// Fitia SENDRISON 21318175
#include "exo.h"
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

// Fonction pour vérifier si une chaîne correspond à un motif regex
int matches(const char *pattern, const char *string) {
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result) {
        fprintf(stderr, "Regex compilation failed for pattern: %s\n", pattern);
        return 0;
    }
    result = regexec(&regex, string, 0, NULL, 0);
    regfree(&regex); // Utilisez regfree au lieu de free
    return result == 0;
}

// Q5.1 - Initialisation de la CPU2
CPU2 *cpu_init_2(int memory_size) {
    CPU2 *cpu = (CPU2 *)malloc(sizeof(CPU2));
    if (!cpu) return NULL;

    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();
    cpu->constant_pool = hashmap_create();

    // Allocate and initialize registers
    int *ax = (int *)malloc(sizeof(int));
    int *bx = (int *)malloc(sizeof(int));
    int *cx = (int *)malloc(sizeof(int));
    int *dx = (int *)malloc(sizeof(int));
    int *ip = (int *)malloc(sizeof(int));
    int *zf = (int *)malloc(sizeof(int));
    int *sf = (int *)malloc(sizeof(int));

    if (ax) *ax = 0;
    if (bx) *bx = 0;
    if (cx) *cx = 0;
    if (dx) *dx = 0;
    if (ip) *ip = 0;
    if (zf) *zf = 0;
    if (sf) *sf = 0;

    hashmap_insert(cpu->context, "AX", ax);
    hashmap_insert(cpu->context, "BX", bx);
    hashmap_insert(cpu->context, "CX", cx);
    hashmap_insert(cpu->context, "DX", dx);
    hashmap_insert(cpu->context, "IP", ip);
    hashmap_insert(cpu->context, "ZF", zf);
    hashmap_insert(cpu->context, "SF", sf);

    return cpu;
}

// Q5.2 - Destruction de la CPU2
void free_segment_data(MemoryHandler *memory_handler, const char *segment_name) {
    Segment *segment = hashmap_get(memory_handler->allocated, segment_name);
    if (segment) {
        for (int i = 0; i < segment->size; i++) {
            void *data = load(memory_handler, segment_name, i);
            if (data) {
                printf("Libération de données à l'index %d du segment %s\n", i, segment_name);
                free(data); // Libérer la mémoire allouée pour chaque valeur
            }
        }
    }
}

void cpu_destroy(CPU2 *cpu) {
    if (cpu) {
        // Remove the explicit freeing of register values since hashmap_destroy will handle it
        // Just remove these lines:
        // int *ax = (int *)hashmap_get(cpu->context, "AX");
        // int *bx = (int *)hashmap_get(cpu->context, "BX");
        // int *cx = (int *)hashmap_get(cpu->context, "CX");
        // int *dx = (int *)hashmap_get(cpu->context, "DX");
        // if (ax) free(ax);
        // if (bx) free(bx);
        // if (cx) free(cx);
        // if (dx) free(dx);

        // Libérer les données du segment "DS"
        free_segment_data(cpu->memory_handler, "DS");

        // Libérer le contexte et le pool de constantes
        hashmap_destroy(cpu->context);
        hashmap_destroy(cpu->constant_pool);

        // Libérer la mémoire du gestionnaire de mémoire
        memory_destroy(cpu->memory_handler);

        // Libérer la mémoire du CPU lui-même
        free(cpu);
    }
}

// Q5.3 - Adressage immédiat
void *immediate_addressing(CPU2 *cpu, const char *operand) {
    const char *pattern = "^[0-9]+$";
    if (matches(pattern, operand)) {
        void *value = hashmap_get(cpu->constant_pool, operand);
        if (!value) {
            int *num = (int *)malloc(sizeof(int));
            *num = atoi(operand);
            hashmap_insert(cpu->constant_pool, operand, num);
            return num;
        }
        return value;
    }
    return NULL;
}

// Q5.4 - Adressage par registre
void *register_addressing(CPU2 *cpu, const char *operand) {
    const char *pattern = "^(AX|BX|CX|DX)$";
    if (matches(pattern, operand)) {
        return hashmap_get(cpu->context, operand);
    }
    return NULL; // Si ce n'est pas un registre valide
}

// Q5.5 - Adressage direct en mémoire
void *memory_direct_addressing(CPU2 *cpu, const char *operand) {
    const char *pattern = "^\\[([0-9]+)\\]$";
    if (matches(pattern, operand)) {
        int address = atoi(operand + 1); // Extraire l'adresse
        return load(cpu->memory_handler, "DS", address); // Charger la valeur à l'adresse
    }
    return NULL; // Si ce n'est pas une adresse valide
}

// Q5.6 - Adressage indirect par registre
void *register_indirect_addressing(CPU2 *cpu, const char *operand) {
    const char *pattern = "^\\[(AX|BX|CX|DX)\\]$";
    if (matches(pattern, operand)) {
        // Extraire le nom du registre sans les crochets
        const char *reg_name = operand + 1; // reg_name commence par 'X', 'B', 'C', ou 'D'
        
        // Créer une nouvelle chaîne pour le nom du registre
        char *reg_name_copy = (char *)malloc(3); // 2 caractères + 1 pour le '\0'
        if (!reg_name_copy) {
            printf("Erreur d'allocation mémoire.\n");
            return NULL; // Gestion d'erreur
        }
        
        strncpy(reg_name_copy, reg_name, 2); // Copier les 2 caractères
        reg_name_copy[2] = '\0'; // Terminer la chaîne

        int *address = (int *)register_addressing(cpu, reg_name_copy);
        
        // Vérification si l'adresse est valide
        if (address) {
            printf("Valeur de %s : %d\n", reg_name_copy, *address); // Afficher la valeur du registre

            // Charger la valeur à l'adresse
            void *value = load(cpu->memory_handler, "DS", *address);
            free(reg_name_copy); // Libérer la mémoire après utilisation
            if (value) {
                return value; // Retourner la valeur chargée
            } else {
                printf("Erreur : Impossible de charger la valeur à l'adresse %d.\n", *address);
            }
        } else {
            printf("Erreur : registre %s non trouvé.\n", reg_name_copy);
        }
        
        free(reg_name_copy); // Libérer la mémoire après utilisation
    } else {
        printf("Erreur : format d'opérande invalide pour l'adressage indirect.\n");
    }
    return NULL; 
}
// Q5.7 - Gestion de l'instruction MOV
void handle_MOV(CPU2 *cpu, void *src, void *dest) {
    (void)cpu; // Marquer le paramètre comme non utilisé
    if (src && dest) {
        *(int *)dest = *(int *)src; // Copier la valeur de src vers dest
    }
}

// Q5.8 - Résolution d'adressage
void *resolve_addressing(CPU2 *cpu, const char *operand) {
    // Tenter de résoudre l'adressage immédiat
    void *value = immediate_addressing(cpu, operand);
    if (value != NULL) {
        return value; // Si l'adressage immédiat est applicable, retourner la valeur
    }

    // Tenter de résoudre l'adressage par registre
    value = register_addressing(cpu, operand);
    if (value != NULL) {
        return value; // Si l'adressage par registre est applicable, retourner la valeur
    }

    // Tenter de résoudre l'adressage direct
    value = memory_direct_addressing(cpu, operand);
    if (value != NULL) {
        return value; // Si l'adressage direct est applicable, retourner la valeur
    }

    // Tenter de résoudre l'adressage indirect par registre
    value = register_indirect_addressing(cpu, operand);
    if (value != NULL) {
        return value; // Si l'adressage indirect par registre est applicable, retourner la valeur
    }

    // Si aucun mode d'adressage n'est applicable, retourner NULL
    return NULL;
}

