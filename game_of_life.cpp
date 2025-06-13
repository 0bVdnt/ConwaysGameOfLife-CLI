#include <cstddef>
#include <iostream>
#include <vector>

class Grid {
private:
  std::vector<std::vector<bool>> grid;

public:
  Grid() : grid(std::vector<std::vector<bool>>()) {}
  Grid(size_t _rows, size_t _cols)
      : grid(std::vector<std::vector<bool>>(_rows,
                                            std::vector<bool>(_cols, false))) {}
  void printGrid() {
    for (const std::vector<bool> &row : grid) {
      for (const bool &cell : row)
        // '*' represents a live cell while '.' represents a dead cell.
        std::cout << (cell ? '*' : '.') << ' ';
      std::cout << std::endl;
    }
  }
  void makeAlive(size_t x, size_t y) {
    if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size())
      grid[x][y] = true;
    else {
      std::cerr << "tried accessing region outside the grid.";
      exit(1);
    }
  }
  int countAliveNeighbors(size_t x, size_t y) {
    int liveNbrCnt = 0;
    int r = grid.size();
    if (r == 0)
      return 0;
    int c = grid[0].size();
    // Iterate on all the cells around the current cell.
    // i and j are offsets
    for (int i = -1; i <= 1; ++i)
      for (int j = -1; j <= 1; ++j) {
        if (i == 0 && j == 0) // Skip the cell itself
          continue;

        int nbr_r = x + i;
        int nbr_c = y + j;

        if (nbr_r >= 0 && nbr_r < r && nbr_c >= 0 && nbr_c < c)
          if (grid[nbr_r][nbr_c] == true)
            liveNbrCnt++;
      }
    return liveNbrCnt;
  }
};

int main() {
  Grid g(10, 10);
  g.makeAlive(4, 4);
  g.makeAlive(4, 5);
  g.makeAlive(5, 4);
  g.makeAlive(5, 5);
  g.printGrid();
  std::cout << "\n--- Neighbor Count Tests ---" << std::endl;

  // Test 1: A cell inside the block, e.g., at (4, 4).
  // It should have 3 live neighbors: (4,5), (5,4), and (5,5).
  std::cout << "Neighbors for cell (4, 4): " << g.countAliveNeighbors(4, 4)
            << std::endl;

  // Test 2: A cell adjacent to the block, e.g., at (3, 4).
  // It should have 2 live neighbors: (4,4) and (4,5).
  std::cout << "Neighbors for cell (3, 4): " << g.countAliveNeighbors(3, 4)
            << std::endl;

  // Test 3: A cell in the corner of the grid, e.g., at (0, 0).
  // It has no live neighbors.
  std::cout << "Neighbors for cell (0, 0): " << g.countAliveNeighbors(0, 0)
            << std::endl;
  return 0;
}
