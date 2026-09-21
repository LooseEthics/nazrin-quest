
#include <cstddef>
#include <cstdint>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonGeometry.hpp"
#include "Renderer.hpp"

namespace
{
  constexpr const char* MAZE_VERTEX_SHADER_PATH = "shaders/maze.vert";
  constexpr const char* MAZE_FRAGMENT_SHADER_PATH = "shaders/maze.frag";

  constexpr const char* WALL_TEXTURE_PATH = "assets/tex_wall.png";
  constexpr const char* FLOOR_TEXTURE_PATH = "assets/tex_floor.png";
}

void Renderer::initMaze()
{
  glGenVertexArrays(1, &mazeVertexArray_);
  glGenBuffers(1, &mazeVertexBuffer_);
  glGenBuffers(1, &mazeIndexBuffer_);

  mazeShaderProgram_ = createShaderProgram(MAZE_VERTEX_SHADER_PATH, MAZE_FRAGMENT_SHADER_PATH);

  mazeProjectionLocation_ = glGetUniformLocation(mazeShaderProgram_, "projection");
  mazeViewLocation_ = glGetUniformLocation(mazeShaderProgram_, "view");
  mazeTextureLocation_ = glGetUniformLocation(mazeShaderProgram_, "textureSampler");

  mazeProjection_ = glm::perspective(
    glm::radians(80.0f),
    static_cast<float>(width_) / static_cast<float>(height_),
    0.01f,
    500.0f
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

  wallTexture_ = loadTexture(WALL_TEXTURE_PATH);
  floorTexture_ = loadTexture(FLOOR_TEXTURE_PATH);
}

void Renderer::destroyMaze()
{
  if (floorTexture_) glDeleteTextures(1, &floorTexture_);
  if (wallTexture_) glDeleteTextures(1, &wallTexture_);
  if (mazeShaderProgram_) glDeleteProgram(mazeShaderProgram_);
  if (mazeIndexBuffer_) glDeleteBuffers(1, &mazeIndexBuffer_);
  if (mazeVertexBuffer_) glDeleteBuffers(1, &mazeVertexBuffer_);
  if (mazeVertexArray_) glDeleteVertexArrays(1, &mazeVertexArray_);
}

void Renderer::uploadMesh(const Mesh& mesh)
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

void Renderer::drawMaze(Camera& camera) const
{
  glBindVertexArray(mazeVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mazeVertexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mazeIndexBuffer_);

  glUseProgram(mazeShaderProgram_);

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

  glBindTexture(GL_TEXTURE_2D, wallTexture_);

  glDrawElements(
    GL_TRIANGLES,
    wallIndexCount_,
    GL_UNSIGNED_INT,
    nullptr
  );

  glBindTexture(GL_TEXTURE_2D, floorTexture_);

  glDrawElements(
    GL_TRIANGLES,
    floorIndexCount_,
    GL_UNSIGNED_INT,
    reinterpret_cast<void*>(floorIndexOffset_)
  );
}
