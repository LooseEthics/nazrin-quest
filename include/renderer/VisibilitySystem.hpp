
#pragma once

#include <glad/gl.h>
#include <set>

#include "Maze.hpp"
#include "MazeCoord.hpp"

class VisibilitySystem
{
public:
  VisibilitySystem() = default;

  const std::set<MazeCoord>& visibleCells(
    const Maze& maze,
    const glm::vec3& position
  );

private:
  MazeCoord lastPlayerCoord_ = {-1, -1};
  std::set<MazeCoord> visibleCells_;
};
