# cub3D | 1337 BG

My first RayCaster with miniLibX 💡

<div align="center">
    <img src="sprite_frames/Screenshot from 2024-12-04 15-58-52.png">
</div>

## Table of Contents
- [cub3D | 1337 BG](#cub3d--1337-bg)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Map Parsing](#map-parsing)
  - [Raycasting](#raycasting)
    - [Walls](#walls)
    - [Textures](#textures)
    - [Configuration File Format](#configuration-file-format)
  - [Controls](#controls)
  - [Bonus](#bonus)
  - [Extras](#extras)
  - [Installation](#installation)

## Introduction
The aim of the `cub3d` project is to create a 3D game using the raycasting technique, implemented in the world-famous `Wolfenstein 3D` game.

## Map Parsing
The `cub3D` executable receives a single argument, the map file in `.cub` format. The file must follow these rules:
- The map must be composed of only 6 possible characters: `0` for an empty space, `1` for a wall, and `N`, `S`, `E`, or `W` for the player's start position and spawning orientation.
- The map must be closed/surrounded by walls, otherwise, the program returns an error.
- Each type of element can be separated by one or more empty lines.
- Each type of element can be set in any order in the file, except for the map which always has to be the last.
- Each type of information from an element can be separated by one or more spaces.
- Spaces are a valid part of the map and should be handled accordingly.

## Raycasting
Raycasting creates a 3D perspective in a 2D map by throwing rays in the player's view direction to calculate the distance to the nearest wall.

### Walls
Calculate the distance between the player and the nearest wall using steps such as initializing attributes, moving rays, and fixing fisheye effects.

### Textures
Choose the correct texture based on which side of the wall was hit, and implement stretching if necessary.

![Raycasting Example](https://i.sstatic.net/2DdYw.gif)

Here are some images explaining the raycasting process:

![Raycasting Image 1](https://permadi.com/tutorial/raycast/images/figure33.gif)
![Raycasting Image 2](https://permadi.com/tutorial/raycast/images/figure34.gif)
![Raycasting Image 3](https://permadi.com/tutorial/raycast/images/figure35.gif)

### Configuration File Format
Your program must take as a first argument a scene description file with the `.cub` extension. The file must follow these rules:

- The map must be composed of only 6 possible characters: `0` for an empty space, `1` for a wall, and `N`, `S`, `E`, or `W` for the player's start position and spawning orientation.
- The map must be closed/surrounded by walls, if not the program must return an error.
- Except for the map content, each type of element can be separated by one or more empty line(s).
- Except for the map content which always has to be the last, each type of element can be set in any order in the file.
- Except for the map, each type of information from an element can be separated by one or more space(s).
- The map must be parsed as it looks in the file. Spaces are a valid part of the map and are up to you to handle. You must be able to parse any kind of map, as long as it respects the rules of the map.

Each element (except the map) first information is the type identifier (composed by one or two character(s)), followed by all specific information for each object in a strict order such as:

- North texture:
  - Identifier: `NO`
  - Path to the north texture
- South texture:
  - Identifier: `SO`
  - Path to the south texture
- West texture:
  - Identifier: `WE`
  - Path to the west texture
- East texture:
  - Identifier: `EA`
  - Path to the east texture
- Floor color:
  - Identifier: `F`
  - R,G,B colors in range [0,255]: 0, 255, 255
- Ceiling color:
  - Identifier: `C`
  - R,G,B colors in range [0,255]: 0, 255, 255

Example of the mandatory part with a minimalist `.cub` scene:
```
NO ./path_to_the_north_texture
SO ./path_to_the_south_texture
WE ./path_to_the_west_texture
EA ./path_to_the_east_texture
F 220,100,0
C 225,30,0
1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

If any misconfiguration of any kind is encountered in the file, the program must exit properly and return "Error\n" followed by an explicit error message of your choice.
## Controls
The program displays the image in a window and respects the following rules:
- The left and right arrow keys of the keyboard must allow you to look left and right in the maze.
- The W, A, S, and D keys must allow you to move the point of view through the maze.
- Pressing ESC must close the window and quit the program cleanly.
- Clicking on the red cross on the window’s frame must close the window and quit the program cleanly.
- The use of images of the minilibX is strongly recommended.

## Bonus
Implemented bonuses include wall collisions, minimap, doors, animations, and rotation with the mouse.

## Extras
Additional features include darkening effects, support for Linux and MacOS, and background music.

## Installation
To clone the repository and compile the `cub3D` executable, follow these steps:

1. Clone the repository:
```shell
git clone https://github.com/Khdzakariae/cub3D.git
cd cub3d
```

2. Compile the `cub3D` executable:
```shell
make
```

3. Run the `cub3D` executable:
```shell
./cub3d map/cub3d.cub
```

@Khdzakariae | @achahid19 February 04th, 2024


