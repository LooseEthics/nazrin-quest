
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "SubRendererBase.hpp"
#include "Texture.hpp"

class MazeRenderer : public SubRendererBase
{
public:
  MazeRenderer(uint32_t width, uint32_t height);
  ~MazeRenderer();

  void drawMaze(Camera& camera) const;

  void uploadMesh(const Mesh& mesh);

private:
  void calculateProjection() override;

  GLuint mazeVertexArray_;
  GLuint mazeVertexBuffer_;
  GLuint mazeIndexBuffer_;
  Shader mazeShaderProgram_;

  GLint mazeProjectionLocation_;
  GLint mazeViewLocation_;
  GLint mazeTextureLocation_;

  glm::mat4 mazeProjection_;

  GLsizei wallIndexCount_;
  GLsizei floorIndexCount_;
  GLintptr wallIndexOffset_;
  GLintptr floorIndexOffset_;

  Texture wallTexture_;
  Texture floorTexture_;
};
