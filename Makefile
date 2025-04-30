# Ermal ZEQO 21315866
# Fitia SENDRISON 21318175

CFLAGS = -Wall -Wextra -std=c11 -g

# Nom des fichiers source
SRC = exo1.c exo2.c exo3.c exo4.c exo5.c exo6.c 
OBJ = $(SRC:.c=.o)

# Par défaut, on utilise main.c comme fichier principal
MAIN = main.c
EXEC = exo

# Compilation de l'exécutable
all: $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	gcc $(CFLAGS) -o $(EXEC) $(OBJ) $(MAIN)

%.o: %.c exo.h
	gcc $(CFLAGS) -c $< -o $@



main7: MAIN=main7.c
main7: $(OBJ) $(MAIN)
	gcc $(CFLAGS) -o main7 $(OBJ) main7.c exo7.c
main8: MAIN=main8.c
main8: $(OBJ) $(MAIN)
	gcc $(CFLAGS) -o main8 $(OBJ) main8.c exo8.c

main2:
	$(MAKE) MAIN=main2.c $(EXEC)

main3:
	$(MAKE) MAIN=main3.c $(EXEC)
main4:
	$(MAKE) MAIN=main4.c $(EXEC)
main5:
	$(MAKE) MAIN=main5.c $(EXEC)
main6:
	$(MAKE) MAIN=main6.c $(EXEC)


clean:
	rm -f $(OBJ) $(EXEC) main7 main8
