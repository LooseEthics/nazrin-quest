
#include "Maze.hpp"
#include "MazeGenerator.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(155, 155);
  return 0;
}
