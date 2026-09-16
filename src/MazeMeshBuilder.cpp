
#include <array>
#include <iostream>
#include <stdexcept>

#include "MazeMeshBuilder.hpp"

Mesh MazeMeshBuilder::build(const Maze& maze) const
{
  Mesh mesh;

  for (int z = 0; z < maze.height(); ++z){
    for (int x = 0; x < maze.width(); ++x){
      if (maze.get(x, z) != Cell::Wall) continue;

      const uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());
      const std::array<Vertex, 8> cellVertices = {{
        {x * CELL_SIZE      , FLOOR              , z * CELL_SIZE      },
        {(x + 1) * CELL_SIZE, FLOOR              , z * CELL_SIZE      },
        {(x + 1) * CELL_SIZE, FLOOR              , (z + 1) * CELL_SIZE},
        {x * CELL_SIZE      , FLOOR              , (z + 1) * CELL_SIZE},
        {x * CELL_SIZE      , FLOOR + WALL_HEIGHT, z * CELL_SIZE      },
        {(x + 1) * CELL_SIZE, FLOOR + WALL_HEIGHT, z * CELL_SIZE      },
        {(x + 1) * CELL_SIZE, FLOOR + WALL_HEIGHT, (z + 1) * CELL_SIZE},
        {x * CELL_SIZE      , FLOOR + WALL_HEIGHT, (z + 1) * CELL_SIZE}
      }};

      mesh.vertices.insert(
        mesh.vertices.end(),
        cellVertices.begin(),
        cellVertices.end()
      );

      addDirectedQuad(mesh, baseIndex, Direction::Up);
      for (Direction dir : cardinalDirs){
        if (maze.getNeighbour(x, z, dir) != Cell::Wall)
          addDirectedQuad(mesh, baseIndex, dir);
      }

      addCubeFrame(mesh, baseIndex, INDEX_OFFSETS_CUBE_EDGES);
    }
  }
  // std::cout << "Vertices: " << mesh.vertices.size() << "\n";
  // std::cout << "Indices: " << mesh.indices.size() << "\n";
  return mesh;
}

void MazeMeshBuilder::addDirectedQuad(
  Mesh& mesh,
  uint32_t baseIndex,
  Direction dir
) const {
  const int* offsets;
  switch (dir){
    case Direction::Up:
      offsets = INDEX_OFFSETS_TOP;
      break;
    case Direction::Down:
      offsets = INDEX_OFFSETS_BOTTOM;
      break;
    case Direction::East:
      offsets = INDEX_OFFSETS_EAST;
      break;
    case Direction::South:
      offsets = INDEX_OFFSETS_SOUTH;
      break;
    case Direction::West:
      offsets = INDEX_OFFSETS_WEST;
      break;
    case Direction::North:
      offsets = INDEX_OFFSETS_NORTH;
      break;
    default:
      throw std::invalid_argument("Invalid Maze direction");
  }
  addQuad(mesh, baseIndex, offsets);
}

void MazeMeshBuilder::addQuad(
  Mesh& mesh,
  uint32_t baseIndex,
  const int offsets[4]
) const {
  mesh.triIndices.insert(
    mesh.triIndices.end(),
    {
      baseIndex + offsets[0],
      baseIndex + offsets[1],
      baseIndex + offsets[2],

      baseIndex + offsets[0],
      baseIndex + offsets[2],
      baseIndex + offsets[3]
    }
  );
}

void MazeMeshBuilder::addCubeFrame(
  Mesh& mesh,
  uint32_t baseIndex,
  const int offsets[24]
) const {
  for (int i = 0; i < 24; ++i){
    mesh.edgeIndices.push_back(baseIndex + offsets[i]);
  }
}
