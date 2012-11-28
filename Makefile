CC = g++

SRC_PATH = src
BIN_PATH = bin
INCLUDE_PATH = include
EXTERNAL_PATH = externals

CFLAGS = -Wall -ansi -pedantic -I $(INCLUDE_PATH) -I $(EXTERNAL_PATH)/include
LDFLAGS = -lSDL -lGL

EXEC = ImacParticleSystem

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.cpp, $(SRC_PATH)/%.o, $(SRC_FILES))

all: $(BIN_PATH)/$(EXEC)

$(BIN_PATH)/$(EXEC): $(OBJ_FILES) $(EXTERNAL_PATH)/src/glew-1.9/glew.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXTERNAL_PATH)/src/glew-1.9/glew.o: $(EXTERNAL_PATH)/src/glew-1.9/glew.c
	$(CC) -c -o $@ $(CFLAGS) $^ 

$(SRC_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) -c -o $@ $(CFLAGS) $^ 

clean:
	rm $(OBJ_FILES)

cleanall:
	rm $(BIN_PATH)/$(EXEC) $(OBJ_FILES)
