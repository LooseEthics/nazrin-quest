
#include <iostream>

#include "Maze.hpp"
#include "MazeGenerator.hpp"
#include "MazeMeshBuilder.hpp"
#include "Renderer.hpp"

int main()
{
  MazeGenerator mg{69420};
  Maze maze = mg.generate(205, 203);

  MazeMeshBuilder meshBuilder;
  Mesh mesh = meshBuilder.build(maze);

  Renderer renderer{1280, 720};
  bool running = true;

  while (running) {
    //std::cout << "frame\n";
    SDL_Event event;

    while (SDL_PollEvent(&event)){
      if (event.type == SDL_EVENT_QUIT) running = false;
    }

    renderer.clear();
    renderer.draw(mesh);
    renderer.present();
  }

  return 0;
}
