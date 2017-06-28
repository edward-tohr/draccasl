#OBJS is the objects we'll be compiling.
OBJS = draccasl.cpp map.cpp gameobject.cpp

#OBJ_NAME is the output file name
OBJ_NAME = draccasl

COMPILER_FLAGS = -Wall -Wextra --std=c++11
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
CC = g++

#And the target
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

final : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -O3 $(LINKER_FLAGS) -o $(OBJ_NAME)
