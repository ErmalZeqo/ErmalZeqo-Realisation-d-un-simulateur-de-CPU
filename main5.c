// Ermal ZEQO 21315866
// Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"

CPU2 *setup_test_environment() {
    // Initialiser le CPU2
    CPU2 *cpu = cpu_init_2(1024); // Utiliser cpu_init_2 pour CPU2
    if (!cpu) {
        printf("Erreur : Initialisation du CPU échouée\n");
        return NULL;
    }

    // Initialiser les registres avec des valeurs spécifiques
    int *ax = (int *)hashmap_get(cpu->context, "AX");
    int *bx = (int *)hashmap_get(cpu->context, "BX");
    int *cx = (int *)hashmap_get(cpu->context, "CX");
    int *dx = (int *)hashmap_get(cpu->context, "DX");

    if (ax) *ax = 3;
    if (bx) *bx = 6;
    if (cx) *cx = 100;
    if (dx) *dx = 0;

    // Afficher les valeurs des registres
    printf("Registres initialisés : AX = %d, BX = %d, CX = %d, DX = %d\n", *ax, *bx, *cx, *dx);
    
    // Créer et initialiser le segment de données
    if (!hashmap_get(cpu->memory_handler->allocated, "DS")) {
        create_segment(cpu->memory_handler, "DS", 0, 20);

        
        // Initialiser le segment de données avec des valeurs de test
for (int i = 0; i < 10; i++) {
    int *value = (int *)malloc(sizeof(int));
    *value = i * 10 + 5; // Valeurs 5, 15, 25, 35...
    store(cpu->memory_handler, "DS", i, (void *)value); // Stocker la valeur dans le segment
}

    }
    printf("Environnement de test initialisé.\n");
    return cpu;
}

int main() {
    CPU2 *cpu = setup_test_environment(); // Utiliser CPU2 ici
    if (!cpu) {
        return -1; 
    }

    // Test des différents modes d'adressage
    const char *immediate_operand = "42"; // Adressage immédiat
    const char *register_operand = "AX";   // Adressage par registre
    const char *direct_operand = "[5]";     // Adressage direct
    const char *indirect_operand = "[BX]";  // Adressage indirect par registre

    // Résoudre les opérandes
    void *immediate_value = resolve_addressing(cpu, immediate_operand);
    if (immediate_value == NULL) {
        printf("Erreur : Impossible de résoudre l'opérande immédiat.\n");
        cpu_destroy(cpu);
        return -1; // Gérer l'erreur
    }

    void *register_value = resolve_addressing(cpu, register_operand);
    if (register_value == NULL) {
        printf("Erreur : Impossible de résoudre l'opérande registre.\n");
        cpu_destroy(cpu);
        return -1; // Gérer l'erreur
    }

    void *direct_value = resolve_addressing(cpu, direct_operand);
    if (direct_value == NULL) {
        printf("Erreur : Impossible de résoudre l'opérande direct.\n");
        cpu_destroy(cpu);
        return -1; // Gérer l'erreur
    }

    void *indirect_value = resolve_addressing(cpu, indirect_operand);
    if (indirect_value == NULL) {
        printf("Erreur : Impossible de résoudre l'opérande indirect.\n");
        cpu_destroy(cpu);
        return -1; // Gérer l'erreur
    }

    // Afficher les valeurs résolues
    printf("Valeur immédiate : %d\n", *(int *)immediate_value);
    printf("Valeur registre AX : %d\n", *(int *)register_value);
    printf("Valeur adresse directe [5] : %d\n", *(int *)direct_value);
    printf("Valeur adresse indirecte [BX] : %d\n", *(int *)indirect_value);

    // Tester l'instruction MOV
    handle_MOV(cpu, immediate_value, register_value); // MOV AX, 42
    printf("Après MOV, valeur AX : %d\n", *(int *)register_value);

    // Exemple d'utilisation de load
    void *loaded_value = load(cpu->memory_handler, "DS", 5); // Charger la valeur à l'adresse 5 dans le segment "DS"
    if (loaded_value) {
        printf("Valeur chargée à l'adresse 5 : %d\n", *(int *)loaded_value); // Afficher la valeur chargée
    }

    // Libérer les ressources
    cpu_destroy(cpu);
    return 0;
}
