
#include <array>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Maze.hpp"
#include "Renderer.hpp"

namespace
{
  constexpr const char* MAP_VERTEX_SHADER_PATH = "shaders/map.vert";
  constexpr const char* MAP_FRAGMENT_SHADER_PATH = "shaders/map.frag";
}

void Renderer::initMap()
{
  glGenVertexArrays(1, &mapVertexArray_);
  glGenBuffers(1, &mapVertexBuffer_);

  mapShaderProgram_ = createShaderProgram(MAP_VERTEX_SHADER_PATH, MAP_FRAGMENT_SHADER_PATH);

  mapProjectionLocation_ = glGetUniformLocation(mapShaderProgram_, "projection");
  mapTextureLocation_ = glGetUniformLocation(mapShaderProgram_, "textureSampler");

  glBindVertexArray(mapVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mapVertexBuffer_);

  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    4 * sizeof(float),
    nullptr
  );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    4 * sizeof(float),
    reinterpret_cast<void*>(2 * sizeof(float))
  );

  glEnableVertexAttribArray(1);
}

void Renderer::createMapTexture(const Maze& maze)
{
  mapWidth_ = maze.width();
  mapHeight_ = maze.height();

  std::vector<unsigned char> pixels(
    mapWidth_ * mapHeight_ * 4
  );

  for (int z = 0; z < mapHeight_; ++z){
    for (int x = 0; x < mapWidth_; ++x){
      const size_t i = static_cast<size_t>(z * maze.width() + x) * 4;

      if (maze.get(x, z) == Cell::Wall){
        pixels[i + 0] = 255;
        pixels[i + 1] = 255;
        pixels[i + 2] = 255;
        pixels[i + 3] = 255;
      }
    }
  }

  glGenTextures(1, &mapTexture_);
  glBindTexture(GL_TEXTURE_2D, mapTexture_);

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_S,
    GL_CLAMP_TO_EDGE
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_T,
    GL_CLAMP_TO_EDGE
  );

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    mapWidth_,
    mapHeight_,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    pixels.data()
  );
}

void Renderer::setGoalPosition(const glm::vec3 position)
{
  goalPosition_ = position;
}

void Renderer::drawMap(Camera& camera) const
{
  constexpr float margin = 50.0f;
  constexpr float mapMaxSize = 600.0f;

  const int scale = std::max(1,
    std::min(
      static_cast<int>(mapMaxSize / mapWidth_),
      static_cast<int>(mapMaxSize / mapHeight_)
    )
  );
  const int mapWidth = mapWidth_ * scale;
  const int mapHeight = mapHeight_ * scale;

  const float x0 = (width_ - mapWidth) * 0.5f;
  const float y0 = (height_ - mapHeight) * 0.5f;
  const float x1 = x0 + mapWidth;
  const float y1 = y0 + mapHeight;

  const std::array<glm::vec2, 6> vertices{
    glm::vec2{x0, y0},
    glm::vec2{x1, y0},
    glm::vec2{x1, y1},

    glm::vec2{x0, y0},
    glm::vec2{x1, y1},
    glm::vec2{x0, y1}
  };

  const std::array<glm::vec2, 6> texCoords{
    glm::vec2{0.0f, 0.0f},
    glm::vec2{1.0f, 0.0f},
    glm::vec2{1.0f, 1.0f},

    glm::vec2{0.0f, 0.0f},
    glm::vec2{1.0f, 1.0f},
    glm::vec2{0.0f, 1.0f}
  };

  std::array<float, 24> data;
  for (size_t i = 0; i < 6; ++i){
    data[i * 4 + 0] = vertices[i].x;
    data[i * 4 + 1] = vertices[i].y;
    data[i * 4 + 2] = texCoords[i].x;
    data[i * 4 + 3] = texCoords[i].y;
  };

  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(mapVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mapVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    data.size() * sizeof(float),
    data.data(),
    GL_DYNAMIC_DRAW
  );

  glUseProgram(mapShaderProgram_);

  glUniformMatrix4fv(
    mapProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(uiProjection_)
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mapTexture_);

  glUniform1i(mapTextureLocation_, 0);

  glDrawArrays(
    GL_TRIANGLES,
    0,
    6
  );

  glEnable(GL_DEPTH_TEST);
}
