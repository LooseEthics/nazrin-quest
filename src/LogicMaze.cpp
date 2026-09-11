
#include <iomanip>
#include <iostream>

#include "LogicMaze.hpp"

LogicMaze::LogicMaze(size_t width, size_t height)
  : width(width),
    height(height)
{
  data.assign(width * height, 0);
}

LogicMaze::LogicMaze()
{
  LogicMaze(1, 1);
}

size_t LogicMaze::size() const noexcept
{
  return (width * height);
}

void LogicMaze::printHexes() const
{
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<unsigned>(data[i])
            << ' ';

        if ((i + 1) % width == 0)
            std::cout << '\n';
    }

    std::cout << std::dec;
}
