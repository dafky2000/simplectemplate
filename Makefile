TARGET = simplectemplate
LDLIBS = -lm
CC ?= gcc # Use gcc if CC is undefined
CFLAGS = -Wall -fPIC

.PHONY: default all test clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

SPECK_CFLAGS = -Isrc
SPECK_LDFLAGS =
SPECK_LIBS = $(OBJECTS) $(LDLIBS)
-include speck/speck.mk
test: $(SPECK) $(OBJECTS) $(SUITES)
	@$(SPECK)

clean:
	-rm -f src/*.o
	-rm -f spec/*.so
	-rm -f $(TARGET)
