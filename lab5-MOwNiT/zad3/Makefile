CC=gcc
CFLAGS=-I/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/include/
LDFLAGS=-L/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/lib -lgsl -lgslcblas -lm
OBJ=demo_fn.o root_finding.o

all: root_finding

root_finding:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	./root_finding newton
	./root_finding simplified_newton
	./root_finding steffensen

clean:
	rm *.o root_finding

