cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(Opencraft)

file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
        Opencraft

        src/Main.cc
        src/game/Player.cc
        src/game/Sphere.cc
        src/game/world/Chunk.cc
        src/game/world/Chunk.hpp
        src/game/world/cube/Cube.cc
        src/game/world/World.cc
        src/game/world/World.hpp
        src/game/world/WorldGenerator.hpp
        src/game/world/WorldGenerator.cc
        src/game/world/cube/Cube.hpp
        src/game/world/cube/Cubes.hpp
        src/game/world/cube/Cube.cc
        src/game/world/cube/Air.hpp
        src/game/world/cube/Dirt.hpp
        src/game/world/cube/GrassyDirt.hpp
        src/game/world/cube/Stone.hpp
        src/game/world/cube/Lava.hpp
        src/game/world/cube/Water.hpp
        src/game/world/cube/Sand.hpp
        src/game/world/cube/Grass.hpp
        src/game/world/cube/CubeTypes.hpp
        src/game/Player.hpp
        src/game/Hitbox.hpp
        src/game/Hitbox.cc
        src/game/Sphere.h
        src/engine/keyboard/KeyboardInput.cc
        src/engine/keyboard/KeyHandler.cc
        src/engine/config/ConfigLoader.cc
        lib/src/glad/glad.cc
        src/engine/loadShader.cc
        src/engine/mouse/Camera.cc
        src/engine/mouse/MouseInput.cc
        src/engine/LoadTexture.cc
        src/engine/keyboard/KeyboardInput.hpp
        src/engine/keyboard/KeyHandler.hpp
        src/engine/config/ConfigLoader.hpp
        src/engine/Engine.hpp
        src/engine/LoadTexture.hpp
        src/engine/mouse/MouseInput.hpp
        src/engine/mouse/Camera.hpp
        src/engine/keyboard/KeyConverter.cc
        src/engine/keyboard/KeyConverter.hpp
        src/engine/model/Model.cc
        src/engine/model/Model.hpp
        src/engine/model/ModelLoader.cc
        src/engine/model/ModelLoader.hpp
        src/engine/position/Position.hpp
        src/game/world/cube/GrassyDirt.cc
        src/game/world/strcuture/TreeGenerator.cc
        src/game/world/strcuture/TreeGenerator.hpp
        src/game/world/cube/Wood.cc
        src/game/world/cube/Wood.hpp
        src/game/world/strcuture/StructureGenerator.cc
        src/game/world/strcuture/StructureGenerator.hpp
        src/game/world/cube/TreeLeaves.cc
        src/game/world/cube/TreeLeaves.hpp
        src/game/Game.cc
        src/game/Game.hpp
)
target_include_directories(Opencraft PUBLIC src PRIVATE src)
target_include_directories(Opencraft PUBLIC lib/include PRIVATE lib/include)

target_link_libraries(Opencraft glfw GL GLU dl m pthread noise)
