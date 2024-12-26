#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Equation {
  long long test_value;
  std::vector<int> numbers;
};

// Evaluates an expression with operators from left to right
long long EvaluateExpression(const std::vector<int>& numbers,
                             const std::vector<char>& operators) {
  long long result = numbers[0];
  for (size_t i = 0; i < operators.size(); ++i) {
    switch (operators[i]) {
      case '+':
        result += numbers[i + 1];
        break;
      case '*':
        result *= numbers[i + 1];
        break;
      case '|':  // Concatenation
        std::string concat =
            std::to_string(result) + std::to_string(numbers[i + 1]);
        result = std::stoll(concat);
        break;
    }
  }
  return result;
}

// Generates all possible operator combinations including concatenation
std::vector<std::vector<char>> GenerateOperatorCombinations(size_t length) {
  std::vector<std::vector<char>> combinations;
  const char ops[] = {'+', '*', '|'};
  int total = 1;
  for (size_t i = 0; i < length; ++i) {
    total *= 3;  // 3 operators, so 3^length combinations
  }

  for (int i = 0; i < total; ++i) {
    std::vector<char> current_ops;
    int temp = i;
    for (size_t j = 0; j < length; ++j) {
      current_ops.push_back(ops[temp % 3]);
      temp /= 3;
    }
    combinations.push_back(current_ops);
  }
  return combinations;
}

// Parses an equation from a string
Equation ParseEquation(const std::string& line) {
  std::istringstream iss(line);
  Equation eq;

  // Read test value
  std::string value_str;
  std::getline(iss, value_str, ':');
  eq.test_value = std::stoll(value_str);

  // Read numbers
  int num;
  while (iss >> num) {
    eq.numbers.push_back(num);
  }

  return eq;
}

// Checks if an equation can be satisfied with any operator combination
bool CanSatisfyEquation(const Equation& eq) {
  if (eq.numbers.size() < 2) return false;

  auto combinations = GenerateOperatorCombinations(eq.numbers.size() - 1);
  for (const auto& ops : combinations) {
    try {
      if (EvaluateExpression(eq.numbers, ops) == eq.test_value) {
        return true;
      }
    } catch (const std::exception&) {
      // Skip invalid combinations (e.g., overflow)
      continue;
    }
  }
  return false;
}

int main() {
  std::vector<Equation> equations;
  std::string line;

  // Read input
  while (std::getline(std::cin, line)) {
    if (!line.empty()) {
      equations.push_back(ParseEquation(line));
    }
  }

  // Calculate result
  long long total = 0;
  for (const auto& eq : equations) {
    if (CanSatisfyEquation(eq)) {
      total += eq.test_value;
    }
  }

  std::cout << "Total calibration result: " << total << std::endl;
  return 0;
}