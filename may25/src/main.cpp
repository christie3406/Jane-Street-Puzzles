#include "grid.hpp"
#include "clues.hpp"

// Global memoization set 
std::unordered_set<std::pair<size_t, size_t>, PairHash> incompatible_pairs;

bool tile(Grid& grid, int row, int& max_row, std::unordered_set<int64_t> used_numbers, int index, int& count);

// Try incrementing the row given tile placement
bool increment(Grid& grid, std::vector<Cell*> tiles, int row, int& max_row, std::unordered_set<int64_t> used_numbers, int index, int& count) {
    // Finished incrementing all tiles in this row
    
    // 
    if (count > 60000) {
        return false; 
    }

    // All tiles incremeted
    if (index == int(tiles.size())) {

        if (row == 0) { // no previous row
            return tile(grid, row + 1, max_row, used_numbers, 0, count);
        }

        else if (validate_row(grid, used_numbers, row - 1)){ // if row > 0, check validity of previous row 
            if (row - 1 > max_row) max_row = row - 1; // maximum valid row 
            // print_grid(grid);
            ++count;

            std::vector<Cell> previous_row = grid[row - 1]; // construct previous row
            std::vector<int64_t> nums = extract_numbers(previous_row); // extract numbers from previous row
            for (auto num : nums) used_numbers.insert(num); // insert numbers from valid previous row 

            if (row < 10){ // check with memoization if row < 10 
                size_t prev_hash = hash_row(grid[row]);
                size_t curr_hash = hash_row(grid[row + 1]);
                if (incompatible_pairs.find({prev_hash, curr_hash}) != incompatible_pairs.end()) {
                    return false; // Known to fail
                }
                
                bool result = tile(grid, row + 1, max_row, used_numbers, 0, count); // recurse
                if (!result) {
                    incompatible_pairs.insert({prev_hash, curr_hash}); // Cache failure
                    for (auto num : nums) used_numbers.erase(num); // backtrack 
                }
                return result;
            }

            else { // if row == 10, can't memoize
                bool result = tile(grid, row + 1, max_row, used_numbers, 0, count); // recurse
                if (!result) for (auto num : nums) used_numbers.erase(num); // backtrack 
                return result; 
            }
        }
        return false;
    }

    Cell* current_tile = tiles[index];

    // Collect valid adjacent cells for distribution
    std::vector<std::pair<int, int>> valid_adjacent;
    for (auto& adj : current_tile->adjacent)
        if (!grid[adj.first][adj.second].tile && !grid[adj.first][adj.second].highlight)
            valid_adjacent.push_back(adj);

    // Nested backtracking function for distributing tile digit
    std::function<bool(int, int)> distribute = [&](int i, int remaining) {
        if (i == int(valid_adjacent.size())) {
            if (remaining != 0) return false; // Remaining not fully distributed

            // Check partial row validity
            if (row > 0 && !validate_row_left(grid, used_numbers, row - 1, current_tile->col))
                return false;

            // Recurse to next tile
            return increment(grid, tiles, row, max_row, used_numbers, index + 1, count);
        }

        auto& cell = grid[valid_adjacent[i].first][valid_adjacent[i].second];
        int original = cell.digit; // store original digit for backtracking

        for (int val = 0; val <= remaining && val + original <= 9; ++val) {
            cell.digit += val;
            if (distribute(i + 1, remaining - val)) return true; // recurse 
            cell.digit = original; // backtrack
        }
        return false;
    };

    // If there are no valid adjacent cells, just continue to next tile
    if (valid_adjacent.empty()) return increment(grid, tiles, row, max_row, used_numbers, index + 1, count);

    return distribute(0, current_tile->digit);
}

// Try tile placement
bool tile(Grid& grid, int row, int& max_row, std::unordered_set<int64_t> used_numbers, int index, int& count){

    // If recursion depth is too deep, the grid is likely to be false
    if (count > 60000) {
        return false; 
    }
    
    // grid is complete
    if (row > 10) {
        if (validate_row(grid, used_numbers, 10)) {
            print_grid(grid);
            return true; // solution found
        }
        return false; // last row is invalid
    } 

    if (index == grid[row].size()) { // tiling finished for row 
        std::vector<Cell*> tiles; // store tiles 
        for (auto& c : grid[row]) {
            if (c.tile) tiles.push_back(&c);
        }
        return increment(grid, tiles, row, max_row, used_numbers, 0, count); // increment the row 
    }
    
    // Option 1: Don't place tile
    if (tile(grid, row, max_row, used_numbers, index + 1, count)) return true; 

    // Option 2: Place tile (if allowed)
    Cell& current_cell = grid[row][index];

    bool can_place_tile = true; 

     // cell cannot be highlighted (constant)
    if (current_cell.highlight) can_place_tile = false;

    // cell cannot be one cell away from vertical edges (would produce a single digit number)
    else if (index == 1 || index == int(grid[row].size() - 2)) can_place_tile = false;

    // cell cannot be less than two cells away from last tiled cell (would produce a number with less than 2 digits)
    else if (index > 1 && (grid[row][index - 1].tile || grid[row][index - 2].tile)) can_place_tile = false;

    // cell above cannot be highlighted (constant)
    else if (row > 0 && grid[row - 1][index].tile) can_place_tile = false;

    if (can_place_tile){
        grid[row][index].tile = true; // set tile 
        if (tile(grid, row, max_row, used_numbers, index + 1, count)) return true; // proceed to next tile 
        grid[row][index].tile = false; // undo tile
    }

    return false;
}

// Find original digit assignment to regions
bool find_digits(Grid& grid, int region, Grid& last_grid, int& last_max_row){

    if (region > 8) { // region digit assignment complete

        // check if digits of rows 0 to last_max_row + 2 are same as that of the last grid- if so, grid is known to fail 
        bool no_change = true; 
        for (int i = 0; i <= last_max_row + 2; ++i){
            if (i > 10) break;
            if (extract_numbers(last_grid[i]).size() == 0) { last_grid = grid; continue; } 
            if (extract_numbers(last_grid[i])[0] != extract_numbers(grid[i])[0]){ no_change = false; break; } 
        }

        if (no_change) return false; // known to fail 

        last_max_row = -1;
        std::unordered_set<int64_t> used_numbers;
        incompatible_pairs.clear();
        int count = 0;

        if (tile(grid, 0, last_max_row, used_numbers, 0, count)) {
            return true;
        }

        else {
            last_grid = grid; 

            // output the failed digit assignment 
            std::cout << "Grid assignment: ";
            for (int i = 0; i < 9; ++i) std::cout<< grid[region_coords[i][0].first][region_coords[i][0].second].digit << " "; 
            std::cout << " - Grid didn't work, last max row was " << last_max_row << "\n";
            return false;
        }
    }

    // if region is already filled 
    if (grid[region_coords[region][0].first][region_coords[region][0].second].digit != 0) {
        return find_digits(grid, region + 1, last_grid, last_max_row);
    }


    for (int value = 1; value < 10; ++value) { 

        // check if value for this region is valid
        bool valid = true; 
        for (int dep : region_dependencies[region]) { 
            if (grid[region_coords[dep][0].first][region_coords[dep][0].second].digit == value) { 
                valid = false; 
                break; 
            } 
        } 
        if (valid) { // if value is valid, fill the region with value and proceed to next region
            for (auto c : region_coords[region]) grid[c.first][c.second].digit = value; 
            if (find_digits(grid, region + 1, last_grid, last_max_row)) return true;
        }
    } 

    for (auto c : region_coords[region]) grid[c.first][c.second].digit = 0; // backtrack
    return false;
}

int main() {
    Grid grid(11, std::vector<Cell>(11));
    set_up_grid(grid);
 
    Grid last = grid; 
    int last_max_row = -1;

    if (find_digits(grid, 0, last, last_max_row)) std::cout << "solution found";
    
    else std::cout << "no solution";  
 
    return 0;
}
