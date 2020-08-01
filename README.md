# Opencraft

Opencraft is a 3D open source sandbox game based on Minecraft. The game is fully written on C++.

## Compatibility
The game currently **runs on ArchLinux** without problems. A port to other Linux distros is being worked on. The Windows version will have to wait, since threading works differently on Windows.

## Build and installation

To build the game you first need the following libraries:
* GLFW3
* GLM
* Libnoise
* GLU

```bash
sudo apt install cmake make g++ git libglfw3-dev libglm-dev libnoise-dev libglu1-mesa-dev
sudo ln -s /usr/include/libnoise /usr/include/noise
```

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
