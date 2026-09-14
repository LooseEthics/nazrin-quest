
#pragma once

#include <cstdint>
#include <vector>

constexpr float CELL_SIZE = 1.0f;
constexpr float WALL_HEIGHT = 2.5f;
constexpr float FLOOR = 0.0f;

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
  float startX;
  float startZ;
};
