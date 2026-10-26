# Simulateur de CPU en C

Projet pédagogique réalisé en **C** visant à construire progressivement un simulateur de processeur simplifié. Le projet explore les structures de données nécessaires à la représentation d'un CPU, la gestion de la mémoire, l'analyse d'un pseudo-assembleur et l'exécution de quelques instructions.

> Il s'agit d'un simulateur éducatif et partiel, pas d'une émulation complète d'un processeur x86.

## Fonctionnalités

- **Table de hachage** pour associer des clés à des valeurs, notamment pour représenter les registres, les étiquettes et les symboles.
- **Gestion de la mémoire** avec allocation et libération de segments nommés.
- **Analyse d'un pseudo-assembleur** organisé autour de sections `.DATA` et `.CODE`, avec des étiquettes et des opérandes.
- **État du processeur** représenté par des registres tels que `AX`, `BX`, `CX`, `DX`, `IP`, `ZF` et `SF`, selon les modules.
- **Modes d'adressage** : immédiat, par registre, mémoire directe et indirecte par registre.
- **Exécution pas à pas** d'un petit ensemble d'instructions dans le module d'exécution.
- **Gestion d'une pile** à l'aide des opérations `push` et `pop`.
- **Allocation dynamique du segment `ES`**, libération du segment et accès avec une forme d'adressage segmenté comme `[ES:AX]`.

## Technologies

- **Langage :** C
- **Compilation :** GCC
- **Automatisation :** Make (`Makefile`)
- **Standard de compilation :** C11

## Prérequis

Sur Linux, installer un compilateur C et `make`. Par exemple, sur Fedora :

```bash
sudo dnf install gcc make
```

Sur Debian ou Ubuntu :

```bash
sudo apt install build-essential
```

## Compilation et exécution

Placez-vous dans le dossier du projet :

```bash
cd ErmalZeqo-Realisation-d-un-simulateur-de-CPU-main
```

### Exemple 1 — Table de hachage

La cible par défaut compile le programme de démonstration de la table de hachage :

```bash
make clean
make all
./exo
```

### Exécuter un exercice précis

Les exercices 2 à 6 réutilisent le nom d'exécutable `exo`. Lancez `make clean` avant de changer d'exercice afin de forcer la recompilation avec le bon fichier `main`.

| Cible Make | Commandes | Démonstration |
|---|---|---|
| `main2` | `make clean && make main2 && ./exo` | Initialisation de la mémoire, création et suppression d'un segment |
| `main3` | `make clean && make main3 && ./exo` | Analyse du fichier pseudo-assembleur `example.asm`, sections `.DATA` et `.CODE`, étiquettes et symboles |
| `main4` | `make clean && make main4 && ./exo` | Initialisation du CPU et allocation de variables dans le segment de données |
| `main5` | `make clean && make main5 && ./exo` | Tests de différents modes d'adressage et de l'instruction `MOV` |
| `main6` | `make clean && make main6 && ./exo` | Exécution d'un petit programme d'instructions pas à pas |
| `main7` | `make clean && make main7 && ./main7` | Empilement et dépilement de valeurs |
| `main8` | `make clean && make main8 && ./main8` | Allocation/libération du segment `ES` et accès segmenté |

Pour le programme de l'exercice 6, appuyez sur **Entrée** pour continuer l'exécution instruction par instruction, ou tapez `q` pour arrêter.

Nettoyer les exécutables et fichiers objets générés :

```bash
make clean
```

## Organisation du dépôt

| Fichier | Rôle |
|---|---|
| `exo1.c` | Table de hachage |
| `exo2.c` | Gestionnaire mémoire et segments |
| `exo3.c` | Analyse des sections de données et de code |
| `exo4.c` | Initialisation du CPU et allocation des variables |
| `exo5.c` | Registres et résolution des modes d'adressage |
| `exo6.c` | Chargement du code, récupération et exécution des instructions |
| `exo7.c` | Gestion de la pile (`push` / `pop`) |
| `exo8.c` | Adressage segmenté et allocation/libération dynamique de `ES` |
| `main.c`, `main2.c` … `main8.c` | Programmes de démonstration des modules |
| `exo.h`, `exo2.h`, `exo8.h` | Structures et déclarations partagées |
| `example.asm` | Exemple de programme en pseudo-assembleur |
| `Makefile` | Règles de compilation et de nettoyage |

## Exemple de pseudo-assembleur

Le dépôt contient un exemple de syntaxe avec une section de données et une section de code :

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

Le format est pédagogique et ne correspond pas à un assembleur x86 complet.

## Limites connues

- Le projet est développé par étapes : toutes les fonctionnalités d'un processeur réel ne sont pas implémentées.
- Le jeu d'instructions pris en charge par le moteur d'exécution est limité. Certaines instructions présentes dans l'exemple assembleur, comme `ADD`, ne sont pas encore exécutées par le gestionnaire d'instructions.
- Les formats de lignes et d'opérandes attendus par le parseur sont simplifiés ; les programmes d'exemple servent avant tout à illustrer chaque module.

## Auteurs

Projet réalisé par **Ermal Zeqo** et **Fitia Sendrison**.
