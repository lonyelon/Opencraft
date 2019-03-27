./build/a.out: ./Chunk.o ./CubeTypes.o ./Cube.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o 
	g++ ./Chunk.o ./CubeTypes.o ./Cube.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o -lglfw -lGL -ldl -lm -lnoise -lpthread -o ./build/a.out

./Chunk.o: ./src/game/Chunk.cc 
	g++ -c  ./src/game/Chunk.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./CubeTypes.o: ./src/game/CubeTypes.cc 
	g++ -c  ./src/game/CubeTypes.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./Cube.o: ./src/game/Cube.cc 
	g++ -c  ./src/game/Cube.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./Main.o: ./src/Main.cc 
	g++ -c  ./src/Main.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./KeyboardInput.o: ./src/engine/keyboard/KeyboardInput.cc 
	g++ -c  ./src/engine/keyboard/KeyboardInput.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./KeyHandler.o: ./src/engine/keyboard/KeyHandler.cc 
	g++ -c  ./src/engine/keyboard/KeyHandler.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./glad.o: ./src/engine/glad/glad.cc 
	g++ -c  ./src/engine/glad/glad.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./loadShader.o: ./src/engine/loadShader.cc 
	g++ -c  ./src/engine/loadShader.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./Camera.o: ./src/engine/mouse/Camera.cc 
	g++ -c  ./src/engine/mouse/Camera.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

./MouseInput.o: ./src/engine/mouse/MouseInput.cc 
	g++ -c  ./src/engine/mouse/MouseInput.cc -lglfw -lGL -ldl -lm -lnoise -lpthread

clear: 
	rm *.o
