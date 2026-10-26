# Réalisation d'un simulateur de CPU en C

## Présentation

Ce projet pédagogique consiste à réaliser progressivement un simulateur simplifié de processeur en langage C. Organisé en huit exercices, il met en pratique plusieurs structures de données et présente les principaux mécanismes nécessaires à l'exécution d'un programme : gestion de la mémoire, registres, analyse d'un pseudo-assembleur, modes d'adressage, exécution d'instructions et gestion d'une pile.

Le projet est conçu comme une progression : les premières étapes construisent les structures de base, puis les exercices suivants les réutilisent pour modéliser un CPU de plus en plus complet.

> Le simulateur est un outil pédagogique. Il ne constitue pas une émulation complète d'un processeur x86 et ne prend en charge qu'un sous-ensemble limité d'instructions.

## Objectifs du projet

Le projet vise à :

- implémenter une table de hachage pour stocker des associations clé-valeur ;
- représenter une mémoire simulée organisée en segments nommés ;
- allouer, libérer et retrouver des segments mémoire ;
- analyser un fichier de pseudo-assembleur composé de sections `.DATA` et `.CODE` ;
- représenter les registres et l'état du processeur ;
- résoudre différents modes d'adressage ;
- charger et exécuter des instructions de manière séquentielle ;
- implémenter les opérations d'empilement et de dépilement ;
- étudier l'allocation dynamique d'un segment supplémentaire et l'accès mémoire segmenté.

## Organisation et méthodes étudiées

Le travail est divisé en huit exercices indépendants ou progressifs :

1. création d'une table de hachage ;
2. gestion de la mémoire et des segments ;
3. analyse d'un fichier de pseudo-assembleur ;
4. initialisation du CPU et allocation des données ;
5. résolution des modes d'adressage et instruction `MOV` ;
6. chargement et exécution d'un programme ;
7. gestion d'une pile avec `PUSH` et `POP` ;
8. allocation/libération dynamique du segment `ES` et adressage segmenté.

Chaque exercice possède un programme de démonstration (`main.c`, `main2.c`, etc.) permettant d'observer le fonctionnement des mécanismes développés.

## Fonctionnalités détaillées par exercice

### Exercice 1 — Table de hachage

**But :** créer une structure de données permettant d'associer une clé à une valeur. Cette table est réutilisée dans les exercices suivants pour représenter notamment les registres, les étiquettes et les emplacements mémoire.

**Fonctions principales :**

- `simple_hash(str)` : calcule l'indice de hachage d'une chaîne ;
- `hashmap_create()` : crée et initialise une table de hachage ;
- `hashmap_insert(map, key, value)` : ajoute une association clé-valeur ;
- `hashmap_get(map, key)` : recherche la valeur associée à une clé ;
- `hashmap_remove(map, key)` : supprime une association ;
- `hashmap_destroy(map)` : libère les ressources de la table.

Le programme `main.c` illustre la création de la table, l'insertion d'une valeur, sa récupération, la suppression de la clé et la destruction de la structure.

### Exercice 2 — Gestion de la mémoire

**But :** représenter une mémoire simulée et la découper en segments nommés. Une liste chaînée conserve les zones libres et une table de hachage référence les segments alloués.

**Fonctions principales :**

- `memory_init(size)` : initialise une mémoire de taille donnée ;
- `find_free_segment(handler, start, size, prev)` : recherche une zone libre couvrant une plage demandée ;
- `create_segment(handler, name, start, size)` : crée un segment à une adresse et d'une taille données ;
- `remove_segment(handler, name)` : libère un segment et fusionne les zones libres adjacentes ;
- `memory_destroy(handler)` : libère le gestionnaire mémoire.

Le programme `main2.c` initialise une mémoire de 1 024 unités, crée le segment `data`, le supprime, puis libère le gestionnaire.

### Exercice 3 — Analyse du pseudo-assembleur

**But :** lire un fichier texte et séparer ses instructions de données et de code. Le parseur conserve également les étiquettes et les emplacements associés aux variables.

Le format pédagogique utilise notamment les sections `.DATA` et `.CODE`.

**Fonctions principales :**

- `parse_data_instruction(line, memory_locations)` : analyse une déclaration de donnée ;
- `parse_code_instruction(line, labels, code_count)` : analyse une ligne de code et traite les étiquettes ;
- `parse(filename)` : analyse le fichier et rassemble les résultats dans une structure `ParserResult` ;
- `free_parser_result(result)` : libère les structures créées par le parseur.

Le programme `main3.c` crée un fichier `example.asm`, lance son analyse et affiche les instructions reconnues, les étiquettes et les emplacements mémoire.

### Exercice 4 — Initialisation du CPU et gestion des données

**But :** réunir le gestionnaire mémoire et les registres dans une structure représentant le processeur, puis stocker les variables déclarées dans le segment de données `DS`.

**Fonctions principales :**

- `cpu_init(memory_size)` : initialise le CPU, sa mémoire et ses registres ;
- `store(handler, segment_name, pos, data)` : stocke une valeur à une position d'un segment ;
- `load(handler, segment_name, pos)` : récupère une valeur depuis un segment ;
- `allocate_variables(cpu, data_instructions, data_count)` : crée le segment de données et y place les valeurs ;
- `print_data_segment(cpu)` : affiche le contenu du segment `DS` ;
- `cpu_distroy(cpu)` : libère les ressources associées au CPU.

Le programme `main4.c` crée un CPU, alloue des variables dans le segment de données et affiche les valeurs mémorisées.

### Exercice 5 — Registres et modes d'adressage

**But :** représenter les registres du processeur et résoudre plusieurs formes d'opérandes. L'exercice introduit également l'instruction `MOV`, qui copie une valeur vers une destination.

**Modes d'adressage étudiés :**

- **Immédiat :** une constante, par exemple `42` ;
- **Par registre :** le contenu d'un registre, par exemple `AX` ;
- **Mémoire directe :** une adresse explicite, par exemple `[5]` ;
- **Indirect par registre :** une adresse contenue dans un registre, par exemple `[BX]`.

**Fonctions principales :**

- `cpu_init_2(memory_size)` : initialise les registres, la mémoire et le pool de constantes ;
- `immediate_addressing(cpu, operand)` : résout une constante immédiate ;
- `register_addressing(cpu, operand)` : accède à un registre ;
- `memory_direct_addressing(cpu, operand)` : accède à une adresse du segment de données ;
- `register_indirect_addressing(cpu, operand)` : utilise un registre comme adresse mémoire ;
- `resolve_addressing(cpu, operand)` : tente de résoudre l'opérande selon les modes pris en charge ;
- `handle_MOV(cpu, src, dest)` : copie la valeur de la source vers la destination.

Le programme `main5.c` initialise un environnement de test, affiche les valeurs résolues pour chaque mode d'adressage et teste une copie avec `MOV AX, 42`.

### Exercice 6 — Chargement et exécution des instructions

**But :** construire une boucle d'exécution simplifiée. Les instructions sont placées dans le segment de code `CS`, récupérées à l'aide du registre `IP`, puis exécutées en fonction de leur mnémonique.

**Fonctions principales :**

- `resolve_constants(result)` : remplace les références prises en charge par les valeurs correspondantes ;
- `cpu_inite(memory_size)` : initialise le CPU et ses registres, notamment `IP`, `ZF` et `SF` ;
- `allocate_code_segment(cpu, code_instructions, code_count)` : crée le segment `CS` et y place les instructions ;
- `handle_instruction(cpu, instr, src, dest)` : applique le comportement prévu pour l'instruction ;
- `execute_instruction(cpu, instr)` : résout les opérandes puis exécute une instruction ;
- `fetch_next_instruction(cpu)` : récupère l'instruction pointée par `IP` et avance le compteur ;
- `run_program(cpu)` : exécute le programme instruction par instruction.

Dans l'implémentation actuelle, le moteur traite `MOV`, `JMP`, `JZ`, `JNZ` et `HALT`. L'exécution interactive permet d'appuyer sur **Entrée** pour continuer ou de saisir `q` pour quitter.

Le programme `main6.c` met en scène un petit programme de démonstration et affiche l'état du CPU au cours de son exécution.

### Exercice 7 — Gestion de la pile

**But :** utiliser un segment dédié `SS` et les registres `SP` et `BP` pour représenter une pile. Les opérations permettent d'empiler et de dépiler des valeurs, ainsi que de détecter les tentatives de dépassement ou de dépilement d'une pile vide.

**Fonctions principales :**

- `cpu_init_stack(memory_size)` : initialise le CPU et le segment de pile ;
- `push_value(cpu, value)` : empile une valeur ;
- `pop_value(cpu, dest)` : dépile une valeur dans la destination fournie ;
- `handle_instruction_2(cpu, instruction, operand)` : traite les opérations `PUSH` et `POP` dans le contexte prévu par l'exercice.

Le programme `main7.c` empile plusieurs valeurs, affiche le contenu de la pile, les dépile dans l'ordre prévu et tente un dépilement supplémentaire afin d'illustrer le cas d'une pile vide.

### Exercice 8 — Allocation dynamique et adressage segmenté

**But :** étendre le modèle mémoire avec un segment supplémentaire `ES`. La taille à allouer et la stratégie de recherche sont sélectionnées à partir des registres ; le code permet ensuite d'accéder à une valeur par l'intermédiaire d'un registre de décalage.

**Fonctionnalités :**

- accès mémoire segmenté avec une notation comme `[ES:AX]` ;
- recherche d'un emplacement libre selon une stratégie de placement ;
- allocation du segment `ES` avec `ALLOC` ;
- libération du segment avec `FREE` ;
- mise à jour du registre `ES` et du drapeau `ZF` selon le résultat de l'allocation.

**Fonctions principales :**

- `setup_memory_and_registers(cpu, memory_size)` : initialise la mémoire et les registres nécessaires ;
- `segment_override_addressing(cpu, operand)` : résout une adresse de la forme `[SEGMENT:REGISTRE]` ;
- `resolve_addressing_2(cpu, operand)` : traite l'adressage segmenté dans cette extension ;
- `find_free_address_strategy(handler, size, strategy)` : recherche une zone libre selon la stratégie choisie ;
- `alloc_es_segment(cpu)` : alloue le segment `ES` ;
- `free_es_segment(cpu)` : libère le segment `ES` ;
- `handle_instruction_extended(cpu, instruction, operand)` : ajoute la prise en charge de `ALLOC` et `FREE` dans cette extension.

Les stratégies prévues par `find_free_address_strategy` sont : `0` pour **First Fit**, `1` pour **Best Fit** et `2` pour **Worst Fit**. Le programme `main8.c` teste l'allocation du segment `ES`, l'accès à une valeur par `[ES:AX]` et la libération du segment.

## Compilation et exécution

### Prérequis

- un système Linux ou un environnement disposant d'un compilateur C ;
- GCC ;
- Make.

Sur Fedora :

```bash
sudo dnf install gcc make
```

Sur Debian ou Ubuntu :

```bash
sudo apt install build-essential
```

### Se placer dans le dossier du projet

Lancez les commandes ci-dessous depuis la racine du dépôt, dans le répertoire qui contient le fichier `Makefile`.

### Exécuter un exercice

Chaque exercice possède son propre programme de démonstration. Les exercices 2 à 6 réutilisent le nom d'exécutable `exo` : exécutez donc `make clean` avant de passer d'un exercice à l'autre.

| Exercice | Commandes | Démonstration |
|---|---|---|
| 1 | `make clean && make all && ./exo` | Table de hachage : insertion, recherche et suppression |
| 2 | `make clean && make main2 && ./exo` | Initialisation de la mémoire et gestion d'un segment |
| 3 | `make clean && make main3 && ./exo` | Analyse du pseudo-assembleur et affichage des étiquettes |
| 4 | `make clean && make main4 && ./exo` | Initialisation du CPU et allocation des données |
| 5 | `make clean && make main5 && ./exo` | Modes d'adressage et instruction `MOV` |
| 6 | `make clean && make main6 && ./exo` | Exécution interactive d'un petit programme |
| 7 | `make clean && make main7 && ./main7` | Opérations sur la pile |
| 8 | `make clean && make main8 && ./main8` | Allocation, accès et libération du segment `ES` |

Pour supprimer les fichiers objets et les exécutables générés :

```bash
make clean
```

## Exemple de pseudo-assembleur

Le programme de démonstration du parseur utilise une syntaxe simplifiée comportant une section de données et une section de code :

```asm
.DATA
X DW 42
arr DB 20, 21, 22, 23
y DB 10

.CODE
start : MOV AX, x
loop : ADD AX, y
JMP loop
```

Cet exemple illustre la forme du langage étudié. Il ne signifie pas que toutes les instructions montrées sont prises en charge par le moteur d'exécution : notamment, `ADD` n'est pas implémentée dans `handle_instruction` à ce stade.

## Validation et limites

Les fichiers `main.c`, `main2.c` jusqu'à `main8.c` servent de programmes de démonstration pour les différentes étapes. Le dépôt ne contient pas de suite de tests unitaires automatisés distincte. Les huit cibles du `Makefile` ont été vérifiées à la compilation ; cela ne signifie toutefois pas que chaque démonstration s'exécute sans erreur.

Les principales limites de cette réalisation sont les suivantes :

- le processeur est simplifié et ne reproduit pas l'architecture complète d'un CPU réel ;
- le parseur attend un format de pseudo-assembleur restreint et, dans la version actuelle, certaines lignes de la section `.CODE` ne sont pas reconnues correctement ;
- la démonstration de l'exercice 4 (`main4.c`) présente actuellement une erreur de gestion mémoire à la fin de l'exécution (`munmap_chunk(): invalid pointer`) ;
- la démonstration de l'exercice 6 (`main6.c`) n'alloue pas correctement le segment `CS` dans son environnement de test et n'exécute donc pas le programme prévu ;
- le jeu d'instructions exécutées est limité à `MOV`, `JMP`, `JZ`, `JNZ` et `HALT` dans le moteur principal ;
- l'adressage segmenté de l'exercice 8 est une extension pédagogique et ne remplace pas un gestionnaire d'adressage complet ;
- certains modules sont testés séparément plutôt que réunis dans un simulateur unique.

## Technologies

- **Langage :** C
- **Compilation :** GCC
- **Automatisation :** Make
- **Norme de compilation indiquée dans le Makefile :** C11
- **Structures de données :** table de hachage, liste chaînée, tableaux et segments mémoire.

## Mots-clés

C, structures de données, table de hachage, gestion mémoire, segmentation mémoire, CPU, pseudo-assembleur, registres, modes d'adressage, pile, architecture des ordinateurs, GCC, Make.
