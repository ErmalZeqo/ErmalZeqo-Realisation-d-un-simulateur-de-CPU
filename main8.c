#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo8.h"

/*int main() {
    // Initialisation du CPU avec gestion de pile et mémoire
    CPU *cpu = cpu_init_2(64);
    setup_memory_and_registers(cpu, 64);

    // AX = taille du segment à allouer
    int *ax = hashmap_get(cpu->context, "AX");
    *ax = 5; // 5 cases mémoire

    // BX = stratégie (0 = First Fit)
    int *bx = hashmap_get(cpu->context, "BX");
    *bx = 0;

    // Appel à ALLOC
    printf("[ALLOC] Allocation dynamique du segment ES\n");
    alloc_es_segment(cpu);

    int *es = hashmap_get(cpu->context, "ES");
    printf("Adresse de base du segment ES : %d\n", *es);

    // Simuler un stockage dans ES:AX = 42
    *ax = 2;
    int *value = malloc(sizeof(int));
    *value = 42;
    store(cpu->memory_handler, "ES", *ax, value);

    // Lire avec [ES:AX]
    void *data = segment_override_addressing(cpu, "[ES:AX]");
    if (data) {
        printf("Valeur à l'adresse [ES:AX] : %d\n", *(int *)data);
    } else {
        printf("Erreur d'accès à [ES:AX]\n");
    }

    // Libération du segment ES
    printf("[FREE] Libération du segment ES\n");
    free_es_segment(cpu);

    // Nettoyage
    hashmap_destroy(cpu->context);
    memory_destroy(cpu->memory_handler);
    free(cpu->stack);
    free(cpu);
    
    //free_cpu2(cpu);

    return 0;
}*/


int main() {
    // Initialisation du CPU avec gestion de pile et mémoire
    CPU2 *cpu = cpu_init_2(64);  // Modification ici
    setup_memory_and_registers(cpu, 64);

    // AX = taille du segment à allouer
    int *ax = hashmap_get(cpu->context, "AX");
    if (!ax) {
        printf("Erreur : registre AX non trouvé.\n");
        return -1;
    }
    *ax = 5; // 5 cases mémoire à allouer

    // BX = stratégie (0 = First Fit)
    int *bx = hashmap_get(cpu->context, "BX");
    if (!bx) {
        printf("Erreur : registre BX non trouvé.\n");
        return -1;
    }
    *bx = 0; // Stratégie de First Fit

    // Appel à ALLOC
    printf("[ALLOC] Allocation dynamique du segment ES\n");
    int alloc_result = alloc_es_segment(cpu);
    if (alloc_result == 1) {
        int *es = hashmap_get(cpu->context, "ES");
        if (es) {
            printf("Adresse de base du segment ES : %d\n", *es);
        } else {
            printf("Erreur : segment ES non trouvé après allocation.\n");
            return -1;
        }
    } else {
        printf("Échec de l'allocation du segment ES.\n");
        return -1;
    }

    // Simuler un stockage dans ES:AX = 42
    *ax = 2;  // Charger un offset dans AX
    int *value = malloc(sizeof(int));
    if (!value) {
        printf("Erreur d'allocation mémoire pour la valeur.\n");
        return -1;
    }
    *value = 42;
    store(cpu->memory_handler, "ES", *ax, value);  // Stocker la valeur dans ES[AX]

    // Lire avec [ES:AX]
    void *data = segment_override_addressing(cpu, "[ES:AX]");
    if (data) {
        printf("Valeur à l'adresse [ES:AX] : %d\n", *(int *)data);
    } else {
        printf("Erreur d'accès à [ES:AX]\n");
    }

    // Libération du segment ES
    printf("[FREE] Libération du segment ES\n");
    int free_result = free_es_segment(cpu);
    if (free_result == 0) {
        printf("Le segment ES a été libéré avec succès.\n");
    } else {
        printf("Erreur lors de la libération du segment ES.\n");
    }

    // Nettoyage
    hashmap_destroy(cpu->context);  // Libérer la hashmap du contexte
    memory_destroy(cpu->memory_handler);  // Libérer la mémoire
    free(cpu);  // Libérer le CPU

    return 0;
}

