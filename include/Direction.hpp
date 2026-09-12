
#pragma once

constexpr uint8_t D_EAST  = 0x01;
constexpr uint8_t D_SOUTH = 0x02;
constexpr uint8_t D_WEST  = 0x04;
constexpr uint8_t D_NORTH = 0x08;

constexpr uint8_t C_BUILT = 0x10;
constexpr uint8_t C_START = 0x20;
constexpr uint8_t C_GOAL = 0x40;

enum class Direction : uint8_t
{
  East = 0,
  South = 1,
  West = 2,
  North = 3,
  Up = 4,
  Down = 5,
  Invalid = 6
};

constexpr bool isCardinal(Direction dir)
{
  return (
    dir == Direction::East ||
    dir == Direction::South ||
    dir == Direction::West ||
    dir == Direction::North
  );
}

constexpr Direction cardinalDirs[] = {
  Direction::East,
  Direction::South,
  Direction::West,
  Direction::North
};

constexpr uint8_t directionMask(Direction dir)
{
  if (isCardinal(dir)) return 1u << static_cast<uint8_t>(dir);
  return 0x0;
}

constexpr Direction opposite(Direction dir)
{
  switch (dir){
    case Direction::East: return Direction::West;
    case Direction::South: return Direction::North;
    case Direction::West: return Direction::East;
    case Direction::North: return Direction::South;
    case Direction::Up: return Direction::Up;
    case Direction::Down: return Direction::Down;
    default: return Direction::Invalid;
  }
}
