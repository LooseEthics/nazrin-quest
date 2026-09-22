
#pragma once

#include <glad/gl.h>

#include "Maze.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class MapRenderer
{
public:
  MapRenderer(uint32_t width, uint32_t height);
  ~MapRenderer();

  void drawMap(glm::vec3 pos, float yaw) const;

  void createMapTexture(const Maze& maze);

  void setWindowDimensions(uint32_t width, uint32_t height);

private:
  uint32_t windowWidth_;
  uint32_t windowHeight_;

  Texture mapTexture_;
  GLuint mapVertexArray_;
  GLuint mapVertexBuffer_;
  Shader mapShaderProgram_;

  glm::mat4 mapProjection_;
  GLint mapProjectionLocation_;
  GLint mapTextureLocation_;

  int mapWidth_;
  int mapHeight_;

  GLuint mapMarkerVertexArray_;
  GLuint mapMarkerVertexBuffer_;
  Shader mapMarkerShaderProgram_;

  GLint mapMarkerProjectionLocation_;
};
