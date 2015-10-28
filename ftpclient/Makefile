# Author: Jussi Laakkonen / 

CFLAGS = -Wall

OUTPUT = ftpclient
SOURCES = ftpclient.c genoper.c fileoper.c

all: build

build: $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $(OUTPUT)

clean: remove

debug: $(SOURCES)
	gcc -g $(SOURCES) -o $(OUTPUT)
	
remove:
	rm -v $(OUTPUT)