CC ?= ${CC}
CFLAGS ?= -O2 -pipe
LDFLAGS ?=

all: sampleapp

sampleapp: sampleapp.c
	$(CC) $(CFLAGS) sampleapp.c -o sampleapp $(LDFLAGS)

clean:
	rm -f sampleapp *.o
