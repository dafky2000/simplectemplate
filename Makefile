TARGET = simplectemplate
LDLIBS = -lm
CC ?= gcc # Use gcc if CC is undefined
CFLAGS = -Wall -fPIC

.PHONY: default all test clean

default: $(TARGET)
all: default

OBJECTS = main.o $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))
HEADERS = main.c $(wildcard src/*.h)

test: CFLAGS += -g --coverage -DUNITTESTS
test: SPECK_CFLAGS += -coverage -DUNITTESTS

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
	valgrind --leak-check=full --error-exitcode=1 --errors-for-leak-kinds=all $(SPECK)
	rm *.gc*

clean:
	-rm -f *.o      # Build files
	-rm -f */*.o    # Build files
	-rm -f *.so     # Build files
	-rm -f */*.so   # Build files
	-rm -f *.gc*    # gcov (for cpp-coveralls)
	-rm -f */*.gc*  # gcov (for cpp-coveralls)
	-rm -f vgcore.* # Valgrind
	-rm -f $(TARGET)
	-cd speck && make clean
