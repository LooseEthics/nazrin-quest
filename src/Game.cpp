
#include <iostream>

#include "Game.hpp"
#include "MazeGenerator.hpp"
#include "MazeMeshBuilder.hpp"

Game::Game()
  : input_(std::make_unique<Input>()),
    renderer_(std::make_unique<Renderer>(1280, 720))
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
  if (input_->keyPressed(SDL_SCANCODE_UP)){
    --configField_;
  }
  if (input_->keyPressed(SDL_SCANCODE_DOWN)){
    ++configField_;
  }
  if (input_->keyPressed(SDL_SCANCODE_LEFT)){
    int decrement = 1;
    if (configField_ == ConfigField::Width || configField_ == ConfigField::Height)
      decrement = 2;
    if (input_->keyHeld(SDL_SCANCODE_LCTRL))
      decrement = 10;
    else if (input_->keyHeld(SDL_SCANCODE_LSHIFT))
      decrement = 100;
    changeConfigValue(configField_, -decrement);
  }
  if (input_->keyPressed(SDL_SCANCODE_RIGHT)){
    int increment = 1;
    if (configField_ == ConfigField::Width || configField_ == ConfigField::Height)
      increment = 2;
    if (input_->keyHeld(SDL_SCANCODE_LCTRL))
      increment = 10;
    else if (input_->keyHeld(SDL_SCANCODE_LSHIFT))
      increment = 100;
    changeConfigValue(configField_, increment);
  }
  if (input_->keyPressed(SDL_SCANCODE_RETURN))
    if (configField_ == ConfigField::Start)
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
  winTime_ += deltaTime;
  if (winTime_ > 5.0f)
    input_->requestQuit();
}


void Game::renderConfig()
{
  std::vector<std::string> configText{
    "Width: " + std::to_string(config_.mazeWidth),
    "Height: " + std::to_string(config_.mazeHeight),
    "Seed: " + std::to_string(config_.seed),
    "Start"
  };

  renderer_->clear();
  for (int i = 0; i < configText.size(); ++i){
    renderer_->drawText(
      configText[i],
      glm::vec2{100.0f, i * 100.0f},
      8.0f,
      configField_ == static_cast<ConfigField>(i) ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{1.0f, 1.0f, 1.0f}
    );
  }
  renderer_->present();
}

void Game::renderPlaying()
{
  renderer_->clear();
  if (!mapVisible_)
    renderer_->drawCamera(player_->camera());
  else
    renderer_->drawMap(player_->camera());
  renderer_->present();
}

void Game::renderWon()
{
  std::vector<std::string> winText{
    "CONGRASHUNZ!!!11!1!!1",
    "YOU ARE A WINRAR!"
  };

  renderer_->clear();
  for (int i = 0; i < winText.size(); ++i){
    renderer_->drawText(
      winText[i],
      glm::vec2{100.0f, i * 100.0f},
      8.0f,
      glm::vec3{1.0f, 1.0f, 1.0f}
    );
  }
  renderer_->present();
}

void Game::changeConfigValue(ConfigField field, int increment)
{
  switch (field){
    case ConfigField::Width:
      config_.mazeWidth = glm::clamp(
        config_.mazeWidth + increment,
        MAZE_MIN_SIZE,
        MAZE_MAX_SIZE
      );
      break;
    case ConfigField::Height:
      config_.mazeHeight = glm::clamp(
        config_.mazeHeight + increment,
        MAZE_MIN_SIZE,
        MAZE_MAX_SIZE
      );
      break;
    case ConfigField::Seed:
      config_.seed += increment;
      break;
    default:
      break;
  }
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

  renderer_->uploadMesh(*mesh_);
  renderer_->setGoalPosition(maze_->getGoalCoords());

  mapVisible_ = false;
  state_ = GameState::Playing;
}
