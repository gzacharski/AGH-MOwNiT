CC=gcc
CFLAGS=-I/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/include/
LDFLAGS=-L/afs/wszib.edu.pl/usr/k/z/kzajac/pub/gsl/lib -lgsl -lgslcblas -lm
OBJ=demo_fn.o root_finding.o

all: root_finding chart

root_finding:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

chart: script_fun1.txt
	gnuplot script_fun1.txt

clean:
	rm *.o root_finding fun1.jpg

