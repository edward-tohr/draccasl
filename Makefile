#SRC_OBJS is the objects we'll be compiling.
SRC_OBJS = draccasl.cpp map.cpp gameobject.cpp
OBJS = draccasl.o map.o gameobject.o
HEADERS = draccasl.h map.h gameobject.h constants.h sdl_files.h


#OBJ_NAME is the output file name
LINUX_OBJ_NAME = draccasl
WIN_OBJ_NAME = draccasl.exe

LINUX_COMPILER_FLAGS = -Wall -Wextra --std=c++11
LINUX_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
LINUX_DEBUG_FLAGS = -g 

WIN_COMPILER_FLAGS = -Wall -Wextra --std=c++11
WIN_LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
WIN_INCLUDE_PATHS = -IE:/draccasl/draccasl/dev/include/
WIN_LIBRARY_PATHS = -LE:/draccasl/draccasl/dev/lib/
WIN_DEFINE_FLAGS = -D WIN32
LINUX_CROSS_COMPILE_FLAGS = -D __unix_cross_win__

CC = g++
WIN_CC = x86_64-w64-mingw32-g++

#And the target
default : $(OBJS)
	$(CC) $? $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)

all: $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)

win: $(OBJS)
	$(WIN_CC) $(OBJS) $(WIN_COMPILER_FLAGS) $(WIN_LINKER_FLAGS) $(WIN_INCLUDE_PATHS) $(WIN_LIBRARY_PATHS) $(WIN_DEFINE_FLAGS) -o $(WIN_OBJ_NAME)

obj: $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) -c $*.cpp

cross-win: $(OBJS)
	$(WIN_CC) $(OBJS) $(WIN_COMPILER_FLAGS) $(WIN_LINKER_FLAGS) $(WIN_INCLUDE_PATHS) $(WIN_LIBRARY_PATHS) $(LINUX_CROSS_COMPILE_FLAGS) -o $(WIN_OBJ_NAME)

draccasl.o: draccasl.cpp $(HEADERS)
	$(CC) -c draccasl.cpp

.cpp.o:
		$(CC) $(LINUX_COMPILER_FLAGS) -c *.cpp

gameobject.o: gameobject.cpp gameobject.h constants.h sdl_files.h
	$(CC) -c gameobject.cpp

map.o: map.cpp map.h constants.h sdl_files.h
	$(CC) -c map.cpp

final : $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) -O3 $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)

debug : $(OBJS)
	$(CC) $(OBJS) $(LINUX_COMPILER_FLAGS) $(LINUX_DEBUG_FLAGS) $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)

clean: 
	-rm *.o *.exe draccasl
