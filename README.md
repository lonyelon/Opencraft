# Opencraft

Opencraft is a 3D sandbox game based on Minecraft. The game is fully written on
C++.

## Build and installation

To build the game you first need the following libraries:
* GLFW
* GLM
* Boost libraries
* Libnoise

Next, you can clone this repository with:
```bash
git clone https://github.com/lonyelon/Opencraft
```

The enter the folder and compile the game:
```bash
cmake .
make
chmod +x ./Opencraft
```

Lastly, run the game with:
```
./Opencraft
```

### Ideas for Alpha

- [x] **First version**
  - [x] Blocks
  - [x] Camera
  - [x] Chunks

- [x] **Extra additions**
  - [x] Negative coordinates

- [x] **Gameplay**
  - [x] Remove blocks
  - [x] Place blocks
  - [x] Player camera
  - [x] Floating camera

- [ ] **Graphics**
  - [ ] Textures *Work in progress*
  - [ ] Lighting
  - [x] Skybox

- [ ] **World gen**
  - [x] Stone
  - [x] Sand
  - [ ] Infinite world
  - [ ] Trees
  - [ ] Grass
  - [ ] Ores

- [ ] **UI**
  - [ ] Game menu
  - [ ] World saving
  - [ ] Creative Inventory  
