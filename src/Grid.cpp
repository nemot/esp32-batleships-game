
#include "Grid.h"
#include <algorithm>
#include <Arduino.h>

Grid::Grid() {
  for(short i = 0; i < HEIGHT; i++) {
    std::vector<char> row = {};
    for(short j = 0; j < HEIGHT; j++) {
      row.push_back(' ');
    }
    grid.push_back(row);
  }
}

void Grid::acknowledge_cell(Coordinate point, char value) {
  if(value == CELL_EMPTY) {
    grid[point[0]][point[1]] = value;
    return;
  }

  if(value == CELL_FULL) {
    for(auto& neighbour : connected_cells(point)) {
      if(neighbours_has_ship(neighbour)) {
        grid[neighbour[0]][neighbour[1]] = CELL_EMPTY;
      }
    }
    grid[point[0]][point[1]] = value;
  }
}

bool Grid::neighbours_has_ship(Coordinate point) {
  auto neighbours = connected_cells(point);
  return std::any_of(neighbours.begin(), neighbours.end(), [this](Coordinate p) {
    return grid[p[0]][p[1]] == CELL_FULL;
  });
}

std::vector<Coordinate> Grid::connected_cells(Coordinate point) {
  std::vector<Coordinate> result = {};

  if( point[0] > 0 ) { result.push_back({ point[0]-1, point[1]}); }
  if( point[0] < HEIGHT-1 ) { result.push_back({ point[0]+1, point[1]}); }
  if( point[1] > 0 ) { result.push_back({ point[0], point[1]-1}); }
  if( point[1] < WIDTH-1 ) { result.push_back({ point[0], point[1]+1}); }
  if( point[0] > 0 && point[1] > 0 ) { result.push_back({ point[0]-1, point[1]-1}); }
  if( point[0] > 0 && point[1] < WIDTH-1 ) { result.push_back({ point[0]-1, point[1]+1}); }
  if( point[0] < HEIGHT-1 && point[1] < WIDTH-1 ) { result.push_back({ point[0]+1, point[1]+1}); }
  if( point[0] < HEIGHT-1 && point[1] > 0 ) { result.push_back({ point[0]+1, point[1]-1}); }

  return result;
}

std::vector<Coordinate> Grid::empty_cells() {
  std::vector<Coordinate> result;
  for(short i = 0; i < HEIGHT; i++) {
    for(short j = 0; j < HEIGHT; j++) {
      if(grid[i][j] == CELL_EMPTY && !neighbours_has_ship({i,j})) { result.push_back({i,j}); }
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
    grid[cell[0]][cell[1]] = CELL_FULL;
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
      Serial.print(" ");
      Serial.print(c);
      Serial.print(" |");
    }
    Serial.println("");
    Serial.println("  +---+---+---+---+---+---+---+---+---+---+");
  }
  
}
