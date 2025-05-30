CXX      = g++
CC       = gcc
CXXFLAGS = -std=c++17
CFLAGS   =

CPP_SRC  = main.cpp auth.cpp http.cpp routes.cpp
C_SRC    = cJSON.c

OBJ      = $(CPP_SRC:.cpp=.o) $(C_SRC:.c=.o)

# Libraries
LDLIBS = -lcurl

# Target
all: BusTimer

# BusTimer executable
BusTimer: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

# Compilation rules for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Compilation rules for .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f *.o BusTimer
