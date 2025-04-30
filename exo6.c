//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h> 
#include "exo.h"


//la fonction search 
char *trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    return str;
}



int search_and_replace(char **str, HashMap *values) {
    if (!str || !*str || !values) return 0;

    int replaced = 0;
    char *input = *str;

    // Iterate through all keys in the hashmap
    for (int i = 0; i < values->size; i++) {
        if (values->table[i].key && values->table[i].key != (void *)-1) {
            char *key = values->table[i].key;
            int value = (int)(long)values->table[i].value;

            // Find potential substring match
            char *substr = strstr(input, key);
            if (substr) {
                // Construct replacement buffer
                char replacement[64];
                snprintf(replacement, sizeof(replacement), "%d", value);

                // Calculate lengths
                int key_len = strlen(key);
                int repl_len = strlen(replacement);
                //int remain_len = strlen(substr + key_len);

                // Create new string
                char *new_str = (char *)malloc(strlen(input) - key_len + repl_len + 1);
                strncpy(new_str, input, substr - input);
                new_str[substr - input] = '\0';
                strcat(new_str, replacement);
                strcat(new_str, substr + key_len);

                // Free and update original string
                free(input);
                *str = new_str;
                input = new_str;

                replaced = 1;
            }
        }
    }
    return replaced; // Ensure to return the replaced count
}
// Q6.1 Remplace les symboles dans les instructions avec leur valeur effective
int resolve_constants(ParserResult *result) {

    if (!result || !result->code_instructions) {
        return 0;
    }
    int modified = 0;


    for (int i = 0; i < result->code_count; i++) {
        Instruction *instr = result->code_instructions[i];
        if (instr->operand1 != NULL) {
            if (search_and_replace(&instr->operand1, result->memory_locations)) {
                modified = 1;
            }
            if (search_and_replace(&instr->operand1, result->labels)) {
                modified = 1;
            }
        }


        if (instr->operand2 != NULL) {
            if (search_and_replace(&instr->operand2, result->memory_locations)) {
                modified = 1;
            }
            if (search_and_replace(&instr->operand2, result->labels)) {
                modified = 1;
            }
        }
    }
    return modified; // Ensure this return statement is present
}

//Q6.2 Initialisation d'une structure CPU2 avec ses registres et gestion mémoire
CPU2 *cpu_inite(int memory_size) {
    CPU2 *cpu = malloc(sizeof(CPU2));
    if (cpu == NULL) {
        return NULL;
    }

    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();
    cpu->constant_pool = hashmap_create();

    const char *reg_names[] = {"AX", "BX", "CX", "DX", "IP", "ZF", "SF"};

    for (int i = 0; i < 7; i++) {
        int *val = malloc(sizeof(int));
        *val = 0;
        hashmap_insert(cpu->context, reg_names[i], val);
    }

    return cpu;
}

//Q6.3 Alloue le segment de code (CS) et y stocke les instructions, initialise IP à 0
void allocate_code_segment(CPU2 *cpu, Instruction **code_instructions, int code_count) {
    if (!cpu || !cpu->memory_handler || !code_instructions || code_count <= 0) {
        return;
    }

    int ok = create_segment(cpu->memory_handler, "CS", 0, code_count);
    if (!ok) {
        printf("Erreur : allocation du segment CS échouée.\n");
        return;
    }

    for (int i = 0; i < code_count; i++) {
        store(cpu->memory_handler, "CS", i, code_instructions[i]);
    }

    int *ip = hashmap_get(cpu->context, "IP");
    if (ip != NULL) {
        *ip = 0;
    }
}


//Q6.4 Exécute une instruction en modifiant les registres/drapeaux selon le mnémonique
int handle_instruction(CPU2 *cpu, Instruction *instr, void *src, void *dst) {
    if (!cpu || !instr || !instr->mnemonic) return -1;

    // Récupération des drapeaux
    int *IP = (int *)hashmap_get(cpu->context, "IP");
    int *ZF = (int *)hashmap_get(cpu->context, "ZF");
    int *SF = (int *)hashmap_get(cpu->context, "SF");

    // Juste au cas où
    if (!IP || !ZF || !SF) return -1;

    // On compare les mnémoniques
    if (strcmp(instr->mnemonic, "MOV") == 0) {
        if (dst && src) {
            *(int *)dst = *(int *)src;
        }
    }

    else if (strcmp(instr->mnemonic, "JMP") == 0) {
        if (src) {
            *IP = *(int *)src;
        }
    }

    else if (strcmp(instr->mnemonic, "JZ") == 0) {
        if (*ZF == 1 && src) {
            *IP = *(int *)src;
        }
    }

    else if (strcmp(instr->mnemonic, "JNZ") == 0) {
        if (*ZF == 0 && src) {
            *IP = *(int *)src;
        }
    }

    else if (strcmp(instr->mnemonic, "HALT") == 0) {
        // Le segment de code
        Segment *cs = (Segment *)hashmap_get(cpu->memory_handler->allocated, "CS");
        if (cs) {
            *IP = cs->start + cs->size;  // On force la sortie
        }
    }

    return 0;
}

//Q6.5 Résout les opérandes et exécute l'instruction en appelant handle_instruction
int execute_instruction(CPU2 *cpu, Instruction *instr) {
    if (!cpu || !instr) {
        return -1;
    }

    void *src = NULL;
    void *dst = NULL;

    if (instr->operand1 != NULL) {
        dst = resolve_addressing(cpu, instr->operand1);
    }

    if (instr->operand2 != NULL) {
        src = resolve_addressing(cpu, instr->operand2);
    }

    return handle_instruction(cpu, instr, src, dst);
}


//Q6.6 Récupère l’instruction pointée par IP et incrémente IP
Instruction *fetch_next_instruction(CPU2 *cpu) {
    if (!cpu || !cpu->memory_handler) {
        return NULL;
    }

    Segment *cs = hashmap_get(cpu->memory_handler->allocated, "CS");
    if (cs == NULL) {
        return NULL;
    }

    int *ip = hashmap_get(cpu->context, "IP");
    if (ip == NULL) {
        return NULL;
    }

    int index = *ip;

    if (index < 0 || index >= cs->size) {
        return NULL;
    }

    Instruction *instr = cpu->memory_handler->memory[cs->start + index];

    *ip += 1;

    return instr;
}


//Q6.7 Exécute un programme entier instruction par instruction avec pause utilisateur
int run_program(CPU2 *cpu) {
    if (cpu == NULL) {
        return -1;
    }

    printf("=== État initial du CPU ===\n");
    print_data_segment((CPU *)cpu);

    while (1) {
        Instruction *instr = fetch_next_instruction(cpu);

        if (instr == NULL) {
            printf("Fin du programme ou erreur.\n");
            break;
        }

        printf("\nInstruction : %s", instr->mnemonic);
        if (instr->operand1) {
            printf(" %s", instr->operand1);
        }
        if (instr->operand2) {
            printf(", %s", instr->operand2);
        }
        printf("\n");

        execute_instruction(cpu, instr);

        printf("Appuie sur [Entrée] pour continuer, 'q' pour quitter : ");

        char input = getchar();
        if (input == 'q' || input == 'Q') {
            printf("Arrêt demandé par l'utilisateur.\n");
            break;
        }

        // Vider le buffer (en cas d'appui sur Entrée)
        while (input != '\n' && getchar() != '\n');
    }

    printf("\n=== État final du CPU ===\n");
    print_data_segment((CPU *)cpu);

    return 0;
}
