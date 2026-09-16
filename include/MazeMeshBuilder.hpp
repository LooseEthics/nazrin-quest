
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

constexpr int INDEX_OFFSETS_BOTTOM[] = {3, 2, 1, 0}; // floor indices face into the cube
constexpr int INDEX_OFFSETS_TOP[] = {7, 6, 5, 4};
constexpr int INDEX_OFFSETS_EAST[] = {1, 2, 6, 5};
constexpr int INDEX_OFFSETS_SOUTH[] = {2, 3, 7, 6};
constexpr int INDEX_OFFSETS_WEST[] = {0, 3, 7, 4};
constexpr int INDEX_OFFSETS_NORTH[] = {0, 1, 5, 4};

class MazeMeshBuilder
{
public:
  [[nodiscard]]
  Mesh build(const Maze& maze) const;

private:
  void addDirectedQuad(
    Mesh& mesh,
    Direction dir,
    const std::array<glm::vec3, 8> vertexPositions
  ) const;
  void addQuad(
    Mesh& mesh,
    std::vector<uint32_t>& indexVector,
    const std::array<Vertex, 4> quadVertices
  ) const;
};
