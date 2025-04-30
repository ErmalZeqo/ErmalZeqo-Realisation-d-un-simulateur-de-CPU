#include <stdio.h>
#include "exo2.h"
#define BASE_STACK 128

int main() {
    // Initialisation du CPU avec gestionnaire mémoire et pile
    CPU2 *cpu = cpu_init_stack(1024);
    if (!cpu) {
        fprintf(stderr, "Erreur lors de l'initialisation du CPU.\n");
        return 1;
    }

    printf("Empilement de valeurs...\n");
    for (int i = 0; i < 5; i++) {
        if (push_value(cpu, i) == 0) {
            printf("Valeur %d empilée.\n", i);
        } else {
            printf("Erreur : Pile pleine lors de l'empilement de %d.\n", i);
        }
    }

    // Affichage du contenu de la pile (via load)
    printf("État de la pile après empilement :\n");
    int *sp = (int *)hashmap_get(cpu->context, "SP");
    for (int i = *sp + 1; i <= BASE_STACK + STACK_SIZE - 1; i++) {
        int *val = (int *)load(cpu->memory_handler, "SS", i - BASE_STACK);
        if (val) {
            printf("SS[%d] = %d\n", i, *val);
        } else {
            printf("SS[%d] = (null)\n", i);
        }
    }

    // Dépilement
    printf("Dépilement de valeurs...\n");
    for (int i = 0; i < 5; i++) {
        int value;
        if (pop_value(cpu, &value) == 0) {
            printf("Valeur dépilée : %d\n", value);
        } else {
            printf("Erreur : Pile vide lors du dépilement.\n");
        }
    }

    // Dépassement
    printf("Tentative de dépilement supplémentaire...\n");
    int value;
    if (pop_value(cpu, &value) == -1) {
        printf("Pile vide, impossible de dépiler.\n");
    }

    // Libérations manuelles nécessaires ici
    hashmap_destroy(cpu->context);
    // libérer chaque cellule mémoire du segment si nécessaire
    free(cpu);

    return 0;
}

