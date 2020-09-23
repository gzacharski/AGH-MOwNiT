CC=gcc
CFLAGS=-I/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/include/
LDFLAGS=-L/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/lib -lgsl -lgslcblas -lm
OBJ=solver.o

all: solver charts

solver:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	./solver 10
	./solver 20
	./solver 50
	./solver 100
	./solver 200
	./solver 300
	./solver 400
	./solver 500
	./solver 600
	./solver 800
	./solver 1000

charts: script_decomp.txt script_solve.txt
	gnuplot script_decomp.txt
	gnuplot script_solve.txt

clean:
	rm *.o *.jpg solver decomp_times.txt solve_times.txt

