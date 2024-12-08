#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

struct Point {
  int row, col;
  bool operator<(const Point& other) const {
    return row < other.row || (row == other.row && col < other.col);
  }
  bool operator==(const Point& other) const {
    return row == other.row && col == other.col;
  }
};

struct Path {
  std::vector<std::vector<char>> pattern;
  std::set<Point> points;
};

class GuardSimulator {
 public:
  GuardSimulator(const std::vector<std::string>& map)
      : map_(map), rows_(map.size()), cols_(map[0].size()) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (map[i][j] == '^') {
          start_ = {i, j};
          return;
        }
      }
    }
  }

  Path GetOriginalPath() {
    std::vector<std::vector<char>> pattern(rows_,
                                           std::vector<char>(cols_, '.'));
    std::set<Point> points;
    Point pos = start_;
    int dir = 0;  // 0: up, 1: right, 2: down, 3: left

    while (true) {
      points.insert(pos);
      pattern[pos.row][pos.col] = GetDirChar(dir);

      auto next = GetNextPos(pos, dir);
      if (!IsInBounds(next) || map_[next.row][next.col] == '#') {
        dir = (dir + 1) % 4;
        continue;
      }
      if (next == pos) break;
      pos = next;
    }

    return {pattern, points};
  }

  int CountLoopPoints() {
    Path originalPath = GetOriginalPath();
    std::set<Point> candidates;

    for (const auto& point : originalPath.points) {
      for (int dir = 0; dir < 4; ++dir) {
        Point adj = GetNextPos(point, dir);
        if (IsInBounds(adj) && map_[adj.row][adj.col] == '.' && adj != start_ &&
            originalPath.points.find(adj) == originalPath.points.end()) {
          candidates.insert(adj);
        }
      }
    }

    int count = 0;
    for (const auto& point : candidates) {
      if (TestPoint(point)) {
        count++;
      }
    }
    return count;
  }

 private:
  bool TestPoint(Point obs) {
    Point pos = start_;
    int dir = 0;
    std::set<std::pair<Point, int>> visited;

    while (true) {
      auto state = std::make_pair(pos, dir);
      if (visited.find(state) != visited.end()) return true;
      visited.insert(state);

      auto next = GetNextPos(pos, dir);
      if (!IsInBounds(next)) return false;
      if (next == obs || map_[next.row][next.col] == '#') {
        dir = (dir + 1) % 4;
      } else {
        pos = next;
      }
    }
  }

  char GetDirChar(int dir) {
    if (dir % 2 == 0) return '|';  // vertical
    return '-';                    // horizontal
  }

  Point GetNextPos(Point p, int dir) const {
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};
    return {p.row + dr[dir], p.col + dc[dir]};
  }

  bool IsInBounds(Point p) const {
    return p.row >= 0 && p.row < rows_ && p.col >= 0 && p.col < cols_;
  }

  const std::vector<std::string>& map_;
  int rows_, cols_;
  Point start_;
};

std::vector<std::string> ReadInput(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<std::string> map;
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty()) map.push_back(line);
  }
  return map;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }
  auto map = ReadInput(argv[1]);
  GuardSimulator sim(map);
  std::cout << "Result: " << sim.CountLoopPoints() << '\n';
  return 0;
}