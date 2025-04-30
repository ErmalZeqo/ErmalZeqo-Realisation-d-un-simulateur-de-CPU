//Ermal ZEQO 21315866
//Fitia SENDRISON 21318175
#include "exo.h"

int main() {
// Création d'un fichier d'exemple contenant un pseudo-assembleur
      const char *filename = "example.asm";

// Exemple de contenu du fichier (section .DATA et .CODE)
      FILE *file = fopen(filename, "w");
      if (!file) {
          printf("Domage !");
          return 1;
      }

// Contenu du fichier assembleur
      fprintf(file, ".DATA\n");
      fprintf(file, "X DW 42\n");
      fprintf(file, "arr DB 20, 21, 22, 23\n");
      fprintf(file, "y DB 10\n");
      fprintf(file, ".CODE\n");
      fprintf(file, "start : MOV AX, x\n");
      fprintf(file, "loop : ADD AX, y\n");
      fprintf(file, "JMP loop\n");

      fclose(file); 

	// Appel de la fonction parse pour analyser le fichier
      ParserResult *result = parse(filename);
      if (!result) {
        printf("Erreur lors de l'analyse du fichier !\n");
        return 1;
      }

	// Affichage des résultats de l'analyse de la section .DATA
      printf("Data Instructions:\n");
      for (int i = 0; i < result->data_count; i++) {
          printf("%s %s %s\n", result->data_instructions[i]->mnemonic,
                              result->data_instructions[i]->operand1,
                              result->data_instructions[i]->operand2);
      }

	// Affichage des résultats de l'analyse de la section .CODE
	printf("\nCode Instructions:\n");
	for (int i = 0; i < result->code_count; i++) {
    		if (result->code_instructions[i]) {
       		 printf("Mnemonic: %s, Operand1: %s, Operand2: %s\n", 
               result->code_instructions[i]->mnemonic,
               result->code_instructions[i]->operand1 ? result->code_instructions[i]->operand1 : "NULL",
               result->code_instructions[i]->operand2 ? result->code_instructions[i]->operand2 : "NULL");
    }
}

	
      // Affichage des labels (etiquettes)
	printf("\nLabels:\n");
	for (int i = 0; i < result->labels->size; i++) {
    		HashEntry entry = result->labels->table[i];
    		if (entry.key != NULL && entry.value != NULL) {
        		printf("Label: %s -> Address: %ld\n", entry.key, (long)entry.value);
    	 	}	
	}

	// Affichage des adresses mémoire des variables
      printf("\nMemory Locations:\n");
      for (int i = 0; i < result->memory_locations->size; i++) {
          HashEntry entry = result->memory_locations->table[i];
          if (entry.value != NULL) {
              printf("%s -> %ld\n", entry.key, (long)entry.value);
          }
      }

	// Libération de la mémoire allouée pour l'analyse
      free_parser_result(result);

      return 0;
}
