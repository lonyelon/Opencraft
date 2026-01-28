cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

project(Opencraft)

file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
    Opencraft

    src/Main.cpp
    src/game/Player.cpp
    src/game/Sphere.cpp
    src/game/world/Chunk.cpp
    src/game/world/Chunk.hpp
    src/game/world/cube/Cube.cpp
    src/game/world/World.cpp
    src/game/world/World.hpp
    src/game/world/WorldGenerator.hpp
    src/game/world/WorldGenerator.cpp
    src/game/world/cube/Cube.hpp
    src/game/world/cube/Cubes.hpp
    src/game/world/cube/Cube.cpp
    src/game/world/cube/Air.hpp
    src/game/world/cube/Dirt.hpp
    src/game/world/cube/GrassyDirt.hpp
    src/game/world/cube/Stone.hpp
    src/game/world/cube/Lava.hpp
    src/game/world/cube/Water.hpp
    src/game/world/cube/Sand.hpp
    src/game/world/cube/Grass.hpp
    src/game/world/cube/CubeTypes.hpp
    src/game/world/cube/CubeTypes.cpp
    src/game/Player.hpp
    src/game/Hitbox.hpp
    src/game/Hitbox.cpp
    src/game/Sphere.h
    src/engine/keyboard/KeyboardInput.cpp
    src/engine/keyboard/KeyHandler.cpp
    src/engine/config/ConfigLoader.cpp
    lib/src/glad/glad.cpp
    src/engine/loadShader.cpp
    src/engine/mouse/Camera.cpp
    src/engine/mouse/MouseInput.cpp
    src/engine/LoadTexture.cpp
    src/engine/keyboard/KeyboardInput.hpp
    src/engine/keyboard/KeyHandler.hpp
    src/engine/config/ConfigLoader.hpp
    src/engine/Engine.hpp
    src/engine/LoadTexture.hpp
    src/engine/mouse/MouseInput.hpp
    src/engine/mouse/Camera.hpp
    src/engine/keyboard/KeyConverter.cpp
    src/engine/keyboard/KeyConverter.hpp
    src/engine/model/Model.cpp
    src/engine/model/Model.hpp
    src/engine/model/ModelLoader.cpp
    src/engine/model/ModelLoader.hpp
    src/engine/position/Position.hpp
    src/game/world/cube/GrassyDirt.cpp
    src/game/world/strcuture/TreeGenerator.cpp
    src/game/world/strcuture/TreeGenerator.hpp
    src/game/world/cube/Wood.cpp
    src/game/world/cube/Wood.hpp
    src/game/world/strcuture/StructureGenerator.cpp
    src/game/world/strcuture/StructureGenerator.hpp
    src/game/world/cube/TreeLeaves.cpp
    src/game/world/cube/TreeLeaves.hpp
    src/game/Game.cpp
    src/game/Game.hpp
    src/game/world/WorldObject.hpp
)
target_include_directories(Opencraft PUBLIC src PRIVATE src)
target_include_directories(Opencraft PUBLIC lib/include PRIVATE lib/include)

target_link_libraries(Opencraft glfw GL GLU dl m pthread noise-static lz4)
