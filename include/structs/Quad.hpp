
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "FlatTransform.hpp"
#include "Vertex.hpp"

glm::vec2 transformUV(
  glm::vec2 uv,
  const FlatTransform& tf
);

const std::vector<Vertex> getQuadVertices(
  glm::vec3 bottomLeft,
  glm::vec3 bottomRight,
  glm::vec3 topLeft,
  glm::vec3 topRight,
  FlatTransform tf
);
