
#pragma once

#include <vector>

#include "FlatTransform.hpp"
#include "Vertex.hpp"

const std::vector<Vertex> quadVertices(
  glm::vec3 bottomLeft, glm::vec3 bottomRight,
  glm::vec3 topLeft, glm::vec3 topRight,
  FlatTransform tf
);
