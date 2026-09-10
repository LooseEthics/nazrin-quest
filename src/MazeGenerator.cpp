
#include <algorithm>
#include <iostream>

#include "MazeGenerator.hpp"

MazeGenerator::MazeGenerator(std::uint32_t seed)
  : rng_(std::mt19937{seed})
{}

bool isValidLogicDir(size_t cellIndex, uint8_t dir, size_t logicWidth, size_t logicHeight)
{
  switch(dir){
    case 0:
      // east
      return (cellIndex + 1 % logicWidth != 0);
    case 1:
      // south
      return (cellIndex < logicWidth * (logicHeight - 1));
    case 2:
      // west
      return (cellIndex % logicWidth != 0);
    case 3:
      // north
      return (cellIndex >= logicWidth);
    default:
      // invalid direction
      return false;
  }
}

size_t getNeightbourLogicCell(size_t cellIndex, uint8_t dir, size_t logicWidth)
{
  // assumes direction is valid
  switch(dir){
    case 0:
      // east
      return cellIndex + 1;
    case 1:
      // south
      return cellIndex + logicWidth;
    case 2:
      // west
      return cellIndex - 1;
    case 3:
      // north
      return cellIndex - logicWidth;
    default:
      // invalid direction
      return -1;
  }
}

Maze MazeGenerator::generate(int width, int height)
{
  if (width < MAZE_MIN_SIZE || height < MAZE_MIN_SIZE){
    std::cerr << "Maze dimensions must be >= " << MAZE_MIN_SIZE << ".\n";
    std::cerr << "Got (" << width << ", " << height << ").\n";
    return Maze(MAZE_MIN_SIZE, MAZE_MIN_SIZE);
  }
  if (width % 2 == 0 || height % 2 == 0){
    std::cerr << "Maze dimensions must be odd-valued.\n";
    std::cerr << "Got (" << width << ", " << height << ").\n";
    return Maze(MAZE_MIN_SIZE, MAZE_MIN_SIZE);
  }

  size_t logicWidth = (width - 1) / 2;
  size_t logicHeight = (height - 1) / 2;
  std::vector<LogicCell> logicMaze{logicWidth * logicHeight};

  std::uniform_int_distribution<size_t> cellDistribution{0, logicWidth * logicHeight - 1};
  std::uniform_int_distribution<uint8_t> dirDistribution{0, 3};

  // initial branch
  size_t startIndex = cellDistribution(rng_);
  size_t initialBranchLength = (logicWidth + logicHeight) / 2;
  std::vector<size_t> initialBranch;
  uint8_t dir = -1;
  std::vector<size_t> dirVector;
  size_t lastIndex, nextIndex;

  initialBranch.push_back(startIndex);
  while (initialBranch.size() < initialBranchLength){
    lastIndex = initialBranch.back();
    while (!isValidLogicDir(lastIndex, dir, logicWidth, logicHeight))
      dir = dirDistribution(rng_);
    nextIndex = getNeightbourLogicCell(lastIndex, dir, logicWidth);
    auto it = find(initialBranch.begin(), initialBranch.end(), nextIndex);
    if (it == initialBranch.end()){
      initialBranch.push_back(nextIndex);
      dirVector.push_back(dir);
    }
    else{
      size_t dist = distance(initialBranch.begin(), it)
      initialBranch.resize(dist);
      dirVector.resize(dist - 1);
    }
  }
  // branch from random unbuilt lc until exhausted


  Maze maze{width, height};
  return maze;
}
