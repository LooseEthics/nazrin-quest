
#include <array>
#include <iostream>
#include <stdexcept>

#include "MazeMeshBuilder.hpp"

Mesh MazeMeshBuilder::build(const Maze& maze) const
{
  Mesh mesh;

  for (int z = 0; z < maze.height(); ++z){
    for (int x = 0; x < maze.width(); ++x){
      const uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());
      const std::array<glm::vec3, 8> vertexPositions = {
        glm::vec3{x * CELL_SIZE      , FLOOR              , z * CELL_SIZE      },
        glm::vec3{(x + 1) * CELL_SIZE, FLOOR              , z * CELL_SIZE      },
        glm::vec3{(x + 1) * CELL_SIZE, FLOOR              , (z + 1) * CELL_SIZE},
        glm::vec3{x * CELL_SIZE      , FLOOR              , (z + 1) * CELL_SIZE},
        glm::vec3{x * CELL_SIZE      , FLOOR + WALL_HEIGHT, z * CELL_SIZE      },
        glm::vec3{(x + 1) * CELL_SIZE, FLOOR + WALL_HEIGHT, z * CELL_SIZE      },
        glm::vec3{(x + 1) * CELL_SIZE, FLOOR + WALL_HEIGHT, (z + 1) * CELL_SIZE},
        glm::vec3{x * CELL_SIZE      , FLOOR + WALL_HEIGHT, (z + 1) * CELL_SIZE}
      };

      if (maze.get(x, z) != Cell::Wall){
        addDirectedQuad(mesh, Direction::Down, vertexPositions);
      } else {
        addDirectedQuad(mesh, Direction::Up, vertexPositions);
        for (Direction dir : cardinalDirs){
          if (maze.getNeighbour(x, z, dir) != Cell::Wall)
            addDirectedQuad(mesh, dir, vertexPositions);
        }
      }
    }
  }
  return mesh;
}

void MazeMeshBuilder::addDirectedQuad(
  Mesh& mesh,
  Direction dir,
  const std::array<glm::vec3, 8> vertexPositions
) const {
  const int* offsets;
  bool floor = false;
  switch (dir){
    case Direction::Up:
      offsets = INDEX_OFFSETS_TOP;
      break;
    case Direction::Down:
      offsets = INDEX_OFFSETS_BOTTOM;
      floor = true;
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
  const std::array<Vertex, 4> quadVertices = {{
    {vertexPositions[offsets[0]], glm::vec2{0.0f, 1.0f}},
    {vertexPositions[offsets[1]], glm::vec2{1.0f, 1.0f}},
    {vertexPositions[offsets[2]], glm::vec2{1.0f, 0.0f}},
    {vertexPositions[offsets[3]], glm::vec2{0.0f, 0.0f}}
  }};
  addQuad(
    mesh,
    (floor ? mesh.floorIndices : mesh.wallIndices),
    quadVertices
  );
}

void MazeMeshBuilder::addQuad(
  Mesh& mesh,
  std::vector<uint32_t>& indexVector,
  const std::array<Vertex, 4> quadVertices
) const {
  const uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());

  mesh.vertices.insert(
    mesh.vertices.end(),
    quadVertices.begin(),
    quadVertices.end()
  );

  indexVector.insert(
    indexVector.end(),
    {
      baseIndex + 0,
      baseIndex + 1,
      baseIndex + 2,

      baseIndex + 0,
      baseIndex + 2,
      baseIndex + 3
    }
  );
}
