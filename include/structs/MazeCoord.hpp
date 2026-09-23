
#pragma once

struct MazeCoord
{
  int x;
  int y;

  bool operator==(const MazeCoord&) const = default;

  MazeCoord operator+(const MazeCoord& other) const
  {
    return {
      x + other.x,
      y + other.y
    };
  }
  bool operator<(const MazeCoord& other) const
  {
    if (y != other.y)
      return y < other.y;

    return x < other.x;
  }
};
