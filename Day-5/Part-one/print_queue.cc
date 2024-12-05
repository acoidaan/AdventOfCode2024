#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class PrintQueueValidator {
 public:
  struct Rule {
    int before, after;
  };

  void AddRule(const std::string& rule) {
    size_t separator = rule.find('|');
    if (separator == std::string::npos) return;

    int before = std::stoi(rule.substr(0, separator));
    int after = std::stoi(rule.substr(separator + 1));
    dependencies_[after].insert(before);
  }

  bool IsValidSequence(const std::vector<int>& seq) {
    std::unordered_set<int> pages(seq.begin(), seq.end());
    std::unordered_set<int> seen;

    for (size_t i = 0; i < seq.size(); ++i) {
      int current = seq[i];
      if (dependencies_.count(current)) {
        for (int required : dependencies_[current]) {
          if (pages.count(required) && !seen.count(required)) {
            return false;
          }
        }
      }
      seen.insert(current);
    }
    return true;
  }

  int ProcessUpdate(const std::string& line) {
    std::vector<int> seq;
    std::stringstream ss(line);
    std::string num;
    while (std::getline(ss, num, ',')) {
      seq.push_back(std::stoi(num));
    }
    return IsValidSequence(seq) ? seq[seq.size() / 2] : 0;
  }

 private:
  std::unordered_map<int, std::unordered_set<int>> dependencies_;
};

int main(int argc, char* argv[]) {
  if (argc != 2) return 1;
  std::ifstream input(argv[1]);
  if (!input) return 1;

  PrintQueueValidator validator;
  std::string line;
  bool processing_rules = true;
  int sum = 0;

  while (std::getline(input, line)) {
    if (line.empty()) {
      processing_rules = false;
      continue;
    }
    if (processing_rules)
      validator.AddRule(line);
    else
      sum += validator.ProcessUpdate(line);
  }

  std::cout << sum << std::endl;
  return 0;
}