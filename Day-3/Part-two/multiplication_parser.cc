#include <fstream>
#include <iostream>
#include <regex>
#include <string>

class MultiplicationParser {
  static bool enabled;

 public:
  static void reset() { enabled = true; }

  static int ProcessInput(const std::string& input) {
    int total_sum = 0;
    size_t pos = 0;

    while (pos < input.length()) {
      auto isValidCommand = [&](size_t p) {
        return p == 0 || !isalnum(input[p - 1]);
      };

      if (pos + 4 <= input.length() && input.substr(pos, 4) == "do()" &&
          isValidCommand(pos)) {
        enabled = true;
        pos += 4;
        continue;
      }

      if (pos + 7 <= input.length() && input.substr(pos, 7) == "don't()" &&
          isValidCommand(pos)) {
        enabled = false;
        pos += 7;
        continue;
      }

      if (pos + 4 <= input.length() && input.substr(pos, 4) == "mul(" &&
          isValidCommand(pos)) {
        size_t end = input.find(')', pos);
        if (end != std::string::npos) {
          std::string nums = input.substr(pos + 4, end - (pos + 4));
          size_t comma = nums.find(',');
          if (comma != std::string::npos) {
            std::string num1 = nums.substr(0, comma);
            std::string num2 = nums.substr(comma + 1);

            num1.erase(remove_if(num1.begin(), num1.end(), ::isspace),
                       num1.end());
            num2.erase(remove_if(num2.begin(), num2.end(), ::isspace),
                       num2.end());

            if (std::regex_match(num1, std::regex("\\d{1,3}")) &&
                std::regex_match(num2, std::regex("\\d{1,3}"))) {
              if (enabled) {
                total_sum += std::stoi(num1) * std::stoi(num2);
              }
            }
          }
          pos = end + 1;
          continue;
        }
      }
      pos++;
    }
    return total_sum;
  }
};

bool MultiplicationParser::enabled = true;

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

  std::string content((std::istreambuf_iterator<char>(input_file)),
                      std::istreambuf_iterator<char>());

  MultiplicationParser::reset();
  int64_t total = MultiplicationParser::ProcessInput(content);

  std::cout << "La suma es de todas las multiplicaciones habilitadas es: "
            << total << std::endl;
  return 0;
}