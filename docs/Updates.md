# Updates

# Dev6.0

* No height limit!
* Caves are now more common with depth.
* Faster chunk generation and loading.
* Chunks are now stored as compressed LZ4 files.
* Removed references to Minecraft in source.
* Water is now shorter than the rest of cubes.
* Player position is now saved and loaded.
* Tidy game code:
  * Cubes are now c++ classes.
  * BOOST was removed.
  * Start to use c++ pointers.
  * WorldObject is a new abstraction for classes with a position.
  * Move terrain generation to WorldGenerator.
* flake.nix!

# Dev5.1: Graphics Update 1.2-a
* Better renderer, at least x2 performance.
* Model loader.

# Dev5.0 World Gen Update 2.0-a
* Infinite world

*Bugs will now be listed on docs/Bugs.md*

# Dev4.0: Gameplay Update 1.0-a
* Placing blocks
* Removing blocks
* Player camera
* Sprint

# Dev3.2: Graphics Update 1.1-a
* Textures rework
  * New grass texture
  * Block sides
* Vignette

*Bugs Solved*
* 0x5

# Dev3.1: Graphics Update 1.0-b
* Keybindigs
* Mipmaps
* CMake

The windows port needs:

1. The DLLs for glfw
2. _The boost thread lib._ Done
3. _CMake_ Done

*Known Bugs*
* 0x6: Mipmap fails at cube borders

## Dev3.0: Graphics Update 1.0-a
* Config file
* Textures
  * Grass, Dirt, Water, Lava, Stone and Sand
* Skybox

*Bugs Solved*
* 0x4

*Known Bugs*
* 0x5: Textures seem badly placed

## Dev2.4: World Gen Update 1.2-a
* Caves
  * Lava
* x6 faster chunk rendering

## Dev2.3: World Gen Update 1.1-b
* Sand

## Dev2.2: World Gen Update 1.1-a
* Better world generation
  * Dirt
  * Stone
* Better sky color

*Bugs Solved*
* 0x1, the firts bug

## Dev2.1: World Gen Update 1.0-b
* Crosshair
* Multithread chunk generation (temporal)
* Chunk customization (biomes?)

*Bugs Solved*
* 0x2
* 0x3

*Known Bugs*
* 0x4: Crosshair cant be seen in the sky

## Dev2.0: World Gen Update 1.0-a
* New render engine, over x1000 performance increase!
* Brown dirt
* Better world gen
* This log
* Better memory management

*Known bugs*
* 0x3: Cubes below water don't render

## Dev1.1
* Better performance
* Documentation

## Dev1.0
* Free camera
* World generation
  * Cubes
  * Water

*Known bugs*
* 0x1: Cubes dont render when betwen chunks
* 0x2: Memory leak when generating world
