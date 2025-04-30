#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h" // Inclure le fichier d'en-tête pour les déclarations

int main() {
    // Initialiser le CPU avec une taille de mémoire de 1024 unités
    CPU *cpu = cpu_init(1024);
    if (!cpu) {
        printf("Erreur lors de l'initialisation du CPU.\n");
        return EXIT_FAILURE;
    }

    // Créer des instructions de données pour tester l'allocation
    Instruction **data_instructions = (Instruction **)malloc(3 * sizeof(Instruction *));
    for (int i = 0; i < 3; i++) {
        data_instructions[i] = (Instruction *)malloc(sizeof(Instruction));
        data_instructions[i]->mnemonic = strdup("var"); // Nom de la variable
        data_instructions[i]->operand1 = strdup("DW"); // Type de la variable
        data_instructions[i]->operand2 = (char *)malloc(10 * sizeof(char));
        sprintf(data_instructions[i]->operand2, "%d", (i + 1) * 10); // Valeur de la variable
    }

    // Allouer des variables dans le segment de données
    allocate_variables(cpu, data_instructions, 3);

    // Afficher le contenu du segment de données
    print_data_segment(cpu);

    // Récupérer et afficher chaque valeur stockée
    for (int i = 0; i < 3; i++) {
        int *value = (int *)load(cpu->memory_handler, "DS", i);
        if (value) {
            printf("Valeur récupérée à la position %d: %d\n", i, *value);
        }
    }

    // Libérer la mémoire des instructions
    for (int i = 0; i < 3; i++) {
        free(data_instructions[i]->operand2);
        free(data_instructions[i]);
    }
    free(data_instructions);

    // Libérer les ressources du CPU
    cpu_distroy(cpu);

    return EXIT_SUCCESS;
}
