
#include <cstddef>
#include <cstdint>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonGeometry.hpp"
#include "MazeRenderer.hpp"
#include "Vertex.hpp"

namespace
{
  constexpr const char* MAZE_VERTEX_SHADER_PATH = "shaders/maze.vert";
  constexpr const char* MAZE_FRAGMENT_SHADER_PATH = "shaders/maze.frag";

  constexpr const char* WALL_TEXTURE_PATH = "assets/tex_wall.png";
  constexpr const char* FLOOR_TEXTURE_PATH = "assets/tex_floor.png";
}

MazeRenderer::MazeRenderer(uint32_t width, uint32_t height)
  : SubRendererBase(width, height)
{
  calculateProjection();

  glGenVertexArrays(1, &mazeVertexArray_);
  glGenBuffers(1, &mazeVertexBuffer_);
  glGenBuffers(1, &mazeIndexBuffer_);

  mazeShaderProgram_ = Shader{
    MAZE_VERTEX_SHADER_PATH,
    MAZE_FRAGMENT_SHADER_PATH
  };

  mazeProjectionLocation_ = mazeShaderProgram_.projectionLocation();
  mazeViewLocation_ = mazeShaderProgram_.viewLocation();
  mazeTextureLocation_ = mazeShaderProgram_.textureLocation();

  mazeProjection_ = glm::perspective(
    glm::radians(FOV),
    static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_),
    NEAR_CULLING_PLANE,
    FAR_CULLING_PLANE
  );

  glBindVertexArray(mazeVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mazeVertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mazeIndexBuffer_);

  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    nullptr
  );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    reinterpret_cast<void*>(offsetof(Vertex, texCoord))
  );

  glEnableVertexAttribArray(1);

  wallTexture_ = Texture{WALL_TEXTURE_PATH};
  floorTexture_ = Texture{FLOOR_TEXTURE_PATH};
}

MazeRenderer::~MazeRenderer()
{
  if (mazeIndexBuffer_) glDeleteBuffers(1, &mazeIndexBuffer_);
  if (mazeVertexBuffer_) glDeleteBuffers(1, &mazeVertexBuffer_);
  if (mazeVertexArray_) glDeleteVertexArrays(1, &mazeVertexArray_);
}

void MazeRenderer::uploadMesh(const Mesh& mesh)
{
  glBindVertexArray(mazeVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mazeVertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mazeIndexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    mesh.vertices.size() * sizeof(Vertex),
    mesh.vertices.data(),
    GL_STATIC_DRAW
  );

  wallIndexCount_ = static_cast<GLsizei>(mesh.wallIndices.size());
  floorIndexCount_ = static_cast<GLsizei>(mesh.floorIndices.size());

  const GLsizeiptr wallSize = mesh.wallIndices.size() * sizeof(uint32_t);
  const GLsizeiptr floorSize = mesh.floorIndices.size() * sizeof(uint32_t);

  floorIndexOffset_ = static_cast<GLintptr>(wallSize);


  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    wallSize + floorSize,
    nullptr,
    GL_STATIC_DRAW
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    0,
    wallSize,
    mesh.wallIndices.data()
  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,
    floorIndexOffset_,
    floorSize,
    mesh.floorIndices.data()
  );
}

void MazeRenderer::drawMaze(Camera& camera) const
{
  glBindVertexArray(mazeVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mazeVertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mazeIndexBuffer_);

  mazeShaderProgram_.use();

  const glm::mat4 view = camera.viewMatrix();

  glUniformMatrix4fv(
    mazeProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(mazeProjection_)
  );

  glUniformMatrix4fv(
    mazeViewLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(view)
  );

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(mazeTextureLocation_, 0);

  glBindTexture(GL_TEXTURE_2D, wallTexture_.id());

  glDrawElements(
    GL_TRIANGLES,
    wallIndexCount_,
    GL_UNSIGNED_INT,
    nullptr
  );

  glBindTexture(GL_TEXTURE_2D, floorTexture_.id());

  glDrawElements(
    GL_TRIANGLES,
    floorIndexCount_,
    GL_UNSIGNED_INT,
    reinterpret_cast<void*>(floorIndexOffset_)
  );
}

void MazeRenderer::calculateProjection()
{
  mazeProjection_ = glm::perspective(
    glm::radians(FOV),
    static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_),
    NEAR_CULLING_PLANE,
    FAR_CULLING_PLANE
  );
}
