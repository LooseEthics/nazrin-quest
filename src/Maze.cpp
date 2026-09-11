
#include <iostream>

#include "Maze.hpp"

Maze::Maze(int width, int height)
: width_(width),
  height_(height)
{
  cells_.reserve(width * height);
}

Maze::Maze(int width, int height, LogicMaze& logicMaze)
: width_(width),
  height_(height)
{
  cells_.reserve(width * height);
  for (int i = 0; i < logicMaze.size(); ++i){
    size_t logicX = i % logicMaze.width;
    size_t logicY = i / logicMaze.height;
    size_t cellX = logicX * 2 + 1;
    size_t cellY = logicY * 2 + 1;
    uint8_t data = logicMaze.data[i];
    if (data & 0x01) set(cellX + 1, cellY, Cell::Empty);
    if (data & 0x02) set(cellX, cellY + 1, Cell::Empty);
    if (data & 0x04) set(cellX - 1, cellY, Cell::Empty);
    if (data & 0x08) set(cellX, cellY - 1, Cell::Empty);
    if (data & 0x10) set(cellX, cellY, Cell::Empty);
  }
}

bool Maze::isInside(int x, int y) const
{
  return x >= 0 && x < width_ &&
    y >= 0 && y < height_;
}

Cell Maze::get(int x, int y) const
{
  return cells_[x + y * width_];
}

void Maze::set(int x, int y, Cell cell)
{
  cells_[x + y * width_] = cell;
}

int Maze::width() const noexcept
{
  return width_;
}

int Maze::height() const noexcept
{
  return height_;
}

void Maze::printMazeToConsole()
{
  for (int i = 0; i < width_ * height_; ++i){
    std::cout << (cells_[i] == Cell::Wall ? "█" : " ");
    if (i + 1 % width_ == 0) std::cout << std::endl;
  }
}
