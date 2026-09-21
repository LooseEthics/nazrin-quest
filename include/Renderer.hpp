
#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>
#include <string>

#include "Camera.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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
  Texture goalTexture_;
  glm::vec3 goalPosition_;

  GLuint spriteVertexArray_;
  GLuint spriteVertexBuffer_;
  Shader spriteShaderProgram_;

  GLint spriteProjectionLocation_;
  GLint spriteViewLocation_;
  GLint spriteTextureLocation_;

  GLuint uiVertexArray_;
  GLuint uiVertexBuffer_;
  Shader uiShaderProgram_;

  GLint uiProjectionLocation_;
  GLint uiColorLocation_;

  glm::mat4 uiProjection_;

  Texture mapTexture_;
  GLuint mapVertexArray_;
  GLuint mapVertexBuffer_;
  Shader mapShaderProgram_;

  GLint mapProjectionLocation_;
  GLint mapTextureLocation_;

  int mapWidth_;
  int mapHeight_;

  GLuint mapMarkerVertexArray_;
  GLuint mapMarkerVertexBuffer_;
  Shader mapMarkerShaderProgram_;

  GLint mapMarkerProjectionLocation_;
};
