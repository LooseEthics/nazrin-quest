
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#include "Cell.hpp"
#include "CommonGeometry.hpp"
#include "LogicMaze.hpp"
#include "MazeCoord.hpp"

constexpr int MAZE_MIN_SIZE = 5;
constexpr int MAZE_MAX_SIZE = 205;

class Maze
{
public:
  Maze(int width, int height);
  Maze(int width, int height, const LogicMaze& logicMaze);

  [[nodiscard]]
  bool isInside(MazeCoord coord) const;
  [[nodiscard]]
  bool isInside(int x, int y) const;

  [[nodiscard]]
  Cell get(MazeCoord coord) const;
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

  constexpr int xy2index(int x, int y) const
  {
    return y * width_ + x;
  }

  constexpr MazeCoord index2coord(int i) const
  {
    return {i % width_, i / width_};
  }

  constexpr int world2index(glm::vec3 pos) const
  {
    return static_cast<int>(pos.z / CELL_SIZE) * width_ + static_cast<int>(pos.x / CELL_SIZE);
  }

  constexpr MazeCoord world2xy(glm::vec3 pos) const
  {
    return index2coord(world2index(pos));
  }

  void setStart(int x, int y);
  void setGoal(int x, int y);
  MazeCoord getStart() const;
  MazeCoord getGoal() const;
  glm::vec3 getStartCoords() const;
  glm::vec3 getGoalCoords() const;

private:
  int width_;
  int height_;
  std::vector<Cell> cells_;

  MazeCoord start_;
  MazeCoord goal_;
};
