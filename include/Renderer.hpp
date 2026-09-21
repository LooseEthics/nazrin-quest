
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>
#include <string>

#include "Camera.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"

std::string readFile(const std::string& path);

class Renderer
{
public:
  Renderer(
    int width,
    int height
  );
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void clear() const;
  void drawCamera(Camera& camera) const;
  void drawMaze(Camera& camera) const;
  void drawText(
    const std::string& text,
    const glm::vec2& position,
    float scale,
    const glm::vec3& color
  ) const;
  void drawSprite(
    GLuint texture,
    const glm::vec3 position,
    Camera& camera,
    float width,
    float height
  ) const;
  void drawMap(glm::vec3 pos, float yaw) const;
  void present() const;

  void uploadMesh(const Mesh& mesh);
  void createMapTexture(const Maze& maze);
  void setGoalPosition(const glm::vec3 position);

  [[nodiscard]]
  SDL_Window* window() const;

private:
  void initMap();
  void initMaze();
  void initSprite();
  void initText();

  void destroyMap();
  void destroyMaze();
  void destroySprite();
  void destroyText();

  uint32_t width_;
  uint32_t height_;

  SDL_Window* window_;
  SDL_GLContext context_;

  GLuint mazeVertexArray_;
  GLuint mazeVertexBuffer_;
  GLuint mazeIndexBuffer_;
  GLuint mazeShaderProgram_;

  GLint mazeProjectionLocation_;
  GLint mazeViewLocation_;
  GLint mazeTextureLocation_;

  glm::mat4 mazeProjection_;

  GLuint compileShader(
    GLenum type,
    const char* source
  ) const;

  GLuint createShaderProgram(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath
  ) const;

  GLsizei wallIndexCount_;
  GLsizei floorIndexCount_;
  GLintptr wallIndexOffset_;
  GLintptr floorIndexOffset_;

  GLuint loadTexture(const std::string& path) const;

  GLuint wallTexture_;
  GLuint floorTexture_;
  GLuint goalTexture_;
  glm::vec3 goalPosition_;

  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  GLuint spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  GLuint uiShaderProgram_;

  GLuint uiProjectionLocation_;
  GLuint uiColorLocation_;

  glm::mat4 uiProjection_;

  GLuint mapTexture_;
  GLuint mapVertexArray_;
  GLuint mapVertexBuffer_;
  GLuint mapShaderProgram_;

  GLint mapProjectionLocation_;
  GLint mapTextureLocation_;

  int mapWidth_;
  int mapHeight_;

  GLuint mapMarkerVertexArray_;
  GLuint mapMarkerVertexBuffer_;
  GLuint mapMarkerShaderProgram_;

  GLint mapMarkerProjectionLocation_;
};
