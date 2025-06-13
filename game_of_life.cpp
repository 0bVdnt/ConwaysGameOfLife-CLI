#include <chrono> // For std::chrono::milliseconds
#include <cstddef>
#include <cstdlib> // For system()
#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include <vector>

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  // Assume POSIX for Linux, macOS, etc.
  system("clear");
#endif
}

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

  bool isAlive(size_t x, size_t y) {
    if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size())
      return grid[x][y];
    else {
      std::cerr << "tried accessing region outside the grid.";
      exit(1);
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

  void makeDead(size_t x, size_t y) {
    if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size())
      grid[x][y] = false;
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

        int nbr_x = x + i;
        int nbr_y = y + j;

        if (nbr_x >= 0 && nbr_x < r && nbr_y >= 0 && nbr_y < c)
          if (grid[nbr_x][nbr_y] == true)
            liveNbrCnt++;
      }
    return liveNbrCnt;
  }
};

int main() {
  const int rows = 20, cols = 20;
  Grid grid(rows, cols);
  grid.makeAlive(1, 2);
  grid.makeAlive(2, 3);
  grid.makeAlive(3, 1);
  grid.makeAlive(3, 2);
  grid.makeAlive(3, 3);

  while (1) {
    clearScreen();
    std::cout << "Conway's Game of Life (Press Ctrl+C to exit)" << std::endl;
    grid.printGrid();
    // A grid for the next generation
    Grid nextGrid(grid);
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        int liveNeighbors = grid.countAliveNeighbors(r, c);
        // Apply the rules of life
        if (grid.isAlive(r, c)) {
          // A living cell dies if it has < 2 or > 3 neighbors
          if (liveNeighbors < 2 || liveNeighbors > 3)
            nextGrid.makeDead(r, c);
        } else {
          // A dead cell becomes alive if it has exactly 3 neighbors
          if (liveNeighbors == 3) {
            nextGrid.makeAlive(r, c);
          }
        }
      }
    }
    grid = nextGrid;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  return 0;
}
