
#include "Maze.hpp"
#include "MazeGenerator.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(205, 203);
  return 0;
}
