//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"

int main() {
    MemoryHandler *handler = memory_init(1024);
    if (!handler) {
        printf("Erreur lors de l'initialisation de la mémoire\n");
        return 1;
    }
    printf("Mémoire initialisée\n");
    
    create_segment(handler, "data", 200, 100);
    printf("Segment 'data' créé\n");
    
    remove_segment(handler, "data");
    printf("Segment 'data' supprimé\n");
    
    memory_destroy(handler);
    printf("Mémoire libérée\n");
    return 0;
}
