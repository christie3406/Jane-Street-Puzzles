#include <iostream>
#include <vector>
#include <utility> // for pair
#include <algorithm> // for min
#include <iomanip> // for setw
#include "Tilings.h"
using namespace std;

class GridState {
public:
    vector<vector<bool>> grid; // 45x45 grid, true = occupied
    vector<vector<pair<int, int>>> tile_coords; // tile_coords[0] = 1x1 tiles, etc.

    GridState() : grid(45, vector<bool>(45, false)), tile_coords(9) {} // 9 vectors for 1x1 to 9x9 tiles

    // Function to place a tile
    bool place_tile(int tile_size, int x, int y) {
        // Check if we've already placed all tiles of this size
        if (tile_coords[tile_size - 1].size() >= tile_size) return false;

        // Check if tile would go out of bounds
        if (x + tile_size > 45 || y + tile_size > 45) return false;

        // Check for occupied cells in the tile area
        for (int i = x; i < x + tile_size; ++i) {
            for (int j = y; j < y + tile_size; ++j) {
                if (grid[i][j]) return false;
            }
        }

        // Place tile
        for (int i = x; i < x + tile_size; ++i) {
            for (int j = y; j < y + tile_size; ++j) {
                grid[i][j] = true;
            }
        }

        tile_coords[tile_size - 1].emplace_back(x, y);
        return true;
    }

    // Function to find first empty cell (top-left to bottom-right)
    pair<int, int> find_first_empty() const {
        for (int y = 0; y < 45; ++y) {
            for (int x = 0; x < 45; ++x) {
                if (!grid[x][y]) {
                    return {x, y};
                }
            }
        }
        return {-1, -1}; // no empty cell found
    }

};

// Backtracking solver function
bool solve_puzzle(GridState& state) {

    // Find first empty cell
    auto [x, y] = state.find_first_empty();

    // No more empty cells
    if (x == -1 && y == -1) return true; 

    // Try placing each possible tile size (from largest to smallest for efficiency)
    for (int tile_size = 9; tile_size >= 1; --tile_size) {
        // Make copy of the current state for backtracking
        GridState new_state = state;

        // Try to place the tile
        if (new_state.place_tile(tile_size, x, y)) {
            // Recursively try to solve the new state
            if (solve_puzzle(new_state)) {
                // If solved, update the original state and return
                state = new_state;
                return true;
            }
        }
    }

    // No solution found from this state
    return false;
}

// Helper function to print tile coordinates for debugging
void print_tile_coords(const GridState& state) {
    for (int size = 1; size <= 9; ++size) {
        cout << size << "x" << size << " tiles (" 
             << state.tile_coords[size-1].size() << "/" << size << "):" << endl;
        for (const auto& coord : state.tile_coords[size-1]) {
            cout << "  (" << coord.first << ", " << coord.second << ")" << endl;
        }
    }
}

// Print grind 
void print_grid(const GridState& state) {
    // Create a 45x45 grid to store tile sizes
    std::vector<std::vector<int>> display_grid(45, std::vector<int>(45, 0));
    
    // Fill the display grid with tile sizes
    for (int size = 1; size <= 9; ++size) {
        for (const auto& coord : state.tile_coords[size-1]) {
            int x = coord.first;
            int y = coord.second;
            for (int i = x; i < x + size; ++i) {
                for (int j = y; j < y + size; ++j) {
                    display_grid[i][j] = size;
                }
            }
        }
    }

    // Print top axis (column numbers)
    std::cout << "  "; // Space for row numbers
    for (int x = 0; x < 45; ++x) {
        std::cout << std::setw(2) << x % 10; // Only show last digit for compactness
    }
    std::cout << "\n";

    // Print grid with left axis (row numbers)
    for (int y = 0; y < 45; ++y) {
        // Print row number
        std::cout << std::setw(2) << y << " ";
        
        for (int x = 0; x < 45; ++x) {
            int size = display_grid[x][y];
            if (size == 0) {
                std::cout << "  ";
            } else {
                // Set color based on tile size
                switch(size) {
                    case 1: std::cout << "\033[38;2;171;71;188m"; break;  // pink
                    case 2: std::cout << "\033[38;2;124;179;66m"; break;   // green
                    case 3: std::cout << "\033[38;2;255;143;0m"; break;    // orange
                    case 4: std::cout << "\033[38;2;57;73;171m"; break;    // dark-blue
                    case 5: std::cout << "\033[38;2;255;0;0m"; break;    // red
                    case 6: std::cout << "\033[38;2;3;155;229m"; break;    // light-blue
                    case 7: std::cout << "\033[38;2;255;235;59m"; break;   // yellow
                    case 8: std::cout << "\033[38;2;109;76;65m"; break;    // brown
                    case 9: std::cout << "\033[38;2;178;235;242m"; break;  // transparent
                }
                std::cout << "X\033[0m "; // Reset color after X
            }
        }
        std::cout << "\n";
    }
}

int main() {
    GridState state;

    for (int g = 0; g < 9; ++g){
        std::vector<std::vector<std::pair<int, int>>> initial_tiling = all_initial_tilings[g];

        // Place initial tiles
        for (int i = 0; i < int(initial_tiling.size()); ++i) {
            for (const auto& coord : initial_tiling[i]) {
                state.place_tile(i + 1, coord.first, coord.second);
            }
        }

        // Print initial grid
        std::cout << "Initial tiling of grid " << g + 1 << ": \n";
        print_grid(state);

        // Print solved grid
        if (solve_puzzle(state)) {
            cout << "\n Solved tiling grid " << g + 1 << ": \n";
            print_grid(state);
            std::cout << "\n";
        } else {
            cout << "No solution found for grid " << g + 1 << "\n";
        } 

    }

    return 0;
}