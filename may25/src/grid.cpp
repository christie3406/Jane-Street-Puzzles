#include "grid.hpp"
#include "clues.hpp"

size_t hash_row(const std::vector<Cell>& row) { 
    size_t hash = 0; 
    for (const auto& cell : row) { 
        hash = hash * 31 + cell.digit; 
        hash = hash * 31 + (cell.tile ? 1 : 0); 
    } return hash; 
} 

std::vector<int64_t> extract_numbers(const std::vector<Cell>& row) {
    std::vector<int64_t> numbers;
    int64_t current_number = 0;
    for (int i = 0; i < int(row.size()); ++i){
        if (i == 0 && row[i].tile) continue;
        else if (row[i].tile && current_number > 0) {
            numbers.push_back(current_number);
            current_number = 0;
        }
        else {
            current_number *= 10;
            current_number += unsigned(row[i].digit); 
        }
    }

    if (current_number > 0) numbers.push_back(current_number);
    return numbers;
}

void print_grid(const Grid& grid) {
    std::cout << "\n----------------------\n";

    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell.tile) {
                std::cout << "X ";
            } else if (cell.highlight) {
                std::cout << GREEN << cell.digit << RESET << " ";
            } else {
                std::cout << cell.digit << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "----------------------\n";

}

void set_up_grid(Grid& grid){

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            // initialise row, col 
            grid[i][j].row = i;
            grid[i][j].col = j;

            // initilise adjacent cells
            if (grid[i][j].row > 0) grid[i][j].adjacent.emplace_back(grid[i][j].row - 1, grid[i][j].col);  // Up
            if (grid[i][j].row < 10) grid[i][j].adjacent.emplace_back(grid[i][j].row + 1, grid[i][j].col); // Down
            if (grid[i][j].col > 0) grid[i][j].adjacent.emplace_back(grid[i][j].row, grid[i][j].col - 1);  // Left
            if (grid[i][j].col < 10) grid[i][j].adjacent.emplace_back(grid[i][j].row, grid[i][j].col + 1); // Right
        }

        // initialise highlighted cells 
        for (auto c : highlighted_coords) grid[c.first][c.second].highlight = true;
    }

}

bool validate_row(const Grid& grid, std::unordered_set<int64_t> used_numbers, int row){
    std::vector<int64_t> numbers = extract_numbers(grid[row]);
    
    // Check for duplicates in numbers
     std::unordered_set<int64_t> seen;
    for (int64_t num : numbers) {
        if (seen.count(num)) return false; // Duplicate found
        seen.insert(num);
    }

    // Does it satisfy the clue & does the number already exist in the grid 
    for (int64_t i : numbers) if (!functions[row](i) || used_numbers.count(i)) return false; 

    return true;
};

bool validate_row_left(const Grid& grid, std::unordered_set<int64_t> used_numbers, int row, int col){
    int right_edge = col + 1; 
    while (!grid[row][right_edge].tile && right_edge > 1 && right_edge < 10) --right_edge; // new number is formed when a right edge is encountered (tile or edge of grid)
    if (right_edge <= 1) return true; // no valid number in partial row -> automatically valid

    // construct the partial row
    std::vector<Cell> partial_row; 
    for (int i = 0; i < right_edge; ++i) partial_row.push_back(grid[row][i]);
    
    // extract numbers from partial row 
    std::vector<int64_t> numbers = extract_numbers(partial_row);

    // check for duplicates in numbers
     std::unordered_set<int64_t> seen;
    for (int64_t num : numbers) {
        if (seen.count(num)) return false; // Duplicate found
        seen.insert(num);
    }

    // does it satisfy the clue & does the number already exist in the grid 
    for (int64_t i : numbers) if (!functions[row](i) || used_numbers.count(i)) {
        return false; 
    }

    return true;
}
