//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"  

typedef struct {
    char *mnemonic;  // Nom de la variable
    char *operand1;  // Type (DW, DB...)
    char *operand2;  // Valeur(s)
} Instruction;

Instruction *parse_data_instruction(const char *line, HashMap *memory_locations) {
    Instruction *inst = malloc(sizeof(Instruction));
    inst->mnemonic = strtok(strdup(line), " ");  // Récupère le nom de la variable
    inst->operand1 = strtok(NULL, " ");          // Récupère le type (DW, DB)
    inst->operand2 = strtok(NULL, "");           // Récupère la valeur (ex: "3" ou "1, 2, 3")

    // Stocker l’adresse mémoire actuelle de la variable
    static int current_address = 0;  
    hashmap_put(memory_locations, inst->mnemonic, current_address);

    // Calculer la taille en mémoire (chaque valeur compte comme une case)
    int element_count = 1;
    if (inst->operand2 && strchr(inst->operand2, ',')) {
        element_count += count_commas(inst->operand2);
    }
    
    current_address += element_count;  // Mise à jour de l’adresse mémoire
    return inst;
}

// Fonction pour compter les virgules (utilisée pour savoir combien d’éléments il y a)
int count_commas(const char *str) {
    int count = 0;
    while (*str) {
        if (*str == ',') count++;
        str++;
    }
    return count;
}
