#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class XmasSearchSolver {
 public:
  explicit XmasSearchSolver(const std::vector<std::string>& grid)
      : grid_(grid), height_(grid.size()), width_(grid[0].size()) {}

  int CountXmasOccurrences() {
    int count = 0;

    const std::vector<std::pair<int, int>> directions = {
        {0, 1},    // derecha
        {1, 0},    // abajo
        {1, 1},    // diagonal derecha-abajo
        {1, -1},   // diagonal izquierda-abajo
        {0, -1},   // izquierda
        {-1, 0},   // arriba
        {-1, -1},  // diagonal izquierda-arriba
        {-1, 1}    // diagonal derecha-arriba
    };

    for (int row = 0; row < height_; ++row) {
      for (int col = 0; col < width_; ++col) {
        for (const auto& dir : directions) {
          if (CheckXmasAtPosition(row, col, dir.first, dir.second)) {
            ++count;
          }
        }
      }
    }

    return count;
  }

 private:
  bool CheckXmasAtPosition(int row, int col, int drow, int dcol) {
    const std::string target = "XMAS";

    int end_row = row + drow * 3;
    int end_col = col + dcol * 3;

    if (end_row < 0 || end_row >= height_ || end_col < 0 || end_col >= width_) {
      return false;
    }

    for (int i = 0; i < 4; ++i) {
      if (grid_[row + drow * i][col + dcol * i] != target[i]) {
        return false;
      }
    }

    return true;
  }

  const std::vector<std::string>& grid_;
  const int width_;
  const int height_;
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " archivo.txt" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "No se pudo abrir el archivo: " << argv[1] << std::endl;
    return 1;
  }

  std::vector<std::string> grid;
  std::string line;

  // Leer línea por línea y almacenar en el vector
  while (std::getline(file, line)) {
    if (!line.empty()) {  // Ignorar líneas vacías
      grid.push_back(line);
    }
  }

  if (grid.empty()) {
    std::cerr << "El archivo está vacío" << std::endl;
    return 1;
  }

  XmasSearchSolver solver(grid);
  std::cout << solver.CountXmasOccurrences() << std::endl;

  return 0;
}