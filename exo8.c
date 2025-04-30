#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "exo8.h"

void setup_memory_and_registers(CPU2 *cpu, int memory_size) {
    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();

    int *ax = malloc(sizeof(int)); *ax = 0;
    int *bx = malloc(sizeof(int)); *bx = 0;
    int *zf = malloc(sizeof(int)); *zf = 0;
    int *es = malloc(sizeof(int)); *es = -1;

    hashmap_insert(cpu->context, "AX", ax);
    hashmap_insert(cpu->context, "BX", bx);
    hashmap_insert(cpu->context, "ZF", zf);
    hashmap_insert(cpu->context, "ES", es);
}


// Q8.1 - [SEGMENT:REGISTRE]
void* segment_override_addressing(CPU2 *cpu, const char *operand) {
    if (!cpu || !operand) return NULL;

    regex_t regex;
    regmatch_t matches[3];
    const char *pattern = "\\[([A-Z]{2}):([A-Z]{2})\\]";
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) return NULL;

    int valid = regexec(&regex, operand, 3, matches, 0);
    regfree(&regex);
    if (valid != 0) return NULL;

    char segment[3], reg[3];
    strncpy(segment, operand + matches[1].rm_so, 2); segment[2] = '\0';
    strncpy(reg, operand + matches[2].rm_so, 2);     reg[2] = '\0';

    int *offset = hashmap_get(cpu->context, reg);
    if (!offset) return NULL;

    return load(cpu->memory_handler, segment, *offset);
}

// Q8.2 - Extension resolve_addressing
void* resolve_addressing_2(CPU2 *cpu, const char *operand) {
    if (strstr(operand, ":")) return segment_override_addressing(cpu, operand);
    return NULL; // à compléter selon autres modes si besoin
}

// Q8.3 - Trouver segment libre selon stratégie
int find_free_address_strategy(MemoryHandler *handler, int size, int strategy) {
    if (!handler || size <= 0) return -1;
    Segment *cur = handler->free_list;
    Segment *choix = NULL;
    int best_metric = (strategy == 1) ? __INT_MAX__ : -1;

    while (cur) {
        if (cur->size >= size) {
            int diff = cur->size - size;
            if ((strategy == 0) ||
                (strategy == 1 && diff < best_metric) ||
                (strategy == 2 && diff > best_metric)) {
                choix = cur;
                best_metric = diff;
                if (strategy == 0) break;
            }
        }
        cur = cur->next;
    }
    return choix ? choix->start : -1;
}

// Q8.5 - ALLOC
int alloc_es_segment(CPU2 *cpu) {
    int *ax = hashmap_get(cpu->context, "AX");
    int *bx = hashmap_get(cpu->context, "BX");
    int *zf = hashmap_get(cpu->context, "ZF");
    int *es = hashmap_get(cpu->context, "ES");
    if (!ax || !bx || !zf || !es) return -1;

    int size = *ax;
    int strat = *bx;
    int base = find_free_address_strategy(cpu->memory_handler, size, strat);
    if (base == -1 || !create_segment(cpu->memory_handler, "ES", base, size)) {
        *zf = 1;
        return 0;
    }

    for (int i = 0; i < size; i++) {
        int *zero = malloc(sizeof(int));
        *zero = 0;
        store(cpu->memory_handler, "ES", i, zero);
    }

    *es = base;
    *zf = 0;
    return 1;
}

// Q8.6 - FREE
int free_es_segment(CPU2 *cpu) {
    int *es = hashmap_get(cpu->context, "ES");
    if (!es || *es == -1) return -1;

    Segment *seg = hashmap_get(cpu->memory_handler->allocated, "ES");
    if (!seg) return -1;

    for (int i = 0; i < seg->size; i++) {
        void *val = load(cpu->memory_handler, "ES", i);
        if (val) free(val);
    }

    remove_segment(cpu->memory_handler, "ES");
    *es = -1;
    return 0;
}

// Q8.7 - Ajout dans handle_instruction (à intégrer dans ton switch)

void handle_instruction_extended(CPU2 *cpu, const char *instruction, const char *operand) {
    if (strcmp(instruction, "ALLOC") == 0) {
        alloc_es_segment(cpu);
    } else if (strcmp(instruction, "FREE") == 0) {
        free_es_segment(cpu);
    } else {
        Instruction instr;
        instr.mnemonic = strdup(instruction); // si nécessaire, ou initialise directement

        char *src = strdup(operand);  // à adapter si tu dois parser des registres, adresses...
        char *dest = NULL; // si pas d'autre opérande dans ce contexte

        handle_instruction(cpu, &instr, src, dest);

        free(src);
        free(instr.mnemonic);
    }
}


void free_cpu2(CPU2 *cpu) {
    if (!cpu) return;

    if (cpu->context) hashmap_destroy(cpu->context);
    if (cpu->constant_pool) hashmap_destroy(cpu->constant_pool);
    if (cpu->memory_handler) memory_destroy(cpu->memory_handler);

    free(cpu);
}


