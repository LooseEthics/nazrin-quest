
#pragma once

#include <glm/vec3.hpp>
#include <set>

#include "Maze.hpp"
#include "MazeCoord.hpp"

class VisibilitySystem
{
public:
  VisibilitySystem() = default;

  void recalculateCells(
    const Maze& maze,
    const glm::vec3& position
  );

  const std::set<MazeCoord>& visibleCells() const;

private:
  std::set<MazeCoord> visibleCells_;
};
