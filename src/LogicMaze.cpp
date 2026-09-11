
#include "LogicMaze.hpp"

LogicMaze::LogicMaze(size_t width, size_t height)
  : width(width),
    height(height)
{
  data.assign(0, width * height);
}

LogicMaze::LogicMaze()
{
  LogicMaze(1, 1);
}

size_t LogicMaze::size() const noexcept
{
  return (width * height);
}
