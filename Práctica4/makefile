.PHONY: x clean tar

EJECUTABLE= practica_objetos_B3

MODULOS:= practica_objetos_B3.o\
         objetos_B3.o\
         file_ply_stl.o\
      
LIBS:=-lglut -lGLU -lGL -lsupc++ -lm -lc -lstdc++ -lpthread -L/usr/lib/nvidia-331
CPPFLAGS:=-Wall -fmax-errors=2 -g

CC= g++
OPTS = -I"include" -L "/usr/lib" bin/linux-gcc-64/libIrrKlang.so -pthread

x: $(EJECUTABLE)
	./$(EJECUTABLE) beethoven perfil_ply

$(EJECUTABLE): $(MODULOS)	
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS) $(OPTS)

clean:
	rm -rf *.o $(EJECUTABLE)

