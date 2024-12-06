#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>

constexpr char _obstruction_ {'#'};
constexpr char _visited_tile_ {'X'};
constexpr char _clear_tile_ {'.'};

enum class result {
	moved,
	ended,
	loop
};

enum class direction {
	up,
	down,
	left,
	right
};

const std::map<const char, const direction> guard_char_directions = {
	{'^', direction::up},
	{'v', direction::down},
	{'<', direction::left},
	{'>', direction::right},
};

struct place {
	int row;
	int col;
	direction dir;
};

class guard {
	public:

		guard(
				std::vector<std::string> map) 
			: 
					_history()
				,	_map(map)
		{
				_rsize = map.size();
				_csize = map[0].size();
				find_start();
		};

		void find_start() {
			for (int row = 0; row != _rsize; ++row) {
				for (int col = 0; col != _rsize; ++col) {
					const char candidate = _map[row][col];	
					if (guard_char_directions.count(candidate) != 0) {
						_current.dir = guard_char_directions.at(candidate);
						_current.row = row;
						_current.col = col;
					}
				}
			}
		}

		place get_next(const direction where) {
			place next = _current;
			switch(where) {
				case direction::up: {
					next.row--;
					break;
				}
				case direction::down: {
					next.row++;
					break;
				}
				case direction::left: {
					next.col--;
					break;
				}
				case direction::right: {
					next.col++;
					break;
				}
				default:
					std::terminate();
			}
			return next;
		}

		bool in_the_map(const place &to){
			return (to.col >= 0 && to.row >= 0 && to.col < _csize && to.row < _rsize);
		}

		bool is_looping(const place &to) {
			return std::any_of(
					_history.cbegin(), 
					_history.cend(), 
					[&](const place &pl) {
						return (pl.dir == to.dir && pl.row == to.row &&	pl.col == to.col);
					}
				);
		}

		void move(const place& next){
			_current = next;
		}

		bool is_obstructed(const place &pl) {
			return _map[pl.row][pl.col] == _obstruction_;
		}

		void rotate() {
			switch(_current.dir) {
				case direction::up:
					{
					_current.dir = direction::right;
					return;
					}
				case direction::right:
					{
					_current.dir = direction::down;
					return;
					}
				case direction::down:
					{
					_current.dir = direction::left;
					return;
					}
				case direction::left:
					{
					_current.dir = direction::up;
					return;
					}
				default:
					std::terminate();
			}
		}

		result move() {
			_history.push_back(_current);
			const place next = get_next(_current.dir);
			if (is_looping(next)) {
				return result::loop;
			}
			if (!in_the_map(next)) {
				return result::ended;
			}
			if (is_obstructed(next)) {
				rotate();
			}
			else {
				move(next);
			}
			return result::moved;
		}

	private:
		place _current;
		std::vector<place> _history;
		int _rsize;
		int _csize;
		std::vector<std::string> _map;
};

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

	std::vector<std::vector<std::string>> maps_with_extra_obstructions;
	const int row_size = static_cast<int>(map.size());
	const int col_size = static_cast<int>(map[0].size());
	for (int row_idx = 0; row_idx != row_size; ++row_idx) {
		for (int col_idx = 0; col_idx != col_size; ++col_idx) {
			if (map[row_idx][col_idx] == _clear_tile_) {
				std::vector<std::string> map_with_obstruction = map;
				map_with_obstruction[row_idx][col_idx] = _obstruction_;
				maps_with_extra_obstructions.push_back(map_with_obstruction);
			}
		}
	}
	
	int num_loop_positions {0};

	for (auto &modmap : maps_with_extra_obstructions) {
			auto res = result::moved;
			guard grd(modmap);
			while(res == result::moved){
				res = grd.move();	
			}
			if (res == result::loop) {
				num_loop_positions++;
			}
	}

	std::cout << "num_loop_positions: " << num_loop_positions << std::endl;
}
