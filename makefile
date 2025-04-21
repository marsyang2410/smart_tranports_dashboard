# Makefile
CC       = g++
CFLAGS   = -O2 -std=c++11 -Wno-deprecated-declarations
LDFLAGS  = -lcurl

SRC      = main.cpp auth.cpp http.cpp routes.cpp cJSON.c
OBJ      = $(SRC:.cpp=.o)
OBJ     := $(OBJ:.c=.o)

all: BusTimer

BusTimer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o BusTimer
