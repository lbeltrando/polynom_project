CC = gcc

CPPFLAGS = -I.
CFLAGS = -Wall

SRC = f2_poly.c arithm.c mainAuto.c mainRandom.c mainIrred.c mainPrim.c
OBJ= $(SRC:.c=.o)

.PHONY: clean,mrproper

.SUFFIXES:

all:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o testAuto f2_poly.c arithm.c mainAuto.c -lm
	$(CC) $(CPPFLAGS) $(CFLAGS) -o testRandom f2_poly.c arithm.c mainRandom.c -lm
	$(CC) $(CPPFLAGS) $(CFLAGS) -o irred f2_poly.c arithm.c mainIrred.c -lm
	$(CC) $(CPPFLAGS) $(CFLAGS) -o prim f2_poly.c arithm.c mainPrim.c -lm

arithm.o: arithm.h arithm.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c arithm.c

f2_poly.o: f2_poly.h f2_poly.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c f2_poly.c

mainAuto.o: f2_poly.h mainAuto.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c mainAuto.c

mainRandom.o: f2_poly.h mainRandom.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c mainRandom.c

mainIrred.o: f2_poly.h mainIrred.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c mainIrred.c

mainPrim.o: f2_poly.h mainPrim.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c mainPrim.c

clean:
	rm -rf $(OBJ)

mrproper:
	rm -rf testAuto
	rm -rf testRandom
	rm -rf irred
	rm -rf prim
