SOURCES=wav.c
EXECUTALBE=wavgen

all:
	gcc $(SOURCES) -lm -o $(EXECUTALBE)

clean:
	rm -f $(EXECUTALBE)