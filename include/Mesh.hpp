
#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm.vec3.hpp>
#include <vector>

struct Vertex
{
  glm::vec3 position;
  glm::vec2 texCoord;
};

struct Mesh
{
  std::vector<Vertex> vertices;
  std::vector<uint32_t> wallIndices;
  std::vector<uint32_t> floorIndices;
};
