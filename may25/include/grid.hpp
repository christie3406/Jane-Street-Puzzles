#pragma once
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdint>
#include <unordered_set>

struct Cell {
    int digit = 0;  // Stores the cell's digit
    bool highlight = false; // Highlighted cells-> no tiles, no increments
    bool tile = false; 
    std::vector<std::pair<int, int>> adjacent;  // Stores orthogonal neighbors
    int row = 0; int col = 0; 
    // Constructor
    Cell(int val = 0) : digit(val) {} 
};

struct PairHash { 
    size_t operator()(const std::pair<size_t, size_t>& p) const { 
        return p.first ^ (p.second << 16); 
    } 
};

// --------------------------------------------------------------------------------------------------------//
using Grid = std::vector<std::vector<Cell>>;

// Coordinates of cells in each region 
const std::vector<std::vector<std::pair<int,int>>> region_coords = {
     // region 0
    { {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10},
      {1,0}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9}, {1,10}, {2,9} },

    // region 1
    { {1,1}, {2,0}, {2,1}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {5,1}, {5,2}, {5,3}, {5,4},
      {3,3}, {4,3}, {6,4}, {6,5} },

    // region 2
    { {2,2}, {2,3}, {2,4}, {2,5}, {3,1}, {3,2}, {3,4}, {4,1}, {4,2}, {4,4} },

    // region 3
    { {2,6}, {2,7}, {2,8}, {3,6}, {3,7}, {3,8}, {4,7}, {4,8}, {3,9}, 
      {2,10}, {3,10}, {4,10}, {5,10} },

    // region 4
    { {3,5}, {4,5}, {4,6}, {4,9}, {5,5}, {5,6}, {5,7}, {5,8}, {5,9},
      {6,6}, {6,7}, {6,9}, {6,10} },

    // region 5
    { {6,1}, {7,1}, {8,0}, {8,1}, {8,2}, {8,3}, {8,5}, 
      {9,0}, {9,1}, {9,2}, {9,3}, {9,4}, {9,5}, {9,6}, {9,7}, {9,8}, {9,9}, {9,10},
      {10,0}, {10,1}, {10,8}, {10,9}, {10,10} },

    // region 6
    { {6,2}, {6,3}, {6,8}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}, {7,7}, {7,8}, {8,4}, {8,6} },

    // region 7
    { {7,9}, {7,10}, {8,7}, {8,8}, {8,9}, {8,10} },
    
    // region 8
    { {10,2}, {10,3}, {10,4}, {10,5}, {10,6}, {10,7} }
};

// Store highlighted (constant) cells as coordinates
const std::vector<std::pair<int,int>> highlighted_coords = {
    {1,3}, {1,4}, {2,4}, {2,9}, {3,8}, {3,9}, {5,5},
    {6,1}, {6,2}, {6,5}, {6,6}, {7,1}, {7,5}, {8,4}, {8,5}, {9,4}
};

// Region constraints: region A cannot equal region B (adjacent regions cannot have same digit)
const std::vector<std::vector<int>> region_dependencies = { 
    {}, // Region 0: no dependencies 
    {0}, // Region 1: ≠ Region 0 
    {0, 1}, // Region 2: ≠ Region 0 or 1 
    {0, 2}, // Region 3: ≠ Region 0 or 2 
    {1, 2, 3}, // Region 4: ≠ Region 1, 2, or 3 
    {1}, // Region 5: ≠ Region 1 
    {1, 4, 5}, // Region 6: ≠ Region 1, 4, or 5 
    {4, 5, 6}, // Region 7: ≠ Region 4, 5 or 6 
    {5} // Region 8: ≠ Region 5 
};


// --------------------------------------------------------------------------------------------------------//

// Hash row of grid for memoization
size_t hash_row(const std::vector<Cell>& row);

// Extract numbers from row, ignoring tiles
std::vector<int64_t> extract_numbers(const std::vector<Cell>& row);

// Print grid with highlights as green and tiles as X
const std::string GREEN = "\033[32m";
const std::string RESET = "\033[0m";
void print_grid(const Grid& grid);

// Set up grid (adjacent cells, highlighted cells, row and col)
void set_up_grid(Grid& grid);

// Do the numbers in the row satisfy the clue? Are there duplicate numbers? 
bool validate_row(const Grid& grid, std::unordered_set<int64_t> used_numbers, int row);

// Are indexes [0,..,col] of row valid? 
bool validate_row_left(const Grid& grid, std::unordered_set<int64_t> used_numbers, int row, int col);