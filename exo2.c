//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo.h"


//Q2.1 - Initialisation du gestionnaire de mémoire
MemoryHandler* memory_init(int size) {
    MemoryHandler* handler = malloc(sizeof(MemoryHandler));
    if (!handler) return NULL;
    
    // Allocation de la mémoire simulée (tableau de pointeurs)
    handler->memory = calloc(size, sizeof(void*));
    handler->total_size = size;
    
    // Création de la table de hachage pour les segments alloués   
    handler->allocated = hashmap_create();
    
    // Création d'un segment libre initial couvrant toute la mémoire
    handler->free_list = malloc(sizeof(Segment));
    
    // Vérification d’échec d’allocation
    if (!handler->memory || !handler->allocated || !handler->free_list) {
        free(handler->memory);
        free(handler->allocated);
        free(handler);
        return NULL;
    }

    handler->free_list->start = 0;
    handler->free_list->size = size;
    handler->free_list->next = NULL;
    return handler;
}

// Q2.2 - Recherche d’un segment libre contenant une certaine plage mémoire
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) {
    Segment *current = handler->free_list, *previous = NULL;
    while (current) {
        if (current->start <= start && (current->start + current->size) >= (start + size)) {
            if (prev) *prev = previous;
            return current;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

// Q2.3 - Allocation d’un segment nommé à une position donnée
int create_segment(MemoryHandler *handler, const char *name, int start, int size) {
    Segment *prev = NULL;
    Segment *free_seg = find_free_segment(handler, start, size, &prev);
    if (!free_seg) return 0;

    Segment *new_seg = malloc(sizeof(Segment));
    if (!new_seg) {
        return 0;  // Aucun segment libre disponible
    }

    new_seg->start = start;
    new_seg->size = size;
    new_seg->next = NULL;
    
    // Insère le segment alloué dans la table de hachage des segments alloués
    if (!hashmap_insert(handler->allocated, name, new_seg)) {
        free(new_seg);
        return 0;
    }
    
    // Mise à jour de la liste free_list : on coupe le segment libre utilisé
    if (free_seg->start == start && free_seg->size == size) {
        // On retire le segment libre
        if (prev) prev->next = free_seg->next;
        else handler->free_list = free_seg->next;
        free(free_seg);
    } else {
        // On réduit le segment libre
        free_seg->start += size;
        free_seg->size -= size;
    }

    return 1;
}


// Q2.4 - Libération d’un segment nommé et fusion avec les segments libres adjacents
int remove_segment(MemoryHandler *handler, const char *name) {
    Segment *seg = hashmap_get(handler->allocated, name);
    if (!seg) return 0;

    // Supprimer de la table de hachage
    hashmap_remove(handler->allocated, name);
    
    // Réinsérer le segment dans la liste free_list triée
    Segment *current = handler->free_list, *prev = NULL;
    while (current && current->start < seg->start) {
        prev = current;
        current = current->next;
    }
    
    // Insère seg entre prev et current
    seg->next = current;
    if (prev) prev->next = seg;
    else handler->free_list = seg;

    // Fusion avec le segment précédent si adjacent
    if (prev && (prev->start + prev->size == seg->start)) {
        prev->size += seg->size;
        prev->next = seg->next;
        free(seg);
        seg = prev;
    }
    // Fusion avec le segment suivant si adjacent
    if (seg->next && (seg->start + seg->size == seg->next->start)) {
        Segment *temp = seg->next;
        seg->size += temp->size;
        seg->next = temp->next;
        free(temp);
    }
    return 1;
}
// PLUS Libération complète de la mémoire du gestionnaire
void memory_destroy(MemoryHandler *handler) {
    hashmap_destroy(handler->allocated);// Libère les segments alloués
    free(handler->memory);// Libère la mémoire simulée

    // Libère tous les segments libres
    Segment *current = handler->free_list;
    while (current) {
        Segment *next = current->next;
        free(current);
        current = next;
    }
    free(handler);// Libère la structure principale
}

