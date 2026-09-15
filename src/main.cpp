
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

  Player player{maze.getStartCoords()};

  Renderer renderer{1280, 720, player.camera()};
  bool running = true;

  while (running) {
    //std::cout << "frame\n";
    SDL_Event event;

    while (SDL_PollEvent(&event)){
      if (event.type == SDL_EVENT_QUIT) running = false;
    }

    player.camera().rotate(0.01, 0);

    renderer.clear();
    renderer.draw(meshes.solid, GL_TRIANGLES, {0.5f, 0.5f, 0.5f});
    glLineWidth(3.0f);
    renderer.draw(meshes.edges, GL_LINES, {0.9f, 0.0f, 0.0f});
    renderer.present();
  }

  return 0;
}
