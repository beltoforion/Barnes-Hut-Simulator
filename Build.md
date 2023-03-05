# How To Build

First, install dependencies (assumes Ubuntu):

```terminal
sudo apt install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev libglu1-mesa-dev mesa-common-dev freeglut3-dev libgomp1
```

Type the following command under the root directory:

```terminal
cmake .
cmake --build .
./bin/barnes-hut-simulator
```
