#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

class GuardSimulator {
 public:
  GuardSimulator(const std::vector<std::string>& map)
      : map_(map), rows_(map.size()), cols_(map[0].size()) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (map_[i][j] == '^') {
          current_pos_ = {i, j};
          direction_ = 0;  // 0: up, 1: right, 2: down, 3: left
          std::cout << "Start at: " << i << "," << j << std::endl;
          return;
        }
      }
    }
  }

  int SimulatePatrol() {
    std::set<std::pair<int, int>> visited;
    visited.insert(current_pos_);
    int steps = 0;

    while (true) {
      steps++;
      if (steps % 10000 == 0) {
        std::cout << "Step " << steps << " at " << current_pos_.first << ","
                  << current_pos_.second << " dir " << direction_ << std::endl;
      }

      auto next_pos = GetNextPosition();
      if (!IsInBounds(next_pos.first, next_pos.second)) {
        std::cout << "Left map at: " << next_pos.first << "," << next_pos.second
                  << std::endl;
        return visited.size();
      }

      if (map_[next_pos.first][next_pos.second] == '#') {
        direction_ = (direction_ + 1) % 4;
      } else {
        current_pos_ = next_pos;
        visited.insert(current_pos_);
      }
    }
  }

 private:
  bool IsInBounds(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
  }

  std::pair<int, int> GetNextPosition() const {
    const int dr[] = {-1, 0, 1, 0};  // up, right, down, left
    const int dc[] = {0, 1, 0, -1};
    return {current_pos_.first + dr[direction_],
            current_pos_.second + dc[direction_]};
  }

  std::vector<std::string> map_;
  int rows_;
  int cols_;
  std::pair<int, int> current_pos_;
  int direction_;
};

std::vector<std::string> ReadInput(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open " << filename << std::endl;
    exit(1);
  }

  std::vector<std::string> map;
  std::string line;

  while (std::getline(file, line)) {
    if (!line.empty()) {
      map.push_back(line);
    }
  }
  return map;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  auto map = ReadInput(argv[1]);
  GuardSimulator simulator(map);
  std::cout << "Result: " << simulator.SimulatePatrol() << std::endl;
  return 0;
}