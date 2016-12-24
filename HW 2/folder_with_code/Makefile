CC=gcc
CFLAGS=-Wall -g -pedantic -c -fcilkplus 
#CFLAGS= -Wall -g -c
LDFLAGS=-lcilkrts

OUTPUT=glife
SRC=config.c game.c main.c
OBJ=$(patsubst %.c,%.o,$(SRC))

$(OUTPUT): $(OBJ)
	$(CC) -o $(OUTPUT) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(OUTPUT) $(OBJ)
