# Opencraft

Opencraft is a 3D sandbox game based on Minecraft. The game is fully written on
C++.

## Build and installation

To build the game you first need the following libraries:
* GLFW3
* GLM
* Boost libraries
* Libnoise

Next, you can clone this repository with:
```bash
git clone https://github.com/lonyelon/Opencraft
```

The enter the folder and compile the game (both "worldname" and "seed" are mandatory):
```bash
cmake .
make
chmod +x ./Opencraft [worldname] [seed]
```

Lastly, run the game with (both arguments are mandatory):
```
./Opencraft [worldname] [seed]
```
