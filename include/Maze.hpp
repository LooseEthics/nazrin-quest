
#pragma once

#include <cstdint>
#include <vector>

#include "LogicMaze.hpp"

constexpr int MAZE_MIN_SIZE = 3;

enum class Cell : std::uint8_t
{
  Wall,
  Empty
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

  struct Coord
  {
    int x;
    int y;

    bool operator==(const Coord&) const = default;
  };

  constexpr int xy2index(int x, int y) const
  {
    return y * width_ + x;
  }

  constexpr Coord index2coord(int i) const
  {
    return {i % width_, i / width_};
  }

  void setStart(int x, int y);
  void setGoal(int x, int y);
  Coord getStart() const;
  Coord getGoal() const;

private:
  int width_;
  int height_;
  std::vector<Cell> cells_;

  Coord start_;
  Coord goal_;
};
