#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class MultiplicationParser {
 public:
  static int ProcessInput(const std::string& input) {
    int total_sum = 0;
    size_t pos = 0;
    bool enabled = true;  // Estado inicial: habilitado

    while (pos < input.length()) {
      // Buscar el siguiente token relevante
      size_t next_do = input.find("do()", pos);
      size_t next_dont = input.find("don't()", pos);
      size_t next_mul = FindNextValidMul(input, pos);

      // Si no hay más tokens, terminamos
      if (next_do == std::string::npos && next_dont == std::string::npos &&
          next_mul == std::string::npos) {
        break;
      }

      // Encontrar el token más cercano
      size_t next_pos = std::string::npos;
      char next_token = 'n';

      // Verificar do()
      if (next_do != std::string::npos &&
          (next_pos == std::string::npos || next_do < next_pos)) {
        next_pos = next_do;
        next_token = 'd';
      }

      // Verificar don't()
      if (next_dont != std::string::npos &&
          (next_pos == std::string::npos || next_dont < next_pos)) {
        next_pos = next_dont;
        next_token = 'x';
      }

      // Verificar mul
      if (next_mul != std::string::npos &&
          (next_pos == std::string::npos || next_mul < next_pos)) {
        next_pos = next_mul;
        next_token = 'm';
      }

      // Procesar el token encontrado
      if (next_token == 'd') {
        enabled = true;
        pos = next_do + 4;
      } else if (next_token == 'x') {
        enabled = false;
        pos = next_dont + 7;
      } else if (next_token == 'm') {
        auto [x, y] = ExtractNumbers(input, next_mul);
        if (enabled) {
          total_sum += x * y;
        }
        pos = next_mul + 1;  // Avanzar solo un caracter para no perder tokens
      } else {
        ++pos;  // Solo por seguridad
      }
    }

    return total_sum;
  }

 private:
  // Encuentra la siguiente multiplicación válida
  static size_t FindNextValidMul(const std::string& input, size_t start) {
    size_t pos = start;
    while ((pos = input.find("mul", pos)) != std::string::npos) {
      if (pos + 3 < input.length() && input[pos + 3] == '(') {
        size_t close = input.find(')', pos);
        if (close != std::string::npos) {
          size_t comma = input.find(',', pos);
          if (comma != std::string::npos && comma < close) {
            // Verificar números
            auto [x, y] = ExtractNumbers(input, pos);
            if (x >= 0 && x <= 999 && y >= 0 && y <= 999) {
              return pos;
            }
          }
        }
      }
      ++pos;
    }
    return std::string::npos;
  }

  // Extrae los números de una multiplicación
  static std::pair<int, int> ExtractNumbers(const std::string& input,
                                            size_t pos) {
    try {
      size_t open = input.find('(', pos);
      size_t comma = input.find(',', open);
      size_t close = input.find(')', comma);

      std::string num1 = input.substr(open + 1, comma - open - 1);
      std::string num2 = input.substr(comma + 1, close - comma - 1);

      return {std::stoi(num1), std::stoi(num2)};
    } catch (...) {
      return {0, 0};
    }
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
    return 1;
  }

  std::ifstream input_file(argv[1]);
  if (!input_file) {
    std::cerr << "Error: No se pudo abrir el archivo " << argv[1] << "\n";
    return 1;
  }

  std::string line;
  int total = 0;

  while (std::getline(input_file, line)) {
    total += MultiplicationParser::ProcessInput(line);
  }

  std::cout << "La suma total de todas las multiplicaciones habilitadas es: "
            << total << std::endl;

  return 0;
}