#OBJS is the objects we'll be compiling.
OBJS = draccasl.cpp map.cpp gameobject.cpp

#OBJ_NAME is the output file name
LINUX_OBJ_NAME = draccasl
WIN_OBJ_NAME = draccasl.exe

LINUX_COMPILER_FLAGS = -Wall -Wextra --std=c++11
LINUX_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
WIN_COMPILER_FLAGS = -Wall -Wextra --std=c++11
WIN_LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
WIN_INCLUDE_PATHS = -IE:/draccasl/draccasl/dev/include/
WIN_LIBRARY_PATHS = -LE:/draccasl/draccasl/dev/lib/
CC = g++

#And the target
all : $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)

win: $(OBJS)
	$(CC) $(OBJS) $(WIN_COMPILER_FLAGS) $(WIN_LINKER_FLAGS) $(WIN_INCLUDE_PATHS) $(WIN_LIBRARY_PATHS) -o $(WIN_OBJ_NAME)

final : $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) -O3 $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)
