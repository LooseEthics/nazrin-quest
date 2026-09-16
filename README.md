
A simple maze exploration game.
The goal is to find the cheese.

Compilation and run:
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/nazrin-quest.exe
```

Controls:
Configuration screen:
```
Up/down arrow               Option select
Right/left arrow            Increment/decrement option
Hold LCtrl                  Value step 10
Hold LShift                 Value step 100
```

In game:
```
Mouse movement              Camera control
WASD                        Movement
Alt                         Release mouse
Esc                         Quit game
```

Maze is generated using Wilson's algorithm - perfect maze with no exits.
