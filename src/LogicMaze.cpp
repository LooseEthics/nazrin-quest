
#include <iomanip>
#include <iostream>

#include "LogicMaze.hpp"

LogicMaze::LogicMaze(size_t width, size_t height)
  : width(width),
    height(height),
    unbuiltCells(width * height)
{
  data.assign(width * height, 0);
}

LogicMaze::LogicMaze()
  : LogicMaze(1, 1)
{}

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

size_t LogicMaze::getNextUnbuilt(size_t offset) const noexcept
{
  // std::cout << "getNextUnbuilt offset " << offset << "\n";
  // std::cout << "getNextUnbuilt size " << size() << "\n";
  if (unbuiltCells == 0) return size();
  offset = offset % unbuiltCells;
  size_t index = 0;
  for (; offset > 0; ++index){
    // std::cout << index << ":" << offset << "\n";
    if (index >= size()) index = 0;
    if ((data[index] & C_BUILT) == 0) offset -= 1;
  }
  // std::cout << "getNextUnbuilt index " << index << "\n";
  // std::cout << "getNextUnbuilt data " << data[index] << "\n";
  for (;; ++index){
    if (index >= size()) index = 0;
    if ((data[index] & C_BUILT) == 0) break;
  }
  // std::cout << "getNextUnbuilt index " << index << "\n";
  return index;
}
