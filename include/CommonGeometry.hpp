
#pragma once

#include <glm/glm.hpp>

constexpr float CELL_SIZE = 1.0f;
constexpr float WALL_HEIGHT = 2.5f;
constexpr float FLOOR = 0.0f;

struct vec3f
{
  float x;
  float y;
  float z;

  inline vec3f operator+(const vec3f& other) const
  {
    return vec3f{
      x + other.x,
      y + other.y,
      z + other.z
    };
  }
};

glm::vec3 forwardVector(float yaw, float pitch);
glm::vec3 flatForwardVector(float yaw, float pitch);
