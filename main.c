//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"

int main() {
    HashMap* map = hashmap_create();
    if (map) {
        printf("HashMap créée avec succès !\n");
        
        // Test de l'insertion
        int value1 = 42;
        if (hashmap_insert(map, "test", &value1)) {
            printf("Insertion réussie : clé = 'test', valeur = %d\n", value1);
        } else {
            printf("Échec de l'insertion.\n");
        }
        
        int* retrieved_value = (int*) hashmap_get(map, "test");
        if (retrieved_value) {
            printf("Valeur récupérée : %d\n", *retrieved_value);
        } else {
            printf("Échec de la récupération.\n");
        }
        
        if (hashmap_remove(map, "test")) {
            printf("Suppression réussie.\n");
        } else {
            printf("Échec de la suppression.\n");
        }
        
        retrieved_value = (int*) hashmap_get(map, "test");
        if (retrieved_value) {
            printf("La clé existe toujours, valeur : %d\n", *retrieved_value);
        } else {
            printf("La clé a bien été supprimée.\n");
        }
        
        hashmap_destroy(map);
        printf("HashMap détruite avec succès.\n");
        
    } else {
        printf("Erreur lors de la création de la HashMap.\n");
    }
    return 0;
}
