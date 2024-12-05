# cub3d | 1337 BG

**A Journey into Raycasting with miniLibX 💡**

<div align="center">
    <img src="sprite_frames/Screenshot from 2024-12-04 15-58-52.png">
</div>

## Table of Contents

- [Introduction](#introduction)
- [Map Parsing](#map-parsing)
- [Raycasting](#raycasting)
- [Controls](#controls)
- [Bonus Features](#bonus-features)
- [Extras](#extras)
- [Installation](#installation)
- [Usage](#usage)

---

## Introduction

The **cub3d** project aims to develop a 3D game using the **raycasting** technique, the same method made famous by *Wolfenstein 3D*. This group project was a collaborative effort, and I was fortunate to work alongside [@Khdzakariae](https://github.com/Khdzakariae/).

---

## Map Parsing

The **cub3D** executable accepts a single argument: a `.cub` map file. This file must adhere to the following structure:

- **Header Lines:**  
  Specify textures with `NO`, `SO`, `EA`, and `WE`, followed by valid `.xpm` paths. Define floor (`F`) and ceiling (`C`) colors in RGB format separated by commas.

- **Map Body:**  
  Acceptable characters include:
  - `1`: Wall
  - `0`: Floor
  - `N`, `S`, `E`, `W`: Player starting positions  
  The map must be enclosed by walls, with spaces allowed but non-walkable.

**Example Map:**
```plaintext
NO ./texters/1.xpm 

SO ./texters/2.xpm


WE ./texters/4.xpm 




EA ./texters/3.xpm

      F  6,7,11    
      C 6,7,11


1111111111111111111111111111111111111111
1E0000000000000001100000000000000000001            
1111000000000000000000000000000000000001            
1000000000000000000000000000000000000001            
1000000000000000000000000000000000000001            
1000000001000000000000000000000000000001            
10000000000000000111100000000000000000001            
1000000001000000000000000000000000000001            
1000000000000000000000000000000000000001            
10000000110000000011110000000000000000001            
1000000000000000000000000000000000000001            
1000000000000000000000000000000000000001            
1000000000000000000000000000000000000001            
1000000000000000000000000000000000000001            
1111111111111111111111111111111111111111
```


## Raycasting

Raycasting is a rendering technique to create a 3D perspective in a 2D map. 
The logic behind RayCasting is to throw rays in the direction of the player view. Basically, we need to check the distance between the player and the nearest wall (i.e. the point where the ray hits a wall) to caculate the height of the vertical lines we draw.

## Controls

Here is a summary of the various controls in the game:

- The ``WASD`` keys move the player up, down, left and right relative to the player's viewing angle
- The ``left`` and ``right`` arrow keys rotate the viewing angle of the player
- Press the ``ESC`` key or the ``X`` button on the window to exit the game

Note: these are the basic mandatory controls, but we added a few more keys to handle other things. See below for such controls

## Bonus

For this project there were several bonuses, and we did 3 of them:

* Wall Collisions (DONE)

When walking to a wall, instead of stopping in front of it we split the movement into the ``x`` and ``y`` vectors and try to move in either of them, making wall collisions possible

* Minimap

* Doors

* Animations (DONE)

* Rotation with mouse (Press key "1" for activation) (DONE)

This one was very straightforward. There is an event on the ``minilibX`` library that tells the user the position of the mouse. When the position changes, we increment/decrement the player's view direction accordingly

## Extras

We implemented a few things that we were not asked to implement, but we thought would give the project a cooler vibe:

- Added darkening effect to the game. The farther a wall is hit, the darker it is drawn. This gives the game a cave-like feel
- Game works both in Linux and MacOS
- A music running in the background (famous and well known smati hicham instrument remix).

## Installation

### Cloning the repositories
```shell
git clone https://github.com/Khdzakariae/cub3D.git
cd cub3d
```

### Usage

```
make                        compiles cub3D executable

make bonus                  compiles cub3D executable for bonus part

./cub3d map/cub3d.cub #Run mandatory part

./cub3d_BONUS map/cub3d.cub #Run bonus part
```

[@Khdzakariae](https://github.com/Khdzakariae/) | [@achahid19](https://github.com/achahid19/) February 04th, 2024
