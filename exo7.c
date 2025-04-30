//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include "exo2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_STACK 128 // Exemple d’adresse de base pour le segment SS (juste après DS)
#define STACK_TOP (BASE_STACK + STACK_SIZE - 1)

//Q 7.1
CPU2* cpu_init_stack(int memory_size) {
    CPU2 *cpu = (CPU2 *)malloc(sizeof(CPU2));
    if (!cpu) return NULL;

    // Initialiser la mémoire et les registres
    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();

    // Créer le segment de pile "SS"
    create_segment(cpu->memory_handler, "SS", BASE_STACK, STACK_SIZE);

    // Initialiser SP et BP à la fin du segment de pile
    int *sp = malloc(sizeof(int));
    int *bp = malloc(sizeof(int));
    *sp = STACK_TOP;
    *bp = STACK_TOP;
    hashmap_insert(cpu->context, "SP", sp);
    hashmap_insert(cpu->context, "BP", bp);

    return cpu;
}

//Q 7.2
int push_value(CPU2 *cpu, int value) {
    int *sp = (int *)hashmap_get(cpu->context, "SP");
    if (!sp) return -1;

    if (*sp < BASE_STACK) return -1; // Stack overflow

    int *val = malloc(sizeof(int));
    *val = value;

    int offset = *sp - BASE_STACK;
    store(cpu->memory_handler, "SS", offset, val);

    (*sp)--; // Décrémente SP après le PUSH
    return 0;
}

int pop_value(CPU2 *cpu, int *dest) {
    int *sp = (int *)hashmap_get(cpu->context, "SP");
    if (!sp) return -1;

    if (*sp >= STACK_TOP) return -1; // Stack underflow

    (*sp)++; // Incrémente SP avant le POP
    int offset = *sp - BASE_STACK;

    int *val = (int *)load(cpu->memory_handler, "SS", offset);
    if (!val) return -1;

    *dest = *val;
    free(val); // Libérer la donnée lue
    return 0;
}

//Q 7.3
void handle_instruction_2(CPU2 *cpu, const char *instruction, const char *operand) {
    if (strcmp(instruction, "PUSH") == 0) {
        int value;

        void *reg_val = hashmap_get(cpu->context, operand);
        if (reg_val) {
            value = *(int *)reg_val;
        } else {
            value = atoi(operand); // Valeur immédiate
        }

        if (push_value(cpu, value) == -1) {
            printf("Erreur : Stack Overflow\n");
        }
    } else if (strcmp(instruction, "POP") == 0) {
        int value;
        if (pop_value(cpu, &value) == -1) {
            printf("Erreur : Stack Underflow\n");
        } else {
            const char *target = (operand && strlen(operand) > 0) ? operand : "AX";
            int *val = malloc(sizeof(int));
            *val = value;
            hashmap_insert(cpu->context, target, val);
            printf("Valeur dépilée (%s) : %d\n", target, value);
        }
    }
}

