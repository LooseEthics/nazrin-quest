
#pragma once

#include <cstdint>
#include <vector>

struct Vertex
{
  float x;
  float y;
  float z;
};

struct Mesh
{
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
};

struct MazeMeshes
{
  Mesh solid;
  Mesh edges;
};
