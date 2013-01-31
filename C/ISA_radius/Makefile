CPP=g++

all: radauth.o
	$(CPP) -o radauth radauth.o -lcrypto
	rm -f *.o *.core
clean:
	rm -f radauth
