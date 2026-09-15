
#include <glad/gl.h>
#include <iostream>

#include "Maze.hpp"
#include "MazeGenerator.hpp"
#include "MazeMeshBuilder.hpp"
#include "Player.hpp"
#include "Renderer.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(205, 203);

  MazeMeshBuilder meshBuilder;
  MazeMeshes meshes = meshBuilder.build(maze);

  Player player{maze};

  Renderer renderer{1280, 720, player.camera()};

  Input input;
  Uint64 previous = SDL_GetTicks();

  while (!input.quitRequested()) {
    const glm::vec3 pos = player.pos();
    const float yaw = player.yaw();
    const float pitch = player.pitch();

    std::string title =
      "Nazrin Quest | X: " + std::to_string(pos.x) +
      " Y: " + std::to_string(pos.y) +
      " Z: " + std::to_string(pos.z) +
      " Yaw: " + std::to_string(yaw) +
      " Pitch: " + std::to_string(pitch);
    SDL_SetWindowTitle(renderer.window(), title.c_str());

    Uint64 current = SDL_GetTicks();
    float deltaTime = static_cast<float>(current - previous) / 1000.0f;
    previous = current;

    SDL_Event event;

    while (SDL_PollEvent(&event)){
      input.processEvent(event);
    }

    SDL_SetWindowRelativeMouseMode(renderer.window(), input.mouseCaptured());
    player.update(input, deltaTime);

    renderer.clear();

    renderer.draw(meshes.solid, GL_TRIANGLES, {0.5f, 0.5f, 0.5f});

    glLineWidth(3.0f);
    renderer.draw(meshes.edges, GL_LINES, {0.9f, 0.0f, 0.0f});

    renderer.present();

    input.endFrame();
  }

  return 0;
}
