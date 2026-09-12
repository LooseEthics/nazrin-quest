
#pragma once

#include "Maze.hpp"
#include "Mesh.hpp"

constexpr float CELL_SIZE = 1.0f;
constexpr float WALL_HEIGHT = 2.5f;
constexpr float FLOOR = 0.0f;

// ccw from visible side
//     4------5
//   7-+-----6|
//   | |     ||
//   | 0-----+-1
//   3-------2

constexpr int INDEX_OFFSETS_BOTTOM[] = {0, 1, 2, 3};
constexpr int INDEX_OFFSETS_TOP[] = {7, 6, 5, 4};
constexpr int INDEX_OFFSETS_EAST[] = {1, 2, 6, 5};
constexpr int INDEX_OFFSETS_SOUTH[] = {2, 3, 7, 6};
constexpr int INDEX_OFFSETS_WEST[] = {0, 3, 7, 4};
constexpr int INDEX_OFFSETS_NORTH[] = {0, 4, 1, 5};

class MazeMeshBuilder
{
public:
  [[nodiscard]]
  Mesh build(const Maze& maze) const;

private:
  void addDirectedQuad(Mesh& mesh, uint32_t baseIndex, Direction dir) const;
  void addQuad(Mesh& mesh, uint32_t baseIndex, const int offsets[4]) const;
};
