
#include <algorithm>
#include <cassert>
#include <iostream>

#include "MazeGenerator.hpp"

MazeGenerator::MazeGenerator(std::uint32_t seed)
  : rng_(std::mt19937{seed})
{}

bool isValidLogicDir(size_t cellIndex, Direction dir, size_t logicWidth, size_t logicHeight)
{
  switch(dir){
    case Direction::East:
      return ((cellIndex + 1) % logicWidth != 0);
    case Direction::South:
      return (cellIndex < logicWidth * (logicHeight - 1));
    case Direction::West:
      return ((cellIndex % logicWidth) != 0);
    case Direction::North:
      return (cellIndex >= logicWidth);
    default:
      // invalid direction
      return false;
  }
}

size_t getNeighbourLogicCell(size_t cellIndex, Direction dir, size_t logicWidth)
{
  // assumes direction is valid
  assert(dir != Direction::Invalid);
  switch(dir){
    case Direction::East:
      return cellIndex + 1;
    case Direction::South:
      return cellIndex + logicWidth;
    case Direction::West:
      return cellIndex - 1;
    case Direction::North:
      return cellIndex - logicWidth;
    default:
      return -1;
  }
}

void MazeGenerator::LERW(size_t branchStart, size_t maxLength)
{
  branch_.push_back(branchStart);
  // std::cout << "LERW " << branchStart << " " << maxLength << "\n";
  // std::cout << (int)logicMaze_.data[branchStart] << " " << (logicMaze_.data[branchStart] & C_BUILT) << "\n";
  size_t index = branchStart, nextIndex = branchStart;
  Direction dir;
  while (
    (index == branchStart && nextIndex == branchStart)
    || ((logicMaze_.data[nextIndex] & C_BUILT) == 0)
    && (((maxLength > 0) && (branch_.size() < maxLength)) || (maxLength == 0))
  ){
    // std::cout << "branch: ";
    // for (int i : branch_) std::cout << i << ", ";
    // std::cout << "\nlen: " << branch_.size() << "\n";

    index = branch_.back();
    dir = Direction::Invalid;
    while (!isValidLogicDir(index, dir, logicMaze_.width, logicMaze_.height))
      dir = static_cast<Direction>(dirDistribution_(rng_));
    // std::cout << "dir: " << (int)dir << "\n";

    nextIndex = getNeighbourLogicCell(index, dir, logicMaze_.width);
    // std::cout << "next: " << nextIndex << "\n";

    auto it = find(branch_.begin(), branch_.end(), nextIndex);
    if (it == branch_.end()){
      branch_.push_back(nextIndex);
      dirVector_.push_back(dir);
    } else {
      size_t dist = distance(branch_.begin(), it) + 1;
      branch_.resize(dist);
      dirVector_.resize(dist - 1);
    }
    // std::cout << (index == branchStart) << " " <<
    //   ((logicMaze_.data[index] & C_BUILT) == 0) << " " <<
    //   ((maxLength > 0) && (branch_.size() < maxLength)) << std::endl;
  }
  // std::cout << "branch: ";
  // for (int i : branch_) std::cout << i << ", ";
  // std::cout << "\n";
  // std::cout << (index == branchStart) << " " <<
  //   ((logicMaze_.data[index] & C_BUILT) == 0) << " " <<
  //   ((maxLength > 0) && (branch_.size() < maxLength)) << std::endl;
}

void MazeGenerator::insertBranch()
{
  size_t index;
  Direction dir = dirVector_[0];
  uint32_t endDist = logicMaze_.distanceFromStart[branch_.back()];
  //std::cout << endDist << "\n";
  const size_t branchLen = branch_.size();
  for (size_t i = 0; i + 1 < branchLen; ++i){
    index = branch_[i];

    logicMaze_.data[index] |= C_BUILT; // cell built
    logicMaze_.distanceFromStart[index] = endDist + branchLen - i - 1;
    //std::cout << index << " " << logicMaze_.distanceFromStart[index] << "\n";

    // entry direction
    if (i != 0){
      logicMaze_.data[index] |= directionMask(dir);
    }

    // exit direction
    dir = dirVector_[i];
    logicMaze_.data[index] |= directionMask(dir);

    // reverse exit direction for next cell
    dir = opposite(dir);
  }

  // last cell
  index = branch_.back();
  logicMaze_.data[index] |= (C_BUILT | directionMask(dir));
  //std::cout << "last " << index << " " << logicMaze_.distanceFromStart[index] << "\n";

  if (logicMaze_.distanceFromStart[branch_[0]] > logicMaze_.longestDistance){
    logicMaze_.furthestCell = branch_[0];
    logicMaze_.longestDistance = logicMaze_.distanceFromStart[branch_[0]];
  }

  logicMaze_.unbuiltCells -= branch_.size() - 1;
}

Maze MazeGenerator::generate(int width, int height)
{
  if (width < MAZE_MIN_SIZE || height < MAZE_MIN_SIZE){
    std::cerr << "Maze dimensions must be >= " << MAZE_MIN_SIZE << ".\n";
    std::cerr << "Got (" << width << ", " << height << ").\n";
    throw std::invalid_argument("Invalid maze dimensions");
  }
  if (width % 2 == 0 || height % 2 == 0){
    std::cerr << "Maze dimensions must be odd-valued.\n";
    std::cerr << "Got (" << width << ", " << height << ").\n";
    throw std::invalid_argument("Invalid maze dimensions");
  }

  logicMaze_ = LogicMaze{
    static_cast<size_t>((width - 1) / 2),
    static_cast<size_t>((height - 1) / 2)};

  cellDistribution_ = std::uniform_int_distribution<size_t>{0, logicMaze_.size() - 1};

  // initial branch
  size_t startIndex = cellDistribution_(rng_);
  size_t initialBranchLength = (logicMaze_.width + logicMaze_.height) / 2;

  //LERW(startIndex, initialBranchLength);

  logicMaze_.data[startIndex] |= C_BUILT | C_START;
  logicMaze_.distanceFromStart[startIndex] = 0;
  logicMaze_.unbuiltCells -= 1;
  // insertBranch();
  //
  // // logicMaze_.printHexes();
  // // Maze maze{width, height, logicMaze_};
  // // maze.printMazeToConsole();
  //
  // branch_.clear();
  // dirVector_.clear();

  // branch from random unbuilt lc until exhausted
  while (logicMaze_.unbuiltCells > 0){
    // std::cout << "unbuiltCells " << logicMaze_.unbuiltCells << "\n";
    startIndex = logicMaze_.getNextUnbuilt(cellDistribution_(rng_));

    LERW(startIndex);

    insertBranch();
    // logicMaze_.printHexes();

    branch_.clear();
    dirVector_.clear();
    // maze = Maze{width, height, logicMaze_};
    // maze.printMazeToConsole();
  }

  logicMaze_.data[logicMaze_.furthestCell] |= C_GOAL;


  Maze maze{width, height, logicMaze_};
  maze.printMazeToConsole();
  return maze;
}
