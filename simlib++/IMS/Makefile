CPP=g++

all: ims-letiste.o
	$(CPP) -o ims-letiste ims-letiste.o -lsimlib -lm
	rm -f *.o
clean:
	rm -f ims-letiste

run: 
	./ims-letiste
	

test:
	./ims-letiste
	cat stats.out

