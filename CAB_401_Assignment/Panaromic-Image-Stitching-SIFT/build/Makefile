
CC = clang++

INC = ../hdr

CFLAGS = -Wall -Wextra
CFLAGS += -D_cplusplus
CFLAGS += -I$(INC)

LIBDIR = ../lib/
BINDIR = ../bin/

SRC = ../src/main.cpp

OBJ = $(SRC:.cpp=.o)
LIB = $(LIBDIR)/libifs.a

BIN = ../bin/sample.out

all: $(BIN)

$(BIN): $(LIB) $(OBJ)
	$(CC) $(CFLAGS)  -o $(BIN) *.o $(LIB) 
	rm -f *.o

.cpp.o:
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ)
	rm -f $(BIN)