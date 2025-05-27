CC       = g++
CFLAGS   = -O2 -std=c++17 -I/opt/homebrew/Cellar/sfml/3.0.1/include
LDFLAGS  = -lcurl -lsfml-graphics -lsfml-window -lsfml-system
LDLIBS   = -L/opt/homebrew/Cellar/sfml/3.0.1/lib


# Add SFML include path
SFML_INCLUDE = /opt/homebrew/Cellar/sfml/3.0.1/include
SFML_LIB     = /opt/homebrew/Cellar/sfml/3.0.1/lib

SRC      = main.cpp auth.cpp http.cpp routes.cpp cJSON.c
OBJ      = $(SRC:.cpp=.o)
OBJ     := $(OBJ:.c=.o)

# Target
all: BusTimer

# BusTimer executable
BusTimer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

# Compilation rules for .cpp files
%.o: %.cpp
	$(CC) $(CFLAGS) -I$(SFML_INCLUDE) -c $<

# Compilation rules for .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f *.o BusTimer
