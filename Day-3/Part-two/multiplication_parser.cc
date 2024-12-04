#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <string>

class MultiplicationParser {
  static std::stack<bool> enabledStack;

 public:
  static void reset() {
    while (!enabledStack.empty()) enabledStack.pop();
    enabledStack.push(true);
  }

  static bool isEnabled() {
    return !enabledStack.empty() && enabledStack.top();
  }

  static int ProcessInput(const std::string& input) {
    int total_sum = 0;
    size_t pos = 0;
    bool inDisabledSection = false;

    while (pos < input.length()) {
      if (pos + 4 <= input.length() && input.substr(pos, 4) == "do()" &&
          (pos == 0 || !isalnum(input[pos - 1]))) {
        inDisabledSection = false;
        pos += 4;
      } else if (pos + 7 <= input.length() &&
                 input.substr(pos, 7) == "don't()" &&
                 (pos == 0 || !isalnum(input[pos - 1]))) {
        inDisabledSection = true;
        pos += 7;
      } else if (pos + 4 <= input.length() && input.substr(pos, 4) == "mul(" &&
                 (pos == 0 || !isalnum(input[pos - 1]))) {
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
              if (!inDisabledSection) {
                total_sum +=
                    static_cast<int64_t>(std::stoi(num1)) * std::stoi(num2);
              }
            }
          }
          pos = end + 1;
        } else {
          pos++;
        }
      } else {
        pos++;
      }
    }
    return total_sum;
  }
};

std::stack<bool> MultiplicationParser::enabledStack;

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

  std::cout << "Total: " << total << std::endl;
  return 0;
}