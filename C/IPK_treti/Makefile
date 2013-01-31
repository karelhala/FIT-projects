CPP=g++

all: base64.o rdtserver.o rdtclient.o
	$(CPP) -o rdtserver rdtserver.o base64.o
	$(CPP) -o rdtclient rdtclient.o base64.o
	rm -f *.o *.core
clean:
	rm -f *.o *.core rdtserver rdtclient
