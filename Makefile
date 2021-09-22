CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall -lm

nbody_sim: nbody_sim.o
	$(CC) $(LDFLAGS) -o nbody_sim nbody_sim.o

nbody_sim.o: nbody_sim.c
	$(CC) $(CFLAGS) -o nbody_sim.o -c nbody_sim.c

.PHONY: clean test

clean:
	rm -rf nbody_sim.o nbody_sim *~

test: nbody_sim
	./nbody_sim

