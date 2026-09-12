
#pragma once

#include <cstdint>
#include <vector>

#include "LogicMaze.hpp"

constexpr int MAZE_MIN_SIZE = 3;

enum class Cell : std::uint8_t
{
  Wall,
  Empty,
  Start
};

class Maze
{
public:
  Maze(int width, int height);
  Maze(int width, int height, const LogicMaze& logicMaze);

  [[nodiscard]]
  bool isInside(int x, int y) const;

  [[nodiscard]]
  Cell get(int x, int y) const;

  [[nodiscard]]
  Cell getNeighbour(int x, int y, Direction dir) const;

  void set(int x, int y, Cell cell);

  [[nodiscard]]
  int width() const noexcept;
  [[nodiscard]]
  int height() const noexcept;

  void printMazeToConsole() const;

private:
  int width_;
  int height_;
  std::vector<Cell> cells_;
};
