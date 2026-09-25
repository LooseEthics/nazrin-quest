
#include <glm/vec3.hpp>
#include <stdexcept>

#include "Quad.hpp"

const std::vector<Vertex> quadVertices(
  glm::vec3 bottomLeft, glm::vec3 bottomRight,
  glm::vec3 topLeft, glm::vec3 topRight,
  FlatTransform tf
) {
  switch (tf) {
    case FlatTransform::None:
      return {
        {bottomRight, glm::vec2{1.0f, 1.0f}},
        {bottomLeft,  glm::vec2{0.0f, 1.0f}},
        {topLeft,     glm::vec2{0.0f, 0.0f}},

        {bottomRight, glm::vec2{1.0f, 1.0f}},
        {topLeft,     glm::vec2{0.0f, 0.0f}},
        {topRight,    glm::vec2{1.0f, 0.0f}}
      };
    case FlatTransform::Rot90:
      return {
        {bottomRight, glm::vec2{1.0f, 0.0f}},
        {bottomLeft,  glm::vec2{1.0f, 1.0f}},
        {topLeft,     glm::vec2{0.0f, 1.0f}},

        {bottomRight, glm::vec2{1.0f, 0.0f}},
        {topLeft,     glm::vec2{0.0f, 1.0f}},
        {topRight,    glm::vec2{0.0f, 0.0f}}
      };
    case FlatTransform::Rot180:
      return {
        {bottomRight, glm::vec2{0.0f, 0.0f}},
        {bottomLeft,  glm::vec2{1.0f, 0.0f}},
        {topLeft,     glm::vec2{1.0f, 1.0f}},

        {bottomRight, glm::vec2{0.0f, 0.0f}},
        {topLeft,     glm::vec2{1.0f, 1.0f}},
        {topRight,    glm::vec2{0.0f, 1.0f}}
      };
    case FlatTransform::Rot270:
      return {
        {bottomRight, glm::vec2{0.0f, 1.0f}},
        {bottomLeft,  glm::vec2{0.0f, 0.0f}},
        {topLeft,     glm::vec2{1.0f, 0.0f}},

        {bottomRight, glm::vec2{0.0f, 1.0f}},
        {topLeft,     glm::vec2{1.0f, 0.0f}},
        {topRight,    glm::vec2{1.0f, 1.0f}}
      };
    case FlatTransform::FlipX:
      return {
        {bottomRight, glm::vec2{0.0f, 1.0f}},
        {bottomLeft,  glm::vec2{1.0f, 1.0f}},
        {topLeft,     glm::vec2{1.0f, 0.0f}},

        {bottomRight, glm::vec2{0.0f, 1.0f}},
        {topLeft,     glm::vec2{1.0f, 0.0f}},
        {topRight,    glm::vec2{0.0f, 0.0f}}
      };
    case FlatTransform::FlipY:
      return {
        {bottomRight, glm::vec2{1.0f, 0.0f}},
        {bottomLeft,  glm::vec2{0.0f, 0.0f}},
        {topLeft,     glm::vec2{0.0f, 1.0f}},

        {bottomRight, glm::vec2{1.0f, 0.0f}},
        {topLeft,     glm::vec2{0.0f, 1.0f}},
        {topRight,    glm::vec2{1.0f, 1.0f}}
      };
    default:
      throw std::invalid_argument("Invalid FlatTransform");
  }
}
