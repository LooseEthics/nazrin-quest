
#pragma once

#include <cstdint>
#include <vector>

#include "Vertex.hpp"

struct Mesh
{
  std::vector<Vertex> vertices;
  std::vector<uint32_t> wallIndices;
  std::vector<uint32_t> floorIndices;
};
