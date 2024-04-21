CC=gcc-13
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=./fprint

.PHONY: all
all: $(EXECUTABLE)
	$(EXECUTABLE) $(EFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ -Wall -Wextra -std=c99 -pedantic $<

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)