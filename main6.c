//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"

int main() {
   

    ParserResult *res = malloc(sizeof(ParserResult));
    memset(res, 0, sizeof(ParserResult));
    res->code_count = 3;
    res->code_instructions = malloc(sizeof(Instruction*) * res->code_count);

    // MOV AX, [x]
    Instruction *i1 = malloc(sizeof(Instruction));
    i1->mnemonic = strdup("MOV");
    i1->operand1 = strdup("AX");
    i1->operand2 = strdup("[x]");
    res->code_instructions[0] = i1;

    // ADD AX, [y]
    Instruction *i2 = malloc(sizeof(Instruction));
    i2->mnemonic = strdup("ADD");
    i2->operand1 = strdup("AX");
    i2->operand2 = strdup("[y]");
    res->code_instructions[1] = i2;

    // HALT
    Instruction *i3 = malloc(sizeof(Instruction));
    i3->mnemonic = strdup("HALT");
    i3->operand1 = NULL;
    i3->operand2 = NULL;
    res->code_instructions[2] = i3;

    // Simuler les variables x et y
    res->memory_locations = hashmap_create();
    hashmap_insert(res->memory_locations, "x", (void*)(long)0); // x à l'adresse 0
    hashmap_insert(res->memory_locations, "y", (void*)(long)1); // y à l'adresse 1

    // Pas de labels dans ce test simple
    res->labels = hashmap_create();

    // On résout les constantes
    resolve_constants(res);

    // Création du CPU
    CPU2 *cpu = cpu_inite(64); // 64 cases mémoire


    create_segment(cpu->memory_handler, "DS", 0, 2); // 2 cases pour x et y

    // On insère manuellement les valeurs de x et y
    int *val_x = malloc(sizeof(int));
    *val_x = 5;
    store(cpu->memory_handler, "DS", 0, val_x);

    int *val_y = malloc(sizeof(int));
    *val_y = 3;
    store(cpu->memory_handler, "DS", 1, val_y);

    // On charge le code en mémoire
    allocate_code_segment(cpu, res->code_instructions, res->code_count);

    // Lancement du programmouie
    run_program(cpu);

    // Affichage final de AX
    int *AX = hashmap_get(cpu->context, "AX");
    if (AX) {
        printf("\nRésultat final dans AX : %d\n", *AX);
    }

    // Ces lignes provoquaient un double free — on les supprime :
    //free(load(cpu->memory_handler, "DS", 0));
    //free(load(cpu->memory_handler, "DS", 1));
    free_parser_result(res);
    cpu_destroy(cpu);

    return 0;
}
