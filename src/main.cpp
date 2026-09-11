
#include "Maze.hpp"
#include "MazeGenerator.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(5, 5);
  return 0;
}
