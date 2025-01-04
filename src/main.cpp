#include <Arduino.h>
#include <vector>
#include <esp_system.h>
#include "types.h"
#include "Grid.h"
#include "ships.h"

Grid my_grid;
Grid opponents_grid;

void place_random_ship(std::vector<Ship>& ships, short quantity ) {
  Ship random_ship;
  Coordinate random_coordinate;
  short ships_placed = 0;
  auto empty_coordinates = my_grid.empty_cells();
  
  while(ships_placed < quantity) {
    random_coordinate = empty_coordinates[esp_random() % empty_coordinates.size()];
    random_ship = ships[esp_random() % ships.size()];
    if(my_grid.place_figure(random_coordinate, random_ship)) { ships_placed ++; }
  }
}


void setup() {
  Serial.begin(9600);

  my_grid.empty();
  place_random_ship(four_cells_ships, 1);
  place_random_ship(three_cells_ships, 2);
  place_random_ship(two_cells_ships, 3);
  place_random_ship(one_cells_ships, 4);

  
  my_grid.inspect();


}

void loop() {
}
