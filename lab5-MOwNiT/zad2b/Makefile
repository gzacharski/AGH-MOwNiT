CC=gcc
CFLAGS=-I/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/include/
LDFLAGS=-L/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/lib -lgsl -lgslcblas -lm
OBJ=demo_fn.o root_finding.o

all: root_finding chart

root_finding:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	./root_finding bisection
	./root_finding secant
	./root_finding brent

chart: script_fun2.txt
	gnuplot script_fun2.txt

clean:
	rm *.o root_finding fun2.jpg

