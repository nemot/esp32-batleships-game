#include <vector>
#include "types.h"

class Grid {
  public:
    Grid();
    std::vector<std::vector<char>> grid;
    static constexpr short HEIGHT = 10;
    static constexpr short WIDTH = 10;
    void inspect();
    void empty();
    bool place_figure(Coordinate point, Ship ship, bool simulate = false);
    void acknowledge_cell(Coordinate point, char value);
    bool neighbours_has_ship(Coordinate point);
    std::vector<Coordinate> connected_cells(Coordinate point);
    std::vector<Coordinate> empty_cells();
};