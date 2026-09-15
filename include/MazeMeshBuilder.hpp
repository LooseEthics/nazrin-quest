
#pragma once

#include "CommonGeometry.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"

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
constexpr int INDEX_OFFSETS_CUBE_EDGES[] = {
  0, 1, 1, 2, 2, 3, 3, 0,
  0, 4, 1, 5, 2, 6, 3, 7,
  4, 5, 5, 6, 6, 7, 7, 4};

class MazeMeshBuilder
{
public:
  [[nodiscard]]
  MazeMeshes build(const Maze& maze) const;

private:
  void addDirectedQuad(Mesh& mesh, uint32_t baseIndex, Direction dir) const;
  void addQuad(Mesh& mesh, uint32_t baseIndex, const int offsets[4]) const;
  void addCubeFrame(Mesh& mesh, uint32_t baseIndex, const int offsets[24]) const;
};
