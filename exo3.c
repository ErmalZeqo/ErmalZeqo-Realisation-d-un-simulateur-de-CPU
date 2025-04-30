//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"



// Q3.1 - Fonction pour parser une instruction de la section .DATA
Instruction *parse_data_instruction(const char *line, HashMap *memory_locations) {
    Instruction *instruction = malloc(sizeof(Instruction));
    if (!instruction) return NULL;

    char *line_copy = strdup(line); // Dupliquer la ligne pour la manipulation
    char *mnemonic = strtok(line_copy, " ");
    char *operand1 = strtok(NULL, " ");
    char *operand2 = strtok(NULL, " ");

    instruction->mnemonic = strdup(mnemonic);
    instruction->operand1 = operand1 ? strdup(operand1) : NULL;
    instruction->operand2 = operand2 ? strdup(operand2) : NULL;

    // Associer la variable à son adresse
    int address = memory_locations->size; // Exemple d'adresse
    hashmap_insert(memory_locations, mnemonic, (void *)(long)address);

    free(line_copy); // Libérer la copie de la ligne
    return instruction;
}

// Q3.2 - instruction permettant d'analyser et stocker une ligne de la section.code
Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count) {
    Instruction *instruction = malloc(sizeof(Instruction));
    if (!instruction) return NULL;

    char *line_copy = strdup(line); // Dupliquer la ligne pour la manipulation
    char *label = strtok(line_copy, ":");
    if (label) {
        // Si une étiquette est présente
        hashmap_insert(labels, label, (void *)(long)code_count);
        strtok(NULL, ":"); // Passer à la prochaine partie après le label
    }

    char *mnemonic = strtok(NULL, " ");
    if (!mnemonic) {
        free(line_copy);
        free(instruction);
        return NULL; // Si le mnémotechnique est absent, retourner NULL
    }

    char *operand1 = strtok(NULL, ",");
    char *operand2 = strtok(NULL, ",");

    instruction->mnemonic = strdup(mnemonic);
    instruction->operand1 = operand1 ? strdup(operand1) : NULL; // Ne pas allouer si NULL
    instruction->operand2 = operand2 ? strdup(operand2) : NULL; // Ne pas allouer si NULL

    free(line_copy); // Libérer la copie de la ligne
    return instruction;
}


ParserResult *parse(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    ParserResult *result = malloc(sizeof(ParserResult));
    if (!result) return NULL;

    result->data_instructions = NULL;
    result->data_count = 0;
    result->code_instructions = NULL;
    result->code_count = 0;
    result->labels = hashmap_create(); // Créer une table de hachage pour les labels
    result->memory_locations = hashmap_create(); // Créer une table de hachage pour les adresses mémoire

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, ".DATA", 5) == 0) {
            // Passer à la section .DATA
            while (fgets(line, sizeof(line), file) && strncmp(line, ".CODE", 5) != 0) {
                Instruction *instr = parse_data_instruction(line, result->memory_locations);
                if (instr) {
                    result->data_instructions = realloc(result->data_instructions, sizeof(Instruction*) * (result->data_count + 1));
                    result->data_instructions[result->data_count++] = instr;
                }
            }
        }
        if (strncmp(line, ".CODE", 5) == 0) {
            // Passer à la section .CODE
            int code_count = 0;
            while (fgets(line, sizeof(line), file)) {
                Instruction *instr = parse_code_instruction(line, result->labels, code_count);
                if (instr) {
                    result->code_instructions = realloc(result->code_instructions, sizeof(Instruction*) * (result->code_count + 1));
                    result->code_instructions[result->code_count++] = instr;
                    code_count++;
                }
            }
        }
    }

    fclose(file);
    return result;
}

// Fonction de nettoyage
void free_parser_result(ParserResult *result) {
    // Libération des instructions .DATA
    for (int i = 0; i < result->data_count; i++) {
        free(result->data_instructions[i]->mnemonic);  // Libération de la mémoire pour chaque élément.
        free(result->data_instructions[i]->operand1);
        free(result->data_instructions[i]->operand2);
        free(result->data_instructions[i]);
    }

    // Libération des instructions .CODE
    for (int i = 0; i < result->code_count; i++) {
        free(result->code_instructions[i]->mnemonic);
        free(result->code_instructions[i]->operand1);
        free(result->code_instructions[i]->operand2);
        free(result->code_instructions[i]);
    }

    // Libération de la table des labels et des emplacements mémoire
    hashmap_destroy(result->labels);
    hashmap_destroy(result->memory_locations);

    // Libération des tableaux
    free(result->data_instructions);
    free(result->code_instructions);
    free(result);
}

