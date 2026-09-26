
#pragma once

#include <glm/glm.hpp>

constexpr float CELL_SIZE = 2.0f;
constexpr float WALL_HEIGHT = 2.5f;
constexpr float FLOOR = 0.0f;
constexpr float GRAVITY = 9.8f;
constexpr float LANDING_TOLERANCE = 0.05f;

glm::vec3 forwardVector(float yaw, float pitch);
glm::vec3 flatForwardVector(float yaw, float pitch);
