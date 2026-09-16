# Nazrin Quest

A simple first-person maze exploration game.

The goal is to find the cheese.

## Features

* Procedurally generated mazes
* Perfect maze generation using Wilson's algorithm
* First-person 3D rendering
* Mouse-look camera
* Player movement and wall collision
* Configurable maze parameters

## Requirements

* C++20
* CMake 3.25+
* MinGW-w64

## Compilation and Run

```sh
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/nazrin-quest.exe
```

## Controls

### Configuration screen

```text
Up/down arrow       Option select
Right/left arrow    Increment/decrement option
Hold LCtrl          Value step ×10
Hold LShift         Value step ×100
```

### In game

```text
Mouse movement      Camera control
WASD                Movement
Alt                 Release mouse
Esc                 Quit game
```

## Maze Generation

Mazes are generated using Wilson's algorithm, producing a perfect maze: every cell is reachable from every other cell, with exactly one path between any two cells.

The maze is fully enclosed, with no openings to the outside.

## Project Status

Prototype / technical test.

## Planned features

* In-game map
* Glass maze
* Mirror maze
* First person player model
