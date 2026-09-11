
#include "Maze.hpp"
#include "MazeGenerator.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(97, 117);
  return 0;
}
