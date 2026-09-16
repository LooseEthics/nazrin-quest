
#include <iostream>

#include "Game.hpp"
#include "MazeGenerator.hpp"
#include "MazeMeshBuilder.hpp"

Game::Game()
  : input_(std::make_unique<Input>())
{}

void Game::run()
{
  Uint64 previous = SDL_GetTicks();

  while (!input_->quitRequested()){
    Uint64 current = SDL_GetTicks();
    float deltaTime = static_cast<float>(current - previous) / 1000.0f;
    previous = current;

    SDL_Event event;

    while (SDL_PollEvent(&event)){
      input_->processEvent(event);
    }

    update(deltaTime);

    if (renderer_)
      SDL_SetWindowRelativeMouseMode(renderer_->window(), input_->mouseCaptured());

    render();

    input_->endFrame();
  }
}

void Game::update(float deltaTime)
{
  switch (state_){
    case GameState::Config:
      updateConfig(deltaTime);
      break;
    case GameState::Playing:
      updatePlaying(deltaTime);
      break;
    case GameState::Won:
      updateWon(deltaTime);
      break;
  }
}

void Game::render()
{
  switch (state_){
    case GameState::Config:
      renderConfig();
      break;
    case GameState::Playing:
      renderPlaying();
      break;
    case GameState::Won:
      renderWon();
      break;
  }
}

void Game::updateConfig(float deltaTime)
{
  startGame();
}

void Game::updatePlaying(float deltaTime)
{
  player_->update(*input_, deltaTime);

  if (player_->goalReached()){
    state_ = GameState::Won;
  }
}

void Game::updateWon(float deltaTime)
{
  std::cout << "CONGRASHUNZ!\nYOU ARE WINRAR!\n";
  input_->requestQuit();
}


void Game::renderConfig()
{

}

void Game::renderPlaying()
{
  renderer_->clear();
  renderer_->draw();
  renderer_->present();
}

void Game::renderWon()
{

}


void Game::startGame()
{
  MazeGenerator mg{config_.seed};

  maze_ = std::make_unique<Maze>(
    mg.generate(
      config_.mazeWidth,
      config_.mazeHeight
    )
  );

  MazeMeshBuilder meshBuilder;
  mesh_ = std::make_unique<Mesh>(meshBuilder.build(*maze_));

  player_ = std::make_unique<Player>(*maze_);

  renderer_ = std::make_unique<Renderer>(1280, 720, player_->camera());
  renderer_->uploadMesh(*mesh_);
  renderer_->setFaceColor(glm::vec3{0.5f, 0.5f, 0.5f});
  renderer_->setEdgeRendering(true);
  renderer_->setEdgeColor(glm::vec3{1.0f, 0.0f, 0.0f});

  mapVisible_ = false;
  state_ = GameState::Playing;
}
