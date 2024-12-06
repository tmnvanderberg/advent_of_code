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


void print_map(const std::vector<std::string>& map){
	std::cout << "-----map------" << '\n';
	for (const auto& row: map){
		std::cout << row << std::endl;
	}
}

bool increment_guard_position(std::vector<std::string> &map) {
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
								std::cout << "next: " << next << std::endl;
								if (next != _obstruction_){
										std::cout << "=> clear tile" << std::endl;
										map[new_row_idx][col_idx] = _guard_up_;
										map[row_idx][col_idx] = _visited_tile_;
								}
								if (next == _obstruction_) {
									// turn right, don't move
									std::cout << "obstruction" << std::endl;		
									map[row_idx][col_idx] = _guard_right_;
								}
								return true;
							}
							return false;
						}
					case _guard_left_:
						{
							std::cout << "guard left at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_col_idx = static_cast<int>(col_idx) - 1;
							if (new_col_idx >= 0 && new_col_idx < col_size) { 
								const char next = map[row_idx][new_col_idx];
								std::cout << "next: " << next << std::endl;
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
								return true;
							}
							return false;
						}
					case _guard_right_:
						{
							std::cout << "guard right at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_col_idx = static_cast<int>(col_idx) + 1;
							if (new_col_idx >= 0 && new_col_idx < col_size) { 
								const char next = map[row_idx][new_col_idx];
								std::cout << "next: " << next << std::endl;
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
								return true;
							}
							return false;
						}
					case _guard_down_:
						{
							std::cout << "guard down at (" << row_idx << ", " << col_idx << ")" << std::endl;
							const int new_row_idx = static_cast<int>(row_idx) + 1;
							if (new_row_idx >= 0 && new_row_idx < row_size) { 
								const char next = map[new_row_idx][col_idx];
								std::cout << "next: " << next << std::endl;
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
								return true;
							}
							return false;
						}
					default:
						std::cout << "wtf." << std::flush;
				}
			}
		}
	}
	std::terminate();
}

int main() {
	std::vector<std::string> map;
	std::string line;
	std::ifstream file("input.txt");

	if (file.is_open()) {
		while (std::getline(file, line)) {
				map.push_back(line);
		}
		file.close();
	} else {
		std::cerr << "Unable to open file" << std::endl;
		return 1;
	}

	/* std::vector<std::string> map { */
	/* 	"....#.....", */
	/* 	".........#", */
	/* 	"..........", */
	/* 	"..#.......", */
	/* 	".......#..", */
	/* 	"..........", */
	/* 	".#..^.....", */
	/* 	"........#.", */
	/* 	"#.........", */
	/* 	"......#...", */
	/* }; */

	while (increment_guard_position(map)) {
		/* print_map(map); */
	}

	print_map(map);

	const int num_positions_visited = std::accumulate(
		map.cbegin(), 
		map.cend(), 
		1,
		[&](int sum, const std::string &col) -> int { 
			return sum + std::count(col.cbegin(), col.cend(), _visited_tile_);	
		}
	);

	std::cout << "num_positions_visited: " << num_positions_visited << std::endl;
}
