#include <fstream>
#include <iostream>
#include <regex>
#include <string>

// Procesa una cadena para encontrar y sumar todas las multiplicaciones válidas
class MultiplicationParser {
 public:
  static int ProcessInput(const std::string& input) {
    std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

    std::smatch matches;
    std::string::const_iterator search_start(input.cbegin());
    int total_sum = 0;

    while (std::regex_search(search_start, input.cend(), matches, pattern)) {
      int x = std::stoi(matches[1]);
      int y = std::stoi(matches[2]);

      total_sum += x * y;

      search_start = matches.suffix().first;
    }
    return total_sum;
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
    exit(1);
  }

  std::ifstream input_file(argv[1]);
  if (!input_file) {
    std::cerr << "Error: Al abrir el archivo: " << argv[1] << std::endl;
    exit(1);
  }

  std::string line;
  int total = 0;

  while (std::getline(input_file, line)) {
    total += MultiplicationParser::ProcessInput(line);
  }

  std::cout << "La suma es de todas las multiplicaciones es: " << total
            << std::endl;

  return 0;
}
