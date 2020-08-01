# Opencraft

Opencraft is a 3D open source sandbox game based on Minecraft. The game is fully written on C++.

## Compatibility
The game currently **runs on ArchLinux and Ubuntu** without problems. A port to other Linux distros is being worked on. The Windows version will have to wait, since threading works differently on Windows.

## Build and installation

To build the game you first need the following libraries:
* GLFW3
* GLM
* Libnoise
* GLU

### Install dependencies

In **Ubuntu** you can install the needed libraries with:
```bash
sudo apt install cmake make g++ git # Needed packages
sudo apt install libglfw3-dev libglm-dev libnoise-dev libglu1-mesa-dev # Needed libraries 
sudo ln -s /usr/include/libnoise /usr/include/noise # Link noise lib
```

In **Archlinux** you can run: 
```bash
sudo pacman -S base-devel cmake git
sudo pacman glfw-x11 # Change for glfw-wayland if you use gnome.
sudo pacman glu libnoise glm
```

### Download and install the game (distro independent)
You can download the latest version with:
```bash
git clone https://github.com/lonyelon/Opencraft
```

The enter the folder and compile the game:
```bash
cd Opencraft
cmake .
make
chmod +x ./Opencraft
```

Lastly, run the game with:
```
./Opencraft
```
