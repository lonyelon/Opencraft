./build/a.out: ./Player.o ./Sphere.o ./Chunk.o ./CubeTypes.o ./Cube.o ./World.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./ConfigLoader.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o ./LoadTexture.o 
	g++ ./Player.o ./Sphere.o ./Chunk.o ./CubeTypes.o ./Cube.o ./World.o ./Main.o ./KeyboardInput.o ./KeyHandler.o ./ConfigLoader.o ./glad.o ./loadShader.o ./Camera.o ./MouseInput.o ./LoadTexture.o -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread -o ./build/a.out

./Player.o: ./src/game/Player.cc 
	g++ -c  ./src/game/Player.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./Sphere.o: ./src/game/Sphere.cc 
	g++ -c  ./src/game/Sphere.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./Chunk.o: ./src/game/world/Chunk.cc 
	g++ -c  ./src/game/world/Chunk.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./CubeTypes.o: ./src/game/world/CubeTypes.cc 
	g++ -c  ./src/game/world/CubeTypes.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./Cube.o: ./src/game/world/cube/Cube.cc 
	g++ -c  ./src/game/world/cube/Cube.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./World.o: ./src/game/world/World.cc 
	g++ -c  ./src/game/world/World.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./Main.o: ./src/Main.cc 
	g++ -c  ./src/Main.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./KeyboardInput.o: ./src/engine/keyboard/KeyboardInput.cc 
	g++ -c  ./src/engine/keyboard/KeyboardInput.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./KeyHandler.o: ./src/engine/keyboard/KeyHandler.cc 
	g++ -c  ./src/engine/keyboard/KeyHandler.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./ConfigLoader.o: ./src/engine/config/ConfigLoader.cc 
	g++ -c  ./src/engine/config/ConfigLoader.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./glad.o: ./src/engine/glad/glad.cc 
	g++ -c  ./src/engine/glad/glad.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./loadShader.o: ./src/engine/loadShader.cc 
	g++ -c  ./src/engine/loadShader.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./Camera.o: ./src/engine/mouse/Camera.cc 
	g++ -c  ./src/engine/mouse/Camera.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./MouseInput.o: ./src/engine/mouse/MouseInput.cc 
	g++ -c  ./src/engine/mouse/MouseInput.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

./LoadTexture.o: ./src/engine/LoadTexture.cc 
	g++ -c  ./src/engine/LoadTexture.cc -lglfw -lGLU -lGL -ldl -lm -lnoise -lboost_thread -pthread

clear: 
	rm *.o
