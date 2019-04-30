./build/a.out: ./Player.o ./Chunk.o ./CubeTypes.o ./Cube.o ./World.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o 
	g++ ./Player.o ./Chunk.o ./CubeTypes.o ./Cube.o ./World.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o -lglfw -lGL -ldl -lm -lnoise -pthread -o ./build/a.out

./Player.o: ./src/game/Player.cc 
	g++ -c  ./src/game/Player.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./Chunk.o: ./src/game/world/Chunk.cc 
	g++ -c  ./src/game/world/Chunk.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./CubeTypes.o: ./src/game/world/CubeTypes.cc 
	g++ -c  ./src/game/world/CubeTypes.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./Cube.o: ./src/game/world/cube/Cube.cc 
	g++ -c  ./src/game/world/cube/Cube.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./World.o: ./src/game/world/World.cc 
	g++ -c  ./src/game/world/World.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./Main.o: ./src/Main.cc 
	g++ -c  ./src/Main.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./KeyboardInput.o: ./src/engine/keyboard/KeyboardInput.cc 
	g++ -c  ./src/engine/keyboard/KeyboardInput.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./KeyHandler.o: ./src/engine/keyboard/KeyHandler.cc 
	g++ -c  ./src/engine/keyboard/KeyHandler.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./glad.o: ./src/engine/glad/glad.cc 
	g++ -c  ./src/engine/glad/glad.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./loadShader.o: ./src/engine/loadShader.cc 
	g++ -c  ./src/engine/loadShader.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./Camera.o: ./src/engine/mouse/Camera.cc 
	g++ -c  ./src/engine/mouse/Camera.cc -lglfw -lGL -ldl -lm -lnoise -pthread

./MouseInput.o: ./src/engine/mouse/MouseInput.cc 
	g++ -c  ./src/engine/mouse/MouseInput.cc -lglfw -lGL -ldl -lm -lnoise -pthread

clear: 
	rm *.o
