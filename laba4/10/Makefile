CC = gcc -g
CFLAGS = -Wall -g -I./Deikstra -I./Settings  # Include paths added
LDFLAGS = -lpthread

# Define object files using wildcard pattern
DEIKSTRA_OBJECTS = $(patsubst %.c,%.o,$(wildcard Deikstra/*.c))
SETTINGS_OBJECTS = $(patsubst %.c,%.o,$(wildcard Settings/*.c))
PREFIX_TREE_OBJECT = PrefixTree.o
MAIN_OBJECT = main.o

# Define the executable name
EXECUTABLE = main

#Compilation Rules
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link the executable
$(EXECUTABLE): $(DEIKSTRA_OBJECTS) $(SETTINGS_OBJECTS) $(PREFIX_TREE_OBJECT) $(MAIN_OBJECT)
	$(CC) $(LDFLAGS) $^ -o $@ -lm

# Clean up the object files
clean:
	rm -f Deikstra/*.o $(DEIKSTRA_OBJECTS)
	rm -f Settings/*.o $(SETTINGS_OBJECTS)
	rm -f *.o $(EXECUTABLE)