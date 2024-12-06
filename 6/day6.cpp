#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <chrono>
#include <unordered_set>

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

struct place_hash {
    std::size_t operator()(const place &p) const {
        return std::hash<int>()(p.row) 
             ^ (std::hash<int>()(p.col) << 1) 
             ^ (std::hash<int>()(static_cast<int>(p.dir)) << 2);
    }
};

struct place_equal {
    bool operator()(const place &a, const place &b) const {
        return a.row == b.row && a.col == b.col && a.dir == b.dir;
    }
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
				for (int col = 0; col != _csize; ++col) {
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
			return _history.find(to) != _history.end();
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
			_history.insert(_current);
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
		std::unordered_set<place, place_hash, place_equal> _history;
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

	auto t1 = std::chrono::high_resolution_clock::now();

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

	auto t2 = std::chrono::high_resolution_clock::now();
	
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

	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "num_loop_positions: " << num_loop_positions << std::endl;

	auto ms_first = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	auto ms_second= std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

	std::cout << "durations: [" << ms_first.count() << ", " << ms_second.count() << "]" << std::endl;
}
