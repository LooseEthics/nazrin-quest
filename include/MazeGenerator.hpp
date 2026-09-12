
#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include "LogicMaze.hpp"
#include "Maze.hpp"

class MazeGenerator
{
public:
  MazeGenerator(std::uint32_t seed);

  [[nodiscard]]
  Maze generate(int width, int height);

private:
  std::mt19937 rng_;

  LogicMaze logicMaze_;
  std::vector<size_t> branch_;
  std::vector<Direction> dirVector_;

  std::uniform_int_distribution<size_t> cellDistribution_;
  std::uniform_int_distribution<uint8_t> dirDistribution_{0, 3};

  void LERW(size_t branchStart, size_t maxLength = 0);
  void insertBranch();
};
