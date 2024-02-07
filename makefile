SOURCES=wav.c hash.c
EXECUTALBE=wavgen

all:
	gcc $(SOURCES) -lm -o $(EXECUTALBE)

clean:
	rm -f $(EXECUTALBE)