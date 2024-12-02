#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "safety_analysis.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.txt>" << std::endl;
    exit(1);
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Error: Could not open file: " << argv[1] << std::endl;
    exit(1);
  }

  std::vector<std::string> reports;
  std::string line;

  while (std::getline(input, line)) {
    if (!line.empty()) {
      reports.emplace_back(line);
    }
  }

  input.close();

  int safe_count = CountSafeReports(reports);
  std::cout << safe_count << std::endl;

  return 0;
}