#pragma once

#include <vector>
#include <array>

using Coordinate = std::array<int, 2>;
using Ship = std::vector<Coordinate>;
static const char CELL_EMPTY = ' ';
static const char CELL_FULL = '#';
static const char CELL_UNKNOWN = '.';
static const char CELL_HIT = 'X';
static const char CELL_MISS = '+';