#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <optional>
#include <fstream>
#include <numeric>

constexpr char _clear_tile_ {'.'};

constexpr char _guard_up_ {'^'};
constexpr char _guard_left_ {'<'};
constexpr char _guard_right_ {'>'};
constexpr char _guard_down_ {'v'};

constexpr std::array<char,4> _guard_symbols_ = {_guard_up_, _guard_left_, _guard_right_, _guard_down_};

constexpr char _obstruction_ {'#'};
constexpr char _visited_tile_ {'X'};

constexpr char _my_obstruction_{'#'};

enum class orientation {
	up,
	right,
	down,
	left 
};

struct starting_pos_and_orientation {
	int row;
	int col;
	orientation ori;
};

void print_map(const std::vector<std::string>& map){
	std::cout << "-----map------" << '\n';
	for (const auto& row: map){
		std::cout << row << std::endl;
	}
}

enum class result {
	incremented,
	ended,
	loop
};

bool have_i_looped(const int row, const int col, const orientation ori, const starting_pos_and_orientation &spao) {
	const bool ret = (spao.row == row && spao.col == col && spao.ori == ori);
	if (ret) {
		std::cout << "you have looped!" << std::endl;
	}
	return ret;
}

result increment_guard_position(std::vector<std::string> &map, const starting_pos_and_orientation &spao, bool* starting) {
	std::cout << ((*starting) ? "starting" : "not starting") << std::endl;
	const int row_size = static_cast<int>(map.size());
	const int col_size = static_cast<int>(map[0].size());
	for (int row_idx = 0; row_idx != row_size; ++row_idx) {
		for (int col_idx = 0; col_idx != col_size; ++col_idx) {
			const auto symbol = map[row_idx][col_idx];
			const auto guard_symbol_it = std::find(_guard_symbols_.cbegin(), _guard_symbols_.cend(), symbol);
			if (guard_symbol_it != _guard_symbols_.cend()) {
				const char guard_symb = *guard_symbol_it;
				std:: cout << "guard_symb: " << guard_symb << std::endl;
				switch(guard_symb) {
					case _guard_up_:
						{
							std::cout << "guard up at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_row_idx = static_cast<int>(row_idx) - 1;
							if (new_row_idx >= 0 && new_row_idx < row_size) { 
								const char next = map[new_row_idx][col_idx];
								std::cout << "next " << next << " at (" << new_row_idx << "," << col_idx << std::endl;

								// loop can mean: we turned last time and are now at starting pos and orientation
								// OR: we are moving into starting pos now
								// but it should not be our starting pos.
								if(
										!*starting 
										&& (have_i_looped(row_idx, col_idx, orientation::up, spao) 
										|| have_i_looped(new_row_idx, col_idx, orientation::up, spao)
										)
										) 
								{
									return result::loop;
								}

								if (next != _obstruction_){
										// move
										std::cout << "=> clear tile" << std::endl;
										map[new_row_idx][col_idx] = _guard_up_;
										map[row_idx][col_idx] = _visited_tile_;
								}
								if (next == _obstruction_) {
									// turn right, don't move
									std::cout << "obstruction" << std::endl;		
									map[row_idx][col_idx] = _guard_right_;
								}
								return result::incremented;
							}
							return result::ended;
						}
					case _guard_left_:
						{
							std::cout << "guard left at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_col_idx = static_cast<int>(col_idx) - 1;
							if (new_col_idx >= 0 && new_col_idx < col_size) { 
								const char next = map[row_idx][new_col_idx];
								std::cout << "next " << next << " at (" << row_idx << "," << new_col_idx << std::endl;

								if(
										!*starting 
										&& (have_i_looped(row_idx, col_idx, orientation::left, spao) 
										|| have_i_looped(row_idx, new_col_idx, orientation::left, spao))
										) 
								{
									return result::loop;
								}

								if (next != _obstruction_){
										std::cout << "=> clear tile" << std::endl;
										map[row_idx][new_col_idx] = _guard_left_;
										map[row_idx][col_idx] = _visited_tile_;
								}
								if (next == _obstruction_) {
									// turn right, don't move
									std::cout << "obstruction" << std::endl;		
									map[row_idx][col_idx] = _guard_up_;
								}
								return result::incremented;
							}
							return result::ended;
						}
					case _guard_right_:
						{
							std::cout << "guard right at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_col_idx = static_cast<int>(col_idx) + 1;
							if (new_col_idx >= 0 && new_col_idx < col_size) { 
								const char next = map[row_idx][new_col_idx];
								std::cout << "next " << next << " at (" << row_idx << "," << new_col_idx << std::endl;

								if(
										!*starting &&
										(have_i_looped(row_idx, col_idx, orientation::right, spao) 
										|| have_i_looped(row_idx, new_col_idx, orientation::right, spao))
										) 
								{
									return result::loop;
								}

								if (next != _obstruction_){
										std::cout << "=> clear tile" << std::endl;
										map[row_idx][new_col_idx] = _guard_right_;
										map[row_idx][col_idx] = _visited_tile_;
								}
								if (next == _obstruction_) {
									// turn right, don't move
									std::cout << "obstruction" << std::endl;		
									map[row_idx][col_idx] = _guard_down_;
								}
								return result::incremented;
							}
							return result::ended;
						}
					case _guard_down_:
						{
							std::cout << "guard down at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_row_idx = static_cast<int>(row_idx) + 1;
							if (new_row_idx >= 0 && new_row_idx < row_size) { 
								const char next = map[new_row_idx][col_idx];
								std::cout << "next " << next << " at (" << new_row_idx << "," << col_idx << std::endl;

								if(
										!*starting &&
										(have_i_looped(row_idx, col_idx, orientation::down, spao) 
										|| have_i_looped(new_row_idx, col_idx, orientation::down, spao))
										) 
								{
									return result::loop;
								}

								if (next != _obstruction_){
										std::cout << "=> clear tile" << std::endl;
										map[new_row_idx][col_idx] = _guard_down_;
										map[row_idx][col_idx] = _visited_tile_;
								}
								if (next == _obstruction_) {
									// turn right, don't move
									std::cout << "obstruction" << std::endl;		
									map[row_idx][col_idx] = _guard_left_;
								}
								return result::incremented;
							}
							return result::ended;
						}
					default:
						std::cout << "wtf." << std::flush;
				}
			}
		}
	}
	std::terminate();
}


starting_pos_and_orientation get_starting_pos_and_orientation(const std::vector<std::string> initial_map) {
	starting_pos_and_orientation spao;
	const int row_size = static_cast<int>(initial_map.size());
	const int col_size = static_cast<int>(initial_map[0].size());
	for (int row_idx = 0; row_idx != row_size; ++row_idx) {
		for (int col_idx = 0; col_idx != col_size; ++col_idx) {
			const auto symbol = initial_map[row_idx][col_idx];
			const auto guard_symbol_it = std::find(_guard_symbols_.cbegin(), _guard_symbols_.cend(), symbol);
			if (guard_symbol_it != _guard_symbols_.cend()) {
				spao.row = row_idx;
				spao.col = col_idx;
				const char guard_symb = *guard_symbol_it;
				switch(guard_symb) {
					case _guard_up_:
						{							
							spao.ori = orientation::up;	
							break;
						}
					case _guard_down_:
						{							
							spao.ori = orientation::down;	
							break;
						}
					case _guard_left_:
						{							
							spao.ori = orientation::left;	
							break;
						}
					case _guard_right_:
						{							
							spao.ori = orientation::right;	
							break;
						}
				}
			}
		}
	}
	return spao;
}

int main() {
	/* std::vector<std::string> map; */
	/* std::string line; */
	/* std::ifstream file("input.txt"); */

	/* if (file.is_open()) { */
	/* 	while (std::getline(file, line)) { */
	/* 			map.push_back(line); */
	/* 	} */
	/* 	file.close(); */
	/* } else { */
	/* 	std::cerr << "Unable to open file" << std::endl; */
	/* 	return 1; */
	/* } */

	std::vector<std::string> map {
		"....#.....",
		".........#",
		"..........",
		"..#.......",
		".......#..",
		"..........",
		".#..^.....",
		"........#.",
		"#.........",
		"......#...",
	};

	std::vector<std::vector<std::string>> maps_with_extra_obstructions;
	const int row_size = static_cast<int>(map.size());
	const int col_size = static_cast<int>(map[0].size());
	for (int row_idx = 0; row_idx != row_size; ++row_idx) {
		for (int col_idx = 0; col_idx != col_size; ++col_idx) {
			if (map[row_idx][col_idx] == _clear_tile_) {
				std::cout << "generating map with obstruction at (" << row_idx << ", " << col_idx << std::endl;
				std::vector<std::string> map_with_obstruction = map;
				map_with_obstruction[row_idx][col_idx] = _my_obstruction_;
				maps_with_extra_obstructions.push_back(map_with_obstruction);
			}
		}
	}
	
	int num_loop_positions {0};
	for (auto mod_map: maps_with_extra_obstructions) {
		const auto spao = get_starting_pos_and_orientation(mod_map);
		print_map(mod_map);
		auto result = result::incremented;
		bool starting = true;
		while (result == result::incremented) {
			result = increment_guard_position(mod_map, spao, &starting);
			starting = false;
			print_map(mod_map);
		}
		if (result == result::loop) {
			num_loop_positions++;
			std::cout << "loops so far: " << num_loop_positions << std::endl;
		}
	}
	std::cout << "num_loop_positions: " << num_loop_positions << std::endl;

	/* print_map(map); */

	const int num_positions_visited = std::accumulate(
		map.cbegin(), 
		map.cend(), 
		1,
		[&](int sum, const std::string &col) -> int { 
			return sum + std::count(col.cbegin(), col.cend(), _visited_tile_);	
		}
	);

	/* std::cout << "num_positions_visited: " << num_positions_visited << std::endl; */
}
