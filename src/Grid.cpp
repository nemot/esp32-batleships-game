
#include "Grid.h"
#include <algorithm>
#include <Arduino.h>

Grid::Grid() {
  for(short i = 0; i < HEIGHT; i++) {
    std::vector<char> row = {};
    for(short j = 0; j < HEIGHT; j++) {
      row.push_back('.');
    }
    grid.push_back(row);
  }
}

std::vector<Coordinate> Grid::empty_cells() {
  std::vector<Coordinate> result;
  bool connected_cells_busy;
  for(short i = 0; i < HEIGHT; i++) {
    for(short j = 0; j < HEIGHT; j++) {
      connected_cells_busy =
        (i > 0 && i < HEIGHT-1 && (grid[i-1][j] == '#' || grid[i+1][j] == '#')) ||
        (j > 0 && j < WIDTH-1 && (grid[i][j-1] == '#' || grid[i][j+1] == '#')) ||
        (i > 0 && j > 0 && grid[i - 1][j - 1] == '#') ||
        (i > 0 && j < WIDTH - 1 && grid[i - 1][j + 1] == '#') ||
        (i < HEIGHT - 1 && j > 0 && grid[i + 1][j - 1] == '#') ||
        (i < HEIGHT - 1 && j < WIDTH - 1 && grid[i + 1][j + 1] == '#');  

      if(grid[i][j] == ' ' && !connected_cells_busy) {
        result.push_back({i,j});
      }
    }
  }
  return result;
}

bool Grid::place_figure(Coordinate point, Ship ship, bool simulate) {
  Ship position;
  std::for_each(ship.begin(), ship.end(), [this, &point, &position](Coordinate cell) {
    position.push_back({ cell[0]+point[0], cell[1]+point[1] });
  });

  auto available_cells = empty_cells();

  bool imposible = std::any_of(position.begin(), position.end(), [this, available_cells](Coordinate cell) {
    return available_cells.end() == std::find(available_cells.begin(),available_cells.end(), cell);
  });

  if(imposible) { return false; }
  if (simulate) { return true; }

  for(auto& cell : position) {
    grid[cell[0]][cell[1]] = '#';
  }

  return true;
}

void Grid::empty() {
  for(short i = 0; i < HEIGHT; i++) {
    for(short j = 0; j < HEIGHT; j++) {
      grid[i][j] = ' ';
    }
  }
}

void Grid::inspect() {
  const char row_names[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
  Serial.println("    1   2   3   4   5   6   7   8   9   10");
  Serial.println("  +---+---+---+---+---+---+---+---+---+---+");
  for(size_t rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
    Serial.print(row_names[rowIndex]);
    Serial.print(" |");
    for(char& c : grid[rowIndex]) {
      Serial.print(c);
      Serial.print(c);
      Serial.print(c);
      Serial.print("|");
    }
    Serial.println("");
    Serial.println("  +---+---+---+---+---+---+---+---+---+---+");
  }
  
}
