
#include "Maze.hpp"
#include "MazeGenerator.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(15, 15);
  return 0;
}
