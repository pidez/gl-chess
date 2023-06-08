CC = g++

FLAGS = -DDEBUG

LIBS += -lglut
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lassimp

OBJS = chess.o program.o shader.o camera.o mesh.o material.o scene.o

#g++ main.o -lglut -lGLEW -LGL -o main.exe

# -o rinomina output file
# -c compila senza linker
# -g produce informazioni di debug
# -D[...] definisce la macro [...]
# -l[...] cerca la libreria [...] durante linking

# $@ nome del target
# $? prerequisiti piu nuovi del target
# $^ tutti i prerequisiti

chess.exe : $(OBJS)
	$(CC) $(FLAGS) $^ $(LIBS) -o $@

chess.o : chess.cpp
	$(CC) $(FLAGS) -c $? -o $@

program.o : MyClasses/Program.cpp
	$(CC) $(FLAGS) -c $? -o $@

shader.o : MyClasses/Shader.cpp
	$(CC) $(FLAGS) -c $? -o $@

camera.o : MyClasses/Camera.cpp
	$(CC) $(FLAGS) -c $? -o $@

mesh.o : MyClasses/Mesh.cpp
	$(CC) $(FLAGS) -c $? -o $@

material.o : MyClasses/Material.cpp
	$(CC) $(FLAGS) -c $? -o $@

scene.o : MyClasses/Scene.cpp
	$(CC) $(FLAGS) -c $? -o $@

.PHONY clean:
clean:
	rm *.o *.exe



