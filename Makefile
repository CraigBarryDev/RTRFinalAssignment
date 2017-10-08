CC:=g++
LIBS:=-lm -lGL -lGLU -lglut
BIN:=app
FLAGS:=-std=c++11

OBJECTS:= cylinder.o framework/camera.o framework/frameTimer.o framework/glGraphics.o framework/keyboard.o framework/loader.o framework/mouse.o framework/shaders.o init.o inputHandler.o main.o shaders/litShader.o

all: $(BIN)
	./$(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(FLAGS) -o $(BIN) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CC) $(FLAGS) $(LIBS) -c $< -o $@

remake:
	make clean
	make all

clean:
	rm -f $(OBJECTS) $(BIN)