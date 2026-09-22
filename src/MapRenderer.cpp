
#include <array>
#include <glm/gtc/type_ptr.hpp>

#include "MapRenderer.hpp"

namespace
{
  constexpr const char* MAP_VERTEX_SHADER_PATH = "shaders/map.vert";
  constexpr const char* MAP_FRAGMENT_SHADER_PATH = "shaders/map.frag";
  constexpr const char* MAP_MARKER_VERTEX_SHADER_PATH = "shaders/map_marker.vert";
  constexpr const char* MAP_MARKER_FRAGMENT_SHADER_PATH = "shaders/map_marker.frag";

}

MapRenderer::MapRenderer(uint32_t width, uint32_t height)
  : windowWidth_(width),
    windowHeight_(height),
    mapProjection_(glm::ortho(
      0.0f,
      static_cast<float>(windowWidth_),
      static_cast<float>(windowHeight_),
      0.0f
    ))
{
  // map itself
  glGenVertexArrays(1, &mapVertexArray_);
  glGenBuffers(1, &mapVertexBuffer_);

  mapShaderProgram_ = Shader{
    MAP_VERTEX_SHADER_PATH,
    MAP_FRAGMENT_SHADER_PATH
  };

  mapProjectionLocation_ = mapShaderProgram_.projectionLocation();
  mapTextureLocation_ = mapShaderProgram_.textureLocation();

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

  // map marker
  glGenVertexArrays(1, &mapMarkerVertexArray_);
  glGenBuffers(1, &mapMarkerVertexBuffer_);

  mapMarkerShaderProgram_ = Shader{
    MAP_MARKER_VERTEX_SHADER_PATH,
    MAP_MARKER_FRAGMENT_SHADER_PATH
  };

  mapMarkerProjectionLocation_ = mapMarkerShaderProgram_.projectionLocation();

  glBindVertexArray(mapMarkerVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mapMarkerVertexBuffer_);

  glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    2 * sizeof(float),
    nullptr
  );

  glEnableVertexAttribArray(0);


}

MapRenderer::~MapRenderer()
{
  if (mapMarkerVertexBuffer_) glDeleteBuffers(1, &mapMarkerVertexBuffer_);
  if (mapMarkerVertexArray_) glDeleteVertexArrays(1, &mapMarkerVertexArray_);
  if (mapVertexBuffer_) glDeleteBuffers(1, &mapVertexBuffer_);
  if (mapVertexArray_) glDeleteVertexArrays(1, &mapVertexArray_);
}

void MapRenderer::createMapTexture(const Maze& maze)
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
      } else if (Maze::Coord{x, z} == maze.getGoal()) {
        pixels[i + 0] = 0;
        pixels[i + 1] = 255;
        pixels[i + 2] = 0;
        pixels[i + 3] = 255;
      }
    }
  }

  mapTexture_ = Texture{pixels, mapWidth_, mapHeight_, 4};
}

void MapRenderer::drawMap(glm::vec3 pos, float yaw) const
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

  const float x0 = (windowWidth_ - mapWidth) * 0.5f;
  const float y0 = (windowHeight_ - mapHeight) * 0.5f;
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

  mapShaderProgram_.use();

  glUniformMatrix4fv(
    mapProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(mapProjection_)
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mapTexture_.id());

  glUniform1i(mapTextureLocation_, 0);

  glDrawArrays(
    GL_TRIANGLES,
    0,
    6
  );


  // map marker
  const float markerSize = 10.0f;

  const std::array<glm::vec2, 3> marker{
    glm::vec2{              0.0f, -markerSize},
    glm::vec2{ markerSize * 0.6f,  markerSize},
    glm::vec2{-markerSize * 0.6f,  markerSize}
  };

  const float playerX = x0 + (pos.x / static_cast<float>(mapWidth_)) * mapWidth / CELL_SIZE;
  const float playerY = y0 + (pos.z / static_cast<float>(mapHeight_)) * mapHeight / CELL_SIZE;

  const float angle = yaw + glm::half_pi<float>();
  const float c = std::cos(angle);
  const float s = std::sin(angle);

  std::array<glm::vec2, 3> markerVertices;
  for (size_t i = 0; i < marker.size(); ++i){
    const glm::vec2 p = marker[i];

    markerVertices[i] = glm::vec2{
      playerX + p.x * c - p.y * s,
      playerY + p.x * s + p.y * c
    };
  }

  glBindVertexArray(mapMarkerVertexArray_);
  glBindBuffer(GL_ARRAY_BUFFER, mapMarkerVertexBuffer_);

  glBufferData(
    GL_ARRAY_BUFFER,
    markerVertices.size() * sizeof(glm::vec2),
    markerVertices.data(),
    GL_DYNAMIC_DRAW
  );

  mapMarkerShaderProgram_.use();

  glUniformMatrix4fv(
    mapMarkerProjectionLocation_,
    1,
    GL_FALSE,
    glm::value_ptr(mapProjection_)
  );

  glDrawArrays(
    GL_TRIANGLES,
    0,
    3
  );

  glEnable(GL_DEPTH_TEST);
}

void MapRenderer::setWindowDimensions(uint32_t width, uint32_t height)
{
  windowWidth_ = width;
  windowHeight_ = height;
  mapProjection_ = glm::ortho(
    0.0f,
    static_cast<float>(windowWidth_),
    static_cast<float>(windowHeight_),
    0.0f
  );
}
