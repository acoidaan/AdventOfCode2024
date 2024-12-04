#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>

class XmasSearchSolver {
 public:
  explicit XmasSearchSolver(const std::vector<std::string>& grid)
      : grid_(grid), height_(grid.size()), width_(grid[0].size()) {}

  int CountXMASPatterns() {
    int count = 0;

    for (int row = 1; row < height_ - 1; ++row) {
      for (int col = 1; col < width_ - 1; ++col) {
        // Para cada posición central (A), verificar las 4 posibles
        // orientaciones
        if (CheckPattern(row, col)) {
          ++count;
        }
      }
    }

    return count;
  }

 private:
  bool CheckPattern(int center_row, int center_col) {
    // El centro debe ser 'A'
    if (grid_[center_row][center_col] != 'A') {
      return false;
    }

    // Verificar las 4 posibles orientaciones para las dos "MAS"
    const std::vector<std::array<std::pair<int, int>, 4>> orientations = {
        // Diagonal arriba-izq a abajo-der y arriba-der a abajo-izq
        std::array<std::pair<int, int>, 4>{
            {{-1, -1}, {1, 1}, {-1, 1}, {1, -1}}},
        // Las mismas diagonales pero invirtiendo una de las "MAS"
        std::array<std::pair<int, int>, 4>{
            {{1, 1}, {-1, -1}, {-1, 1}, {1, -1}}},
        std::array<std::pair<int, int>, 4>{
            {{-1, -1}, {1, 1}, {1, -1}, {-1, 1}}},
        std::array<std::pair<int, int>, 4>{
            {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}}}};

    for (const auto& orientation : orientations) {
      // Verificar las dos "MAS" en esta orientación
      if (CheckMAS(center_row, center_col, orientation[0].first,
                   orientation[0].second, orientation[1].first,
                   orientation[1].second) &&
          CheckMAS(center_row, center_col, orientation[2].first,
                   orientation[2].second, orientation[3].first,
                   orientation[3].second)) {
        return true;
      }
    }

    return false;
  }

  bool CheckMAS(int center_row, int center_col, int m_dir_row, int m_dir_col,
                int s_dir_row, int s_dir_col) {
    int m_row = center_row + m_dir_row;
    int m_col = center_col + m_dir_col;
    int s_row = center_row + s_dir_row;
    int s_col = center_col + s_dir_col;

    // Verificar límites
    if (m_row < 0 || m_row >= height_ || m_col < 0 || m_col >= width_ ||
        s_row < 0 || s_row >= height_ || s_col < 0 || s_col >= width_) {
      return false;
    }

    // Verificar "MAS" o "SAM"
    return (grid_[m_row][m_col] == 'M' && grid_[s_row][s_col] == 'S') ||
           (grid_[m_row][m_col] == 'S' && grid_[s_row][s_col] == 'M');
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

  while (std::getline(file, line)) {
    if (!line.empty()) {
      grid.push_back(line);
    }
  }

  if (grid.empty()) {
    std::cerr << "El archivo está vacío" << std::endl;
    return 1;
  }

  XmasSearchSolver solver(grid);
  std::cout << solver.CountXMASPatterns() << std::endl;

  return 0;
}