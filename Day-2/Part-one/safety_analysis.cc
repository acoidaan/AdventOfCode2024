#include "safety_analysis.h"

// Returns true if all differences between adjacent numbers are within [1,3]
bool HasValidDifferences(const std::vector<int>& levels) {
  for (size_t i = 1; i < levels.size(); ++i) {
    int diff = std::abs(levels[i] - levels[i - 1]);
    if (diff < 1 || diff > 3) return false;
  }
  return true;
}

// Returns true if the sequence is stricly monotonic
bool IsMonotonic(const std::vector<int>& levels) {
  if (levels.size() <= 1) return true;

  // Check for increase
  bool increasing = true;
  for (size_t i = 1; i < levels.size() && increasing; ++i) {
    if (levels[i] <= levels[i - 1]) {
      increasing = false;
    }
  }

  if (increasing) return true;

  // Check for decrease
  bool decreasing = true;
  for (size_t i = 1; i < levels.size() && decreasing; ++i) {
    if (levels[i] >= levels[i - 1]) {
      decreasing = false;
    }
  }
  return decreasing;
}

// Return true if a report is safe according to all rules
bool IsSafeReport(const std::vector<int>& levels) {
  return IsMonotonic(levels) && HasValidDifferences(levels);
}

// Parse a single line into a vector of integers
std::vector<int> ParseLine(const std::string& line) {
  std::vector<int> levels;
  std::istringstream iss(line);
  int num;
  while (iss >> num) {
    levels.emplace_back(num);
  }
  return levels;
}

// Count safe reports in the input
int CountSafeReports(const std::vector<std::string>& input) {
  int safe_count = 0;
  for (const std::string& line : input) {
    std::vector<int> levels = ParseLine(line);
    if (IsSafeReport(levels)) {
      ++safe_count;
    }
  }
  return safe_count;
}