#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Point {
  int x, y;
  bool operator<(const Point& other) const {
    return x < other.x || (x == other.x && y < other.y);
  }
};

class AntennaSolver {
  std::vector<std::string> grid;
  int rows, cols;

  bool isCollinear(const Point& p1, const Point& p2, const Point& p3) {
    return (p2.y - p1.y) * (p3.x - p1.x) == (p3.y - p1.y) * (p2.x - p1.x);
  }

  bool checkAntinode(const Point& a1, const Point& p, const Point& a2) {
    if (!isCollinear(a1, p, a2)) return false;

    double d1 = sqrt(pow(p.x - a1.x, 2) + pow(p.y - a1.y, 2));
    double d2 = sqrt(pow(p.x - a2.x, 2) + pow(p.y - a2.y, 2));
    double ratio = d1 > d2 ? d1 / d2 : d2 / d1;

    return fabs(ratio - 2.0) < 0.1;
  }

 public:
  AntennaSolver(const std::vector<std::string>& g) : grid(g) {
    rows = grid.size();
    cols = grid[0].size();
  }

  int solve() {
    std::map<char, std::vector<Point>> freqs;
    std::set<Point> antinodes;

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        char c = grid[y][x];
        if (c != '.' && c != ' ') {
          freqs[c].push_back({x, y});
        }
      }
    }

    for (const auto& [freq, antennas] : freqs) {
      for (size_t i = 0; i < antennas.size(); i++) {
        for (size_t j = i + 1; j < antennas.size(); j++) {
          const Point& a1 = antennas[i];
          const Point& a2 = antennas[j];

          for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
              Point p{x, y};
              if (checkAntinode(a1, p, a2)) {
                antinodes.insert(p);
              }
            }
          }
        }
      }
    }

    return antinodes.size();
  }
};

int main() {
  std::vector<std::string> grid;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!line.empty()) grid.push_back(line);
  }

  AntennaSolver solver(grid);
  std::cout << solver.solve() << std::endl;
  return 0;
}