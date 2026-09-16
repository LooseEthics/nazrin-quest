
#pragma once

#include <array>
#include <cstdint>

constexpr int GLYPH_WIDTH = 5;
constexpr int GLYPH_HEIGHT = 7;

using Glyph = std::array<uint8_t, 7>;

const Glyph GLYPH_SPACE = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

const Glyph GLYPH_BLOCK = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

const Glyph GLYPH_A = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b10001,
  0b10001,
  0b10001
};

const Glyph GLYPH_B = {
  0b11100,
  0b10010,
  0b10001,
  0b11110,
  0b10010,
  0b10001,
  0b11110
};

const Glyph GLYPH_C = {
  0b00111,
  0b01000,
  0b10000,
  0b10000,
  0b10000,
  0b01000,
  0b00111
};

const Glyph GLYPH_D = {
  0b11100,
  0b10010,
  0b10001,
  0b10001,
  0b10001,
  0b10010,
  0b11100
};

const Glyph GLYPH_E = {
  0b11111,
  0b10000,
  0b10000,
  0b11111,
  0b10000,
  0b10000,
  0b11111
};

const Glyph GLYPH_F = {
  0b11111,
  0b10000,
  0b10000,
  0b11111,
  0b10000,
  0b10000,
  0b10000
};

const Glyph GLYPH_G = {
  0b00111,
  0b01000,
  0b10000,
  0b10001,
  0b10001,
  0b01001,
  0b00111
};

const Glyph GLYPH_H = {
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b10001,
  0b10001,
  0b10001
};

const Glyph GLYPH_I = {
  0b01110,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b01110
};

const Glyph GLYPH_J = {
  0b11111,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b10010,
  0b01100
};

const Glyph GLYPH_K = {
  0b10001,
  0b10010,
  0b10100,
  0b11000,
  0b10100,
  0b10010,
  0b10001
};

const Glyph GLYPH_L = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b11111
};

const Glyph GLYPH_M = {
  0b10001,
  0b11011,
  0b10101,
  0b10101,
  0b10001,
  0b10001,
  0b10001
};

const Glyph GLYPH_N = {
  0b10001,
  0b11001,
  0b10101,
  0b10101,
  0b10101,
  0b10011,
  0b10001
};

const Glyph GLYPH_O = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

const Glyph GLYPH_P = {
  0b11110,
  0b10001,
  0b10001,
  0b11110,
  0b10000,
  0b10000,
  0b10000
};

const Glyph GLYPH_Q = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b10101,
  0b10011,
  0b01111
};

const Glyph GLYPH_R = {
  0b11110,
  0b10001,
  0b10001,
  0b11110,
  0b10100,
  0b10010,
  0b10001
};

const Glyph GLYPH_S = {
  0b01110,
  0b10001,
  0b01000,
  0b00100,
  0b00010,
  0b10001,
  0b01110
};

const Glyph GLYPH_T = {
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

const Glyph GLYPH_U = {
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

const Glyph GLYPH_V = {
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b01010,
  0b00100
};

const Glyph GLYPH_W = {
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10101,
  0b10101,
  0b01010
};

const Glyph GLYPH_X = {
  0b10001,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b10001
};

const Glyph GLYPH_Y = {
  0b10001,
  0b10001,
  0b01010,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

const Glyph GLYPH_Z = {
  0b11111,
  0b00010,
  0b00100,
  0b00100,
  0b00100,
  0b01000,
  0b11111
};

const Glyph GLYPH_ZERO = {
  0b01110,
  0b10011,
  0b10101,
  0b10101,
  0b10101,
  0b11001,
  0b01110
};

const Glyph GLYPH_ONE = {
  0b00100,
  0b01100,
  0b10100,
  0b00100,
  0b00100,
  0b00100,
  0b01110
};

const Glyph GLYPH_TWO = {
  0b01110,
  0b10001,
  0b10010,
  0b00100,
  0b01000,
  0b01000,
  0b11111
};

const Glyph GLYPH_THREE = {
  0b01110,
  0b10001,
  0b00001,
  0b00110,
  0b00001,
  0b10001,
  0b01110
};

const Glyph GLYPH_FOUR = {
  0b00010,
  0b00110,
  0b01010,
  0b10010,
  0b11111,
  0b00010,
  0b00010
};

const Glyph GLYPH_FIVE = {
  0b11111,
  0b10000,
  0b11100,
  0b00010,
  0b00001,
  0b10001,
  0b01110
};

const Glyph GLYPH_SIX = {
  0b00111,
  0b01000,
  0b10000,
  0b11110,
  0b10001,
  0b10001,
  0b01110
};

const Glyph GLYPH_SEVEN = {
  0b11111,
  0b00010,
  0b00100,
  0b01110,
  0b00100,
  0b01000,
  0b10000
};

const Glyph GLYPH_EIGHT = {
  0b01110,
  0b10001,
  0b10001,
  0b01110,
  0b10001,
  0b10001,
  0b01110
};

const Glyph GLYPH_NINE = {
  0b01110,
  0b10001,
  0b10001,
  0b01111,
  0b00001,
  0b00010,
  0b11100
};

const Glyph GLYPH_EXCLAMATION = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b00100
};

constexpr const Glyph getGlyph(char c)
{
  switch (c){
    case 'A':
    case 'a':
      return GLYPH_A;
    case 'B':
    case 'b':
      return GLYPH_B;
    case 'C':
    case 'c':
      return GLYPH_C;
    case 'D':
    case 'd':
      return GLYPH_D;
    case 'E':
    case 'e':
      return GLYPH_E;
    case 'F':
    case 'f':
      return GLYPH_F;
    case 'G':
    case 'g':
      return GLYPH_G;
    case 'H':
    case 'h':
      return GLYPH_H;
    case 'I':
    case 'i':
      return GLYPH_I;
    case 'J':
    case 'j':
      return GLYPH_J;
    case 'K':
    case 'k':
      return GLYPH_K;
    case 'L':
    case 'l':
      return GLYPH_L;
    case 'M':
    case 'm':
      return GLYPH_M;
    case 'N':
    case 'n':
      return GLYPH_N;
    case 'O':
    case 'o':
      return GLYPH_O;
    case 'P':
    case 'p':
      return GLYPH_P;
    case 'Q':
    case 'q':
      return GLYPH_Q;
    case 'R':
    case 'r':
      return GLYPH_R;
    case 'S':
    case 's':
      return GLYPH_S;
    case 'T':
    case 't':
      return GLYPH_T;
    case 'U':
    case 'u':
      return GLYPH_U;
    case 'V':
    case 'v':
      return GLYPH_V;
    case 'W':
    case 'w':
      return GLYPH_W;
    case 'X':
    case 'x':
      return GLYPH_X;
    case 'Y':
    case 'y':
      return GLYPH_Y;
    case 'Z':
    case 'z':
      return GLYPH_Z;
    case '0':
      return GLYPH_ZERO;
    case '1':
      return GLYPH_ONE;
    case '2':
      return GLYPH_TWO;
    case '3':
      return GLYPH_THREE;
    case '4':
      return GLYPH_FOUR;
    case '5':
      return GLYPH_FIVE;
    case '6':
      return GLYPH_SIX;
    case '7':
      return GLYPH_SEVEN;
    case '8':
      return GLYPH_EIGHT;
    case '9':
      return GLYPH_NINE;
    case ' ':
      return GLYPH_SPACE;
    case '!':
      return GLYPH_EXCLAMATION;
    default:
      return GLYPH_BLOCK;
  }
}
