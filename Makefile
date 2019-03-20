./build/a.out: ./Chunk.o ./Cube.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o 
	g++ ./Chunk.o ./Cube.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o -lglfw -lGL -lm -ldl -lnoise -o ./build/a.out

./Chunk.o: ./src/game/Chunk.cc 
	g++ -c  ./src/game/Chunk.cc -lglfw -lGL -lm -ldl -lnoise

./Cube.o: ./src/game/Cube.cc 
	g++ -c  ./src/game/Cube.cc -lglfw -lGL -lm -ldl -lnoise

./Main.o: ./src/Main.cc 
	g++ -c  ./src/Main.cc -lglfw -lGL -lm -ldl -lnoise

./KeyboardInput.o: ./src/engine/keyboard/KeyboardInput.cc 
	g++ -c  ./src/engine/keyboard/KeyboardInput.cc -lglfw -lGL -lm -ldl -lnoise

./KeyHandler.o: ./src/engine/keyboard/KeyHandler.cc 
	g++ -c  ./src/engine/keyboard/KeyHandler.cc -lglfw -lGL -lm -ldl -lnoise

./glad.o: ./src/engine/glad/glad.cc 
	g++ -c  ./src/engine/glad/glad.cc -lglfw -lGL -lm -ldl -lnoise

./loadShader.o: ./src/engine/loadShader.cc 
	g++ -c  ./src/engine/loadShader.cc -lglfw -lGL -lm -ldl -lnoise

clear: 
	rm *.o
