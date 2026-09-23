
#include "Direction.hpp"
#include "VisibilitySystem.hpp"

void VisibilitySystem::recalculateCells(
  const Maze& maze,
  const glm::vec3& position
) {
  MazeCoord playerCell = maze.world2xy(position);

  const auto isValidCell = [&](MazeCoord cell) {
    return maze.isInside(cell) && maze.get(cell) != Cell::Wall;
  };

  visibleCells_.clear();
  visibleCells_.insert(playerCell);

  for (Direction neighborDirection : cardinalDirs){
    MazeCoord neighborCell = playerCell + neighborOffset(neighborDirection);

    if (!isValidCell(neighborCell)){
      continue;
    }

    visibleCells_.insert(neighborCell);

    // march from each neighbor cell in 3 cardinal directions
    // away from playerCell until we hit a wall
    // along the way check to march sides 1 cell deep
    for (Direction marchDirection : cardinalDirs){
      if (marchDirection == opposite(neighborDirection)) continue;

      MazeCoord activeCell = neighborCell;
      while (isValidCell(activeCell)){
        visibleCells_.insert(activeCell);
        
        MazeCoord lhc = activeCell + neighborOffset(leftCardinal(marchDirection));
        MazeCoord rhc = activeCell + neighborOffset(rightCardinal(marchDirection));
        if (isValidCell(lhc))
          visibleCells_.insert(lhc);
        if (isValidCell(rhc))
          visibleCells_.insert(rhc);

        activeCell = activeCell + neighborOffset(marchDirection);
      }
    }
  }
}

const std::set<MazeCoord>& VisibilitySystem::visibleCells() const {return visibleCells_;}
