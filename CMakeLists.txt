cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "-std=c++17 -lglfw -lGL -lGLU -ldl -lm -lboost_thread -lnoise -pthread")

project(Minecraft)

add_executable(
    Minecraft

    ./src/Main.cc

    ./src/game/Player.cc
    ./src/game/Sphere.cc
    ./src/game/world/Chunk.cc
    ./src/game/world/CubeTypes.cc
    ./src/game/world/cube/Cube.cc
    ./src/game/world/World.cc
    ./src/game/world/World.hpp


    ./src/game/world/cube/Cube.hpp
	./src/game/world/cube/Cubes.hpp
	./src/game/world/cube/Cube.cc

	./src/game/world/cube/Air.hpp
	./src/game/world/cube/Dirt.hpp
	./src/game/world/cube/GrassyDirt.hpp
	./src/game/world/cube/Stone.hpp
	./src/game/world/cube/Lava.hpp
	./src/game/world/cube/Water.hpp
	./src/game/world/cube/Sand.hpp

    ./src/game/world/CubeTypes.hpp
    ./src/game/world/Chunk.hpp
    ./src/game/Player.hpp
    ./src/game/Hitbox.hpp
    ./src/game/Hitbox.cc
    ./src/game/Sphere.h

    ./src/engine/keyboard/KeyboardInput.cc
    ./src/engine/keyboard/KeyHandler.cc
    ./src/engine/config/ConfigLoader.cc
    ./src/engine/glad/glad.cc
    ./src/engine/loadShader.cc
    ./src/engine/mouse/Camera.cc
    ./src/engine/mouse/MouseInput.cc
    ./src/engine/LoadTexture.cc

    ./src/engine/keyboard/KeyboardInput.hpp
    ./src/engine/keyboard/KeyHandler.hpp
    ./src/engine/config/ConfigLoader.hpp
    ./src/engine/glfw.hpp
    ./src/engine/LoadTexture.hpp
    ./src/engine/mouse/MouseInput.hpp
    ./src/engine/mouse/Camera.hpp

    ./src/engine/keyboard/KeyConverter.cc
    ./src/engine/keyboard/KeyConverter.hpp

    ./src/engine/model/Model.cc
    ./src/engine/model/Model.hpp
    ./src/engine/model/ModelLoader.cc
    ./src/engine/model/ModelLoader.hpp
)

target_link_libraries (Minecraft glfw GL GLU dl m boost_thread pthread noise)
